/*
  1. DWC 방식의 스마트 수경재배기 제어부이다.
  2. esp8266용이다.
  3. esp8266에서 serial 통신으로 arduino에 신호를 보낸다. key를 사용한다.
  4. Relay는 arduino가 제어한다. (esp8266의 pin 수가 부족해서.)

  1. 재배기의 내부의 온습도를 관리한다.
    (1) fan1: 외부에서 내부로 공기를 불어 넣는 fan.
    (2) fan2, fan3: 내부의 공기를 순환시키는 fan.
    (3) heater
    (4) cooler
    (5) 제습기

  2. 온습도 제어: 낮 온습도와 밤 온습도를 적용한다.
    (1) 내부 온도가 최소 온도 미만일 때: 
      - 외부 온도가 최소 온도 초과이면: fan1, fan2, fan3 ON 후 OFF. key = '1'
      - 외부 온도가 최소 온도 이하이면: heater, fan2, fan3 ON 후 OFF. key = '2'
    (2) 내부 온도가 최대 온도 초과일 때: 
      - 외부 온도가 최대 온도 미만이면: fan1, fan2, fan3 ON 후 OFF. key = '3'
      - 외부 온도가 최대 온도 이상이면: cooler, fan2, fan3 ON 후 OFF. key = '4'
    (3) 내부 습도가 최소 습도 미만이면: 고려하지 않음.
    (4) 내부 습도가 최대 습도 초과일 때: 
      - 외부 습도가 최대 습도 미만일 때: fan1, fan2, fan3 ON 후 OFF. key = '5'
      - 외부 습도가 최대 습도 이상일 때: 제습기, fan2, fan3 ON 후 OFF. key = '6'
    (5) 위 조건이 아닐 때: fan1을 15초 ON, 15초 OFF. key = '0'

  3. LED ON/OFF
    (1) LED의 ON/OFF 시각을 휴대폰으로 설정한다.
    (2) NTP 시각을 읽어 LED를 ON/OFF한다.
    (3) 휴대폰에서 입력한 시각은 다음과 같이 변환된다.
      오후 11:59 --> 23:59
      오전 12:00 --> 00:00
      오전 12:01 --> 00:01
    (4) LED ON 시간이 자정의 포함 여부를 고려한다.
  4. 재배기 내부의 NTP시각과 온습도를 OLED로 display한다.
  5. 30초마다 sensing한다.
  6. Digital water level sensor로 수위를 측정한다.

  App Inventor program: DWC_smart_cultivator
  arduino program: DWC_smart_cultivator_arduino

  2025.08.24
  박영기

*/

#include "DWC_smart_cultivator.h"

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  delay(10);

  pinMode(water_level_pin, INPUT);
  pinMode(TH_PIN, INPUT);
  pinMode(TH_a_PIN, INPUT);

  dht.begin();
  dht_a.begin();

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");

  // Start the server
  server.begin();
  Serial.println("Server started.");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  ipAddress = WiFi.localIP().toString();

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(32400); // 서울시 시각은 Greenwich Mean Time 시각보다 9시간 빠르다. 9*3,600 = 32,400
  // https://time.is/ko/compare/gmt/Seoul

  u8g2.begin();  // 화면 초기화. 참조: https://github.com/olikraus/u8g2/wiki/u8g2reference#begin

//  TH_sensing();
//  LED_ONOFF();
}

void loop() {
  
  if (display_timer.isOn(display_period)) {
    t_NTP = NTP_time_receive();
    phone_time_receive();
    OLED_display();
    THW_to_phone();
    LED_ONOFF();
  }

  if (sensing_timer.isOn(sensing_period)) {
    day_night_check();
    TH_management();
    W = digitalRead(water_level_pin);
  }
}

int NTP_time_receive() {
  timeClient.update();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int T_minute = 60 * currentHour + currentMinute; // 시각을 분으로 환산한다.
  formattedTime = timeClient.getFormattedTime();
//  Serial.printf("Current time: %s \n", formattedTime);
  return(T_minute);
}

void TH_management() {
  if (TH) {
    TH_sensing();
    key = T_check();
    Serial2.printf("%c %d \n", key, value);
  }
  else {
    TH_sensing();
    key = H_check();
    Serial2.printf("%c %d \n", key, value);
  }
  TH = !TH;
}

void TH_sensing() { 
  T = dht.readTemperature();
  H = dht.readHumidity(); 
  Ta = dht_a.readTemperature();
  Ha = dht_a.readHumidity();
//  T = 35; Tmin = 25; Tmax = 30; Ta = 35;
//  H = 60; Hmax = 50; Ha = 60;
  Serial.printf("T = %f, Tmin = %f, Tmax = %f Ta = %f, H = %f, Hmax = %f, Ha = %f \n", T, Tmin, Tmax, Ta, H, Hmax, Ha);
  snprintf(temp_buf, 6, "%4.1f", T);  // 5는 총 자릿수, 1은 소숫점 이하 자릿수. buffer는 \n을 포함하여 한 자리 수 더 확보. https://reakwon.tistory.com/169
  snprintf(hum_buf, 6, "%4.1f", H); // 5는 총 자릿수, 1은 소숫점 이하 자릿수. buffer는 \n을 포함하여 한 자리 수 더 확보.  https://reakwon.tistory.com/169  
}

char T_check() {
  s = '0';
  if (T < Tmin) {
    if (Ta > Tmin) {
      s = '1';
    }
    else {
      s = '2';
    }
  }
  else {
    if (Ta < Tmax) {
      s = '3';
    }
    else {
      s = '4';
    }
  }
  return s;
}

char H_check() {
  s = '0';
  if (H > Hmax) {
    if (Ha < Hmax) {
      s = '5';
    }
    else {
      s = '6';
    }
  }
  return s;
}

void LED_ONOFF(void) {
  key = '7';
  if (t_LED_set_on < t_LED_set_off) { // LED ON 시간이 자정을 포함하지 않을 때
    if ((t_NTP >= t_LED_set_on) && (t_NTP < t_LED_set_off)) {
      LED_state = HIGH;
    }
    else {
      LED_state = LOW;
    }
  }
  if (t_LED_set_on > t_LED_set_off) { // LED ON 시간이 자정을 포함할 때
    if ((t_NTP >= t_LED_set_on) || (t_NTP < t_LED_set_off)) {
      LED_state = HIGH;
    }
    else {
      LED_state = LOW;
    }
  }

//  Serial.printf("first_LED: %d \n", first_LED);
  if (first_LED == 1) { // 전원 ON, LED ON/OFF 설정값이 바뀌면 1이 된다.
    Serial2.printf("%c %d \n", key, LED_state);
    Serial.printf("LED; key: %c, LED_state: %d \n", key, LED_state);
    first_LED = 0;
    old_LED_state = LED_state;
  }
  else if (LED_state != old_LED_state) { 
    Serial.printf("LED_state: %d, old_LED_state: %d \n", LED_state, old_LED_state);
    Serial2.printf("%c %d \n", key, LED_state);
    Serial.printf("LED; key: %c, LED_state: %d \n", key, LED_state);
    old_LED_state = LED_state;
  }
//  Serial.printf("t_NTP: %d, t_LED_set_on: %d, t_LED_set_off: %d \n", t_NTP, t_LED_set_on, t_LED_set_off);
//  Serial.printf("LED; key: %c, LED_state: %d \n", key, LED_state);
}

void OLED_display() {
  u8g2.firstPage();
  do {    
    u8g2.setFont(u8g2_font_6x13_tf); // https://github.com/olikraus/u8g2/wiki/fntlist12#9-pixel-height
    u8g2.drawStr(IP_x, IP_y, ipAddress.c_str()); 
    u8g2.setFont(u8g2_font_crox5h_tf); // https://github.com/olikraus/u8g2/wiki/fntlist12#9-pixel-height
    u8g2.drawStr(clock_text_x, clock_text_y, formattedTime.c_str()); // c_str()은 string을 char[]로 바꾸어준다.  
    u8g2.setFont(u8g2_font_crox4t_tn); // https://github.com/olikraus/u8g2/wiki/u8g2reference#setfont
    u8g2.drawStr(temp_text_x, temp_hum_y, temp_buf); // https://github.com/olikraus/u8g2/wiki/u8g2reference#drawstr      
    u8g2.drawStr(hum_text_x, temp_hum_y, hum_buf); // https://github.com/olikraus/u8g2/wiki/u8g2reference#drawstr        
    u8g2.setFont(u8g2_font_cu12_t_symbols); 
    u8g2.drawGlyph(temp_unit_x, temp_hum_y, 0x2103); // Icon and Symbol Fonts: https://github.com/olikraus/u8g2/wiki/fnticons    
    u8g2.drawGlyph(hum_unit_x, temp_hum_y, 0x0025); // Icon and Symbol Fonts: https://github.com/olikraus/u8g2/wiki/fnticons  
  } while( u8g2.nextPage() );
}

void day_night_check() {
  if (LED_state == HIGH) { // 낮
    Tmin = Tmin_day;
    Tmax = Tmax_day;
    Hmax = Hmax_day;
  } else { // 밤
    Tmin = Tmin_night;
    Tmax = Tmax_night;
    Hmax = Hmax_night;
  }
}

