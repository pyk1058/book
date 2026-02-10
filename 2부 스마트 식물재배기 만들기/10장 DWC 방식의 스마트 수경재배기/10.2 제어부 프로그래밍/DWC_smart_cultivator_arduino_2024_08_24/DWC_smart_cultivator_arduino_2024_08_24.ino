/*
  1. esp8266의 serial 신호를 받아서 relay를 선택한다.
  2. Relay의 ON/OFF duration을 정한다.
  3. Arduino용 program이다.

  esp8266 program: DWC_smart_cultivator_esp8266
  App Inventor program: DWC_smart_cultivator

  2025.08.24
  박영기
*/

// software serial통신을 이용한다.

#include <SoftwareSerial.h>
#include <AimOutput.h> // Output의 pin과 duration을 관리한다.

#define fan1_pin 2 // D2
#define fan23_pin 3 // D3
#define heater_pin 4 // D4
#define cooler_pin 5 // D5
#define dehumidifier_pin  6 // D6
#define LED_pin 7 // D7

char key;
int on_duration = 1 * 1000; // in milliseconds
int off_duration = 1 * 1000; // in milliseconds
int fan1_value, fan23_value, heater_value, cooler_value, dehumidifier_value, LED_value;

SoftwareSerial Serial2(8, 9);
//AimOutput fan1_timer(fan1_pin);
//AimOutput fan23_timer(fan23_pin);
//AimOutput heater_timer(heater_pin);
//AimOutput cooler_timer(cooler_pin);
//AimOutput dehumidifier_timer(dehumidifier_pin);
//AimOutput fan1_ONOFF(fan1_pin);

void all_LOW() {
  fan1_value = !LOW;
  fan23_value = !LOW;
  heater_value = !LOW;
  cooler_value = !LOW;
  dehumidifier_value = !LOW;
  LED_value = !LOW;
}

void relay_ONOFF() {
  digitalWrite(fan1_pin, fan1_value);
  digitalWrite(fan23_pin, fan23_value);
  digitalWrite(heater_pin, heater_value);
  digitalWrite(cooler_pin, cooler_value);
  digitalWrite(dehumidifier_pin, dehumidifier_value);
//  digitalWrite(LED_pin, LED_value);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);

  pinMode(fan1_pin, OUTPUT);
  pinMode(fan23_pin, OUTPUT);
  pinMode(heater_pin, OUTPUT);
  pinMode(cooler_pin, OUTPUT);
  pinMode(dehumidifier_pin, OUTPUT);
  pinMode(LED_pin, OUTPUT);

  all_LOW();
  relay_ONOFF();

//  fan1_ONOFF.set(on_duration, off_duration);
}

void loop() {
//  fan1_timer.run();
//  fan23_timer.run();
//  heater_timer.run();
//  cooler_timer.run();
//  dehumidifier_timer.run();
//  fan1_ONOFF.run();

  while (Serial2.available()) {
    key = Serial2.read();
    TH_management();   
  }
}

void TH_management() {
  switch (key) {
    case '1':
      Serial.print("key: "); Serial.println(key);
      Serial.println("fan1: ON, fan2: ON, fan3: ON");
      all_LOW();
      fan1_value = !HIGH;
      fan23_value = !HIGH;
      relay_ONOFF();
//      fan1_timer.on(on_duration);
//      fan23_timer.on(on_duration);
      break;

    case '2':
      Serial.print("key: "); Serial.println(key);
      Serial.println("heater: ON, fan2: ON, fan3: ON");
      all_LOW();
      heater_value = !HIGH;
      fan23_value = !HIGH;
      relay_ONOFF();
//      heater_timer.on(on_duration);
//      fan23_timer.on(on_duration);  
      break;

    case '3':
      Serial.print("key: "); Serial.println(key);
      Serial.println("fan1: ON, fan2: ON, fan3: ON");
      all_LOW();
      fan1_value = !HIGH;
      fan23_value = !HIGH;
      relay_ONOFF();
//      fan1_timer.on(on_duration);
//      fan23_timer.on(on_duration);
      break;

    case '4':
      Serial.print("key: "); Serial.println(key);
      Serial.println("cooler: ON, fan2: ON, fan3: ON");
      all_LOW();
      cooler_value = !HIGH;
      fan23_value = !HIGH;
      relay_ONOFF();
//      cooler_timer.on(on_duration);
//      fan23_timer.on(on_duration);    
      break;

    case '5':
      Serial.print("key: "); Serial.println(key);
      Serial.println("fan1: ON, fan2: ON, fan3: ON");
      all_LOW();
      fan1_value = !HIGH;
      fan23_value = !HIGH;
      relay_ONOFF();
//      fan1_timer.on(on_duration);
//      fan23_timer.on(on_duration);
      break;

    case '6':
      Serial.print("key: "); Serial.println(key);
      Serial.println("dehumidifier: ON, fan2: ON, fan3: ON");
      all_LOW();
      dehumidifier_value = !HIGH;
      fan23_value = !HIGH;
      relay_ONOFF();
//      dehumidifier_timer.on(on_duration);
//      fan23_timer.on(on_duration);
      break;

    case '7':
      Serial.print("key: "); Serial.println(key);
      LED_value = Serial2.parseInt();
//      Serial.print(", value: "); Serial.println(value);
      digitalWrite(LED_pin, !LED_value);
      break;

    default:
      break;
  }
}