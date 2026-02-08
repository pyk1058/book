/*
  1. 수위가 
    (1) 낮으면: pump를 OFF하고 MQTT를 실행하지 않는다.
    (2) 정상이면: MQTT를 실행한다.
  
  1. MQTT broker로부터 신호를 받아 relay를 ON/OFF한다.
  2. topic: "pump"
  3. payload:
    (1) "ON": pump ON
    (2) "OFF": pump OFF

  2024.09.01
  박영기
*/

#include "myConfig.h"
#include <AimMqtt.h>
#include <AimTimer.h>

#define relay_pin D1 // GPIO5
#define water_level_pin D2

int water_level = LOW;

AimMqtt mqtt;
AimTimer water_level_timer(2);

void setup() {
  Serial.begin(115200);
  pinMode(relay_pin, OUTPUT);
  pinMode(water_level_pin, INPUT);
  mqtt.begin(myMqttSub, myMqttCallback);
}

void loop() {
  if (water_level_timer.isOn()) {
    water_level = digitalRead(water_level_pin);
  }

  if (water_level == LOW) {
    digitalWrite(relay_pin, LOW);
  }
  else {
    mqtt.run();
  }
}

void myMqttSub() {
  int QoS = 0;
  mqtt.sub("pump", QoS);
}

void myMqttCallback(const String& topic, const String& payload) {
  Serial.printf("topic: %s, payload: %s \n", topic.c_str(), payload.c_str());

  if (topic == "pump") {
    if (payload == "ON") {
      digitalWrite(relay_pin, HIGH);
    }

    if (payload == "OFF") {
      digitalWrite(relay_pin, LOW);
    }  
  }
}