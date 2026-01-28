/*
 * TDH22로 온습도를 측정하여 시리얼 모니터에 표시한다.
 * 
 * 파일명: temperature_humidity_reading_2022-09-18.ino
 * 
 * 2022.09.18
 * 박영기
 */

#include "DHT.h" // DHT 라이브러리를 포함한다.
 
#define DHTPIN 4 // 온습도 데이터를 4번 핀으로 읽는다.
#define DHTTYPE DHT22 // 온습도 센서는 DHT22(AM2302)를 사용한다.
 
DHT dht(DHTPIN, DHTTYPE);

float t, h;

void setup()
{
  Serial.begin(9600); 
  dht.begin();
  Serial.println("DHT22 TEST"); // 시리얼 모니터에 "DHT22 TEST"를 출력하고 다음 줄로 간다.
}

void loop()
{
  t = dht.readTemperature(); // 온도를 읽어 t에 저장한다.
  h = dht.readHumidity(); // 습도를 읽어 h에 저장한다.

  Serial.print("Humidity: "); // 시리얼 모니터에 "Humidity: "를 출력한다.
  Serial.print(h); // 시리얼 모니터에 h값을 출력한다.
  Serial.print(" %\t"); // 시리얼 모니터에 tab만큼 띄운다.
  Serial.print("Temperature: "); // 시리얼 모니터에 "Temperature: "를 출력한다.
  Serial.print(t); // 시리얼 모니터에 t값을 출력한다.
  Serial.println(" *C"); // 시리얼 모니터에 " *C"를 출력하고 다음 줄로 간다.

  delay(2000);
}
