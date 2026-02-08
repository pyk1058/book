/*
  1. 옥상에서 양액을 자동으로 공급하는 program이다.
  2. MQTT를 이용한다.

  1. calibration
  2. 수위 sensing
    (1) min.에서 채워질 때는 짧은 주기로 sensing한다.
    (2) max.에서 소모될 때는 긴 주기로 sensing한다.
  3. 수위가 min.에 도달하면 max.이 아닐 동안 pump를 구동한다.

  1. 4-pin 수위 calibration: v_low_ref, v_high_ref 구하기
  2. v_low_ref와 v_high_ref를 EEPROM에 보관한다. v_low_ref: v_ref[2], v_high_ref: v_ref[3]
  3. EEPROM에 쓰고 읽는 것은 ESP8266와 arduino가 다르다. 참고: https://www.aranacorp.com/en/using-the-eeprom-with-the-esp8266/
  4. ESP8266에서는 begin(), commit(), end() 명령을 사용해야 한다.

  1. empty measurement
    (1) 액체가 없는 상태로 만든다.
    (2) 5회 읽어서 평균을 내어 v_empty에 저장한다.

  2. low level measurement
    (1) 액체를 min 전극봉 끝이 잠길 정도까지 넣는다.
    (2) min 전극봉의 값을 5개 읽어서 평균을 내고, v_low에 저장한다.
    (3) v_low_ref = (v_empty + v_low) / 2로 계산한다.

  3. high level measurement
    (1) 액체를 max 전극봉 끝이 잠길 정도까지 넣는다.
    (2) max 전극봉의 값을 5개 읽어서 평균을 내고, v_high에 저장한다.
    (3) v_high_ref = (v_empty + v_high) / 2로 계산한다.

  4. Subscribe 못할 것을 고려하여 계속 상태를 publish한다.

  2024.09.01
  박영기
*/

#include "myConfig.h"
#include <AimMqtt.h>
#include <EEPROM.h>//https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h
#include <AimTimer.h>

#define select_pin D1 // Wemos D1 mini: GPIO 5. 
#define Vout_pin D2 // Wemod D1 mini: GPIO 4. Sensor에 전압을 공급한다.
#define Vin_pin A0 // Sensor로부터 analog 전입을 읽는다.
#define EEPROM_SIZE 40 // Memory size를 지정해준다. 단위는?

char step;
int select_sensor; // Relay가 sensor를 선택한다. HIGH: high 단자, LOW: low 단자.
int i_select;
int v_empty;
int v_cal, v_ref[3];
int v_get_EEPROM;
int v_address[4] = {0, 10, 20, 30};
int sensing_period = 5; // in seconds
char state;
int VL, VH;

AimMqtt mqtt;
AimTimer sensing_timer;

void setup() {
  Serial.begin(115200);

  pinMode(select_pin, OUTPUT);  
  pinMode(Vout_pin, OUTPUT);
  pinMode(Vin_pin, INPUT);

  sensing_timer.set(sensing_period);
  mqtt.begin(myMqttSub, myMqttCallback);
}

void loop() {
  mqtt.run();

  if (sensing_timer.isOn()) {
    EEPROM.begin(EEPROM_SIZE); // ESP8266에서는 begin() 명령을 사용해야 시작된다. 
    EEPROM.get(v_address[2], v_ref[2]);
    EEPROM.get(v_address[3], v_ref[3]);
    EEPROM.end(); // ESP8266에서는 end() 명령을 사용해야 끝난다. 

    V_distinguish(); // state(L, N, H)를 알려준다.

    if (state == 'L') {     
      while (state != 'H') {
        mqtt.pub("pump", "ON");
        V_distinguish(); // 수위를 읽어서 state = {L, N, H} 중 어떤 것인지 구한다.
        delay(100);
      }
      sensing_timer.set(sensing_period);
    }    
    mqtt.pub("pump", "OFF");
  }
}

void V_distinguish() { // 수위를 읽어서 {L, N, H} 중 어떤 것인지 구한다.
  digitalWrite(select_pin, LOW);
  delay(500);
  VL = read_level();
  digitalWrite(select_pin, HIGH);
  delay(500);
  VH = read_level();

  state = 'N';
  if (VL < v_ref[2]) state = 'L';
  if (VH > v_ref[3]) state = 'H';
  Serial.printf("state: %c \n", state);
}

// 5회 읽은 값의 평균을 낸다.
int read_level() {
  int level;
  int v = 0;

  for (int i = 1; i <= 5; i++) {
    digitalWrite(Vout_pin, HIGH);
    level = analogRead(Vin_pin);
    digitalWrite(Vout_pin, LOW);
    v = v + level;
  }

  v = (int)v/5;
  return v;
}

void myMqttSub() {
  int QoS = 0;
  mqtt.sub("step", QoS);
  mqtt.sub("StartEnd", QoS);
}

void myMqttCallback(const String& topic, const String& payload) {
  Serial.printf("topic: %s, payload: %s \n", topic.c_str(), payload.c_str());
  if (topic == "step") {
    if (payload == "1") {
      step = '1';
      auto_calibration();
    }

    if (payload == "2") {
      step = '2';
      auto_calibration();
    }  

    if (payload == "3") {
      step = '3';
      auto_calibration();
    }
  }

  if (topic == "StartEnd") {
    if (payload == "1") { // calibration mode starts.
      Serial.println("Calibration starts.");
      sensing_timer.set(0);
      mqtt.pub("pump", "OFF");
    }

    if (payload == "0") { // calibration ends.
      Serial.println("Calibration ends.");
      mqtt.pub("pump", "OFF");
      sensing_timer.set(sensing_period);
    }
  }
}