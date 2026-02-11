#include <SoftwareSerial.h>
#include <AimTimer.h>
#include <DHT.h> // 지정 library와 기본 library 모두에서 찾는다.
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h> // https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>
#include <EEPROM.h>//https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h
#include <U8g2lib.h> // https://github.com/olikraus/u8g2
#include <SPI.h>
#include <Wire.h>

#define TH_PIN D6 // GPIO12. 재배기 내부 온습도
#define TH_a_PIN D5 // GPIO14. 외부 온습도
#define water_level_pin D7 // GPIO13. 수위계

// OLED pin
#define SDA_PIN D2
#define SCL_PIN D1

// OLED 좌표
#define IP_x 0 // IP 주소의 왼쪽 좌표
#define IP_y 10 // IP 주소의 y 좌표
#define temp_text_x 0 // 온도 text의 왼쪽 좌표
#define temp_unit_x 34 // 온도 단위 왼쪽 좌표
#define hum_text_x 63 // 습도 text의 왼쪽 좌표
#define hum_unit_x 98 // 습도 단위 왼쪽 좌표
#define temp_hum_y 63 // 온습도의 y 좌표
#define clock_text_x 0 // 첫 글짜 왼쪽 아래의 x 좌표
#define clock_text_y 40 // 첫 글짜 왼쪽 아래의 y 좌표
#define EEPROM_SIZE 80 // Memory size를 지정해준다. 단위는?

const char* ssid = "***";     // 여기에 WiFi 네트워크 이름 입력
const char* password = "***"; // 여기에 WiFi 비밀번호 입력

int value = 0; // dummy value
float T, H, Ta, Ha;
float Tmin, Tmax;
float Tmin_day = 17, Tmax_day = 20;
float Tmin_night = 8, Tmax_night = 10;
float Hmax;
float Hmax_day = 60, Hmax_night = 90;
int W = HIGH; // water_level

int sensing_period = 20; // in seconds
int display_period = 1; // in seconds
bool TH = true; // true이면 온도를 다루고, false이면 습도를 다룬다.
char s, key; 
int t_NTP;
int t_LED_set_on, t_LED_set_off;
int LED_state = LOW, old_LED_state, first_LED = 1;
char temp_buf[6]; // float number를 string으로 변환시켜 보관하는 buffer.
char hum_buf[6]; // float number를 string으로 변환시켜 보관하는 buffer.
String formattedTime;
int address_LED_on = 0, address_LED_off = 10;
int Index[4], Time[4];
String request, String_t[4];
int space[4] = {8, 7, 9, 8};
String ipAddress;

SoftwareSerial Serial2(D8, D4); // RX, TX
DHT dht(TH_PIN, DHT22); // 재배기 내의 온습도 센서
DHT dht_a(TH_a_PIN, DHT22); // ambient temperature and humidity
AimTimer sensing_timer;
AimTimer display_timer;
WiFiServer server(80);
WiFiClient client;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN); // https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#ssd1306-128x64_noname-1
// U8G2_R0와 관련된 내용: https://github.com/olikraus/u8g2/wiki/u8g2reference#carduino-example
