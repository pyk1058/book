/*
 * 블루투스를 통해, 
 * 온습도 측정값을 스마트폰 앱으로 보고, 스마트폰 앱으로 LED를 ON/OFF한다.
 * 
 * 파일명: monitoring_and_control_by_phone_2022-10-28.ino
 * 
 * 2022.10.28
 * 박영기
 */

#include <SoftwareSerial.h> // 소프트웨어로 시리얼 통신을 하는 라이브러리를 포함한다.
#include "DHT.h" // DHT.h 라이브러리를 포함한다.

#define DHTPIN 4 // 온습도 데이터를 4번 핀으로 읽는다.
#define DHTTYPE DHT22 // 온습도 센서는 DHT22(AM2302)를 사용한다.

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial BTSerial(2, 3); // 시리얼 통신을 하는 함수를 BTSerial이라 한다.
                               // 수신은 2번 핀, 발신은 3번 핀으로 한다.
float t, h; //온도를 저장할 변수를 t라 하고, 습도를 저장할 변수를 h라 한다.
int LED_pin = 13; // 출력할 핀의 이름은 LED_pin이라 하고, 13번 핀으로 지정한다.
int on_off; // 읽어들인 숫자를 보관하는 변수 (스마트폰 앱에서 보낸 숫자는 숫자로 받는다.)

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(9600); // 블루투스 통신을 시작한다. 스마트폰 사용.
  pinMode(LED_pin, OUTPUT); // LED_pin을 출력으로 지정한다.
  digitalWrite(LED_pin, LOW); // LED_pin에 0V를 보낸다. 처음에는 꺼져 있게 한다.  
  dht.begin();
}

void loop()
{
  if (BTSerial.available()) // 블루투스 모듈로 문자가 들어오면,
  {
    on_off = BTSerial.read(); // 블루투스로 받은 숫자를 on_off에 저장한다.

    switch(on_off) // on_off의 값에 따라 해당되는 case문을 실행한다.
    {
      case 1: // on_off에 저장된 숫자가 1인 경우
      {
        digitalWrite(LED_pin, HIGH); // LED_pin(13번 핀)에 5V를 보낸다.
        Serial.println(on_off);
        break; // 이 경우를 끝낸다.
      }

      case 0: // on_off에 저장된 숫자가 0인 경우
      {
        digitalWrite(LED_pin, LOW); // LED_pin에 0V를 보낸다.
        Serial.println(on_off);
        break; // 이 경우를 끝낸다.
      }

      default: // 위 경우가 아닌 경우
      {
        break; // 이 경우를 끝낸다.        
      }
    }
  }
  else
  {
    t = dht.readTemperature(); // 온도를 읽어 t에 저장한다.
    h = dht.readHumidity(); // 습도를 읽어 h에 저장한다.

    BTSerial.print("온도: ");BTSerial.print(t); BTSerial.println("C");
    BTSerial.print("습도: ");BTSerial.print(h); BTSerial.println("%");

    delay(2000);
  }
}
