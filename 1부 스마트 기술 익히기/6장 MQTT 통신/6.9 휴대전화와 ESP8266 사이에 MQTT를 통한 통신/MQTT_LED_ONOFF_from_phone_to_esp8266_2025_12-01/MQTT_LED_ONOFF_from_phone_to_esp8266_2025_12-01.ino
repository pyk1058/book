/*
  1. MQTT broker로부터 신호를 받아 LED를 ON/OFF한다.
  2. ESP8266용 프로그램이다.
  3. topic: "LED"
  4. payload:
    (1) "ON": LED ON
    (2) "OFF": LED OFF

  2025.12.01
  박영기
*/

#include "myConfig.h"
#include <AimMqtt.h> // 
#include <AimTimer.h>

#define LED_pin D4 // GPIO2

AimMqtt mqtt;
AimTimer check_timer(1);

void setup() {
  Serial.begin(115200);
  pinMode(LED_pin, OUTPUT);
  mqtt.begin(myMqttSub, myMqttCallback);
}

void loop() {
  if (check_timer.isOn()) {   
  }  
  mqtt.run();
}

void myMqttSub() {
  int QoS = 0;
  mqtt.sub("LED", QoS);
}

void myMqttCallback(const String& topic, const String& payload) {
  Serial.printf("topic: %s, payload: %s \n", topic.c_str(), payload.c_str());

  if (topic == "LED") {
    if (payload == "ON") {
      Serial.printf("Received LED: ON \n");
      digitalWrite(LED_pin, !HIGH); // ESP8266은 HIGH일 때 LED가 OFF되고, LOW일 때 ON된다.
    }

    if (payload == "OFF") {
      digitalWrite(LED_pin, !LOW); // ESP8266은 HIGH일 때 LED가 OFF되고, LOW일 때 ON된다.
      Serial.printf("Received LED: OFF \n");
    }  
  }
}