/*
부록 2. AI에게 올바르게 질문하는 법

2026.04.24
박영기
*/

#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>

#define LED_PIN 8
#define HEATER_PIN 9
#define FAN_PIN 10
#define DHT_PIN 2
#define DHT_TYPE DHT22   // DHT11이면 DHT11로 변경

RTC_DS3231 rtc;
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  dht.begin();

  if (!rtc.begin()) {
    Serial.println("RTC 오류");
    while (1);
  }

  // 처음 1회만 시간 설정 (업로드 후 주석 처리)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();

  // 🌞 LED 제어 (06:00 ~ 18:00)
  if (hour >= 6 && hour < 18) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // 🌡️ 온도 읽기
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("온도 읽기 실패");
    return;
  }

  Serial.print("온도: ");
  Serial.println(temp);

  // 🔥 온도 제어 (20~25도 유지)
  if (temp < 20) {
    digitalWrite(HEATER_PIN, HIGH); // 히터 ON
    digitalWrite(FAN_PIN, LOW);
  } 
  else if (temp > 25) {
    digitalWrite(HEATER_PIN, LOW);
    digitalWrite(FAN_PIN, HIGH); // 팬 ON
  } 
  else {
    digitalWrite(HEATER_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
  }

  delay(2000);
}