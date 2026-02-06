/*
 * 장충동작은도서관 제어부
 * 2021.07.31
 * 박영기
 * 
 * 수위가 부족하면 10분마다 알람을 울리고 모든 동작을 멈춘다.
 * 온도가 섭씨 30도 이상이면 양액을 30분마다 10분 동안 순환시킨다.
 * 온도가 섭씨 20도와 30도 사이이면 양액을 60분마다 10분 동안 순환시킨다.
 * 온도가 섭씨 20도 이하이면 히터를 1분마다 1분 동안 킨다.
 */

#include "DHT.h"
 
#define DHTPIN 2        // SDA 핀의 설정
#define DHTTYPE DHT22   // DHT22 (AM2302) 센서종류 설정
 
DHT dht(DHTPIN, DHTTYPE);

int level_pin = 3;
int buzzer_pin = 5;
boolean level;
int period, duration;
int water_pump_relay_pin = 7;
int heater_relay_pin = 9;

void setup()
{
  Serial.begin(9600); 
  dht.begin();
  pinMode(level_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(water_pump_relay_pin, OUTPUT);
  pinMode(heater_relay_pin, OUTPUT);

  digitalWrite(water_pump_relay_pin, LOW);
  digitalWrite(heater_relay_pin, LOW);
}

void loop()
{
  float t = dht.readTemperature();
  
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.println(" *C");
  
  level = digitalRead(level_pin);
  Serial.print("level = ");
  Serial.println(level);

  if (level == LOW)
  {
    alarm();
    for (int i = 0; i < 10; i++)
    {
      delay(60000);
    }
  }
  else if (t >= 30)
  {
    run_water_pump(30, 10);
  }
  else if (t < 20)
  {
    digitalWrite(heater_relay_pin, HIGH);
    delay(1000);
    digitalWrite(heater_relay_pin, LOW);
    delay(1000);
  }
  else
  {
    run_water_pump(60, 10);    
  }
}

void alarm() 
{
  for (int i = 1; i <= 10; i++)
  {
    tone(buzzer_pin, 1000);
    delay(1000);
    noTone(buzzer_pin);
    delay(1000);
  }
}

void run_water_pump(int period, int duration)
{
  digitalWrite(water_pump_relay_pin, LOW);
  for (int i = 0; i < period; i++)
  {
    delay(60000);
  }
  digitalWrite(water_pump_relay_pin, HIGH);
  for (int i = 0; i < duration; i++)
  {
    delay(60000);
  }
}
