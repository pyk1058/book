/*
 * 블루투스를 통해, 
 * 온습도 측정값을 스마트폰으로 보낸다.
 * 
 * 파일명: temperature_humidity_to_smart_phone_2022-09-23.ino
 * 
 * 2022.09.23
 * 박영기
 */

#include <SoftwareSerial.h> // 소프트웨어로 시리얼 통신을 하는 라이브러리를 포함한다.
#include "DHT.h" // DHT.h 라이브러리를 포함한다.

#define DHTPIN 4 // 온습도 데이터를 4번 핀으로 읽는다.
#define DHTTYPE DHT22 // 온습도 센서는 DHT22(AM2302)를 사용한다.

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial BTSerial(2, 3); // 시리얼 통신을 하는 함수를 BTSerial이라 한다.
                               // 수신은 2번 핀, 발신은 3번 핀으로 한다.
float t, h;

void setup() 
{
  Serial.begin(9600); // 시리얼 통신을 시작한다. 시리얼 모니터 사용.
  BTSerial.begin(9600); // 블루투스 통신을 시작한다. 휴대폰 사용.
  dht.begin();
}

void loop()
{
  t = dht.readTemperature(); // 온도를 읽어 t에 저장한다.
  h = dht.readHumidity(); // 습도를 읽어 h에 저장한다.

  BTSerial.print("  온도: "); BTSerial.print(t); BTSerial.print(" *C");
  BTSerial.print("  습도: "); BTSerial.print(h); BTSerial.println(" %");

  delay(2000);
}
