/*
 * 블루투스를 통해 스마트폰으로 아두이노 보드의 LED를 ON/OFF한다.
 * 
 * 파일명: bluetooth_LED_ON_OFF_2022-08-25.ino
 * 
 * 2022.08.25
 * 박영기
 */
 
#include <SoftwareSerial.h> // 소프트웨어로 시리얼 통신을 하는 라이브러리를 포함한다.

int LED_pin = 13; // 출력할 핀의 이름은 LED_pin이라 하고, 13번 핀으로 지정한다.
char on_off; // 읽어들인 문자를 보관하는 변수

SoftwareSerial BTSerial(2, 3); // 시리얼 통신을 하는 함수를 BTSerial이라 한다.
                               // 수신은 2번 핀, 발신은 3번 핀으로 한다.

void setup() 
{
  BTSerial.begin(9600); // 블루투스 통신을 시작한다. 스마트폰 사용.
  pinMode(LED_pin, OUTPUT); // LED_pin을 출력으로 지정한다.
  digitalWrite(LED_pin, LOW); // LED_pin에 0V를 보낸다. 처음에는 꺼져 있게 한다.
}

void loop()
{
  if (BTSerial.available()) // 블루투스 모듈로 문자가 들어오면,
  {
    on_off = BTSerial.read(); // 블루투스로 받은 문자를 on_off에 저장한다.

    switch(on_off) // on_off의 값에 따라 해당되는 case문을 실행한다.
    {
      case '1': // on_off에 저장된 문자가 '1'인 경우
      {
        digitalWrite(LED_pin, HIGH); // LED_pin(13번 핀)에 5V를 보낸다.
        break; // 이 경우를 끝낸다.
      }

      case '0': // on_off에 저장된 문자가 '0'인 경우
      {
        digitalWrite(LED_pin, LOW); // LED_pin에 0V를 보낸다.
        break; // 이 경우를 끝낸다.
      }

      default: // 위 경우가 아닌 경우
      {
        BTSerial.println("입력을 잘못했습니다.");
        BTSerial.println("1: LED ON, 0: LED OFF");
        break; // 이 경우를 끝낸다.        
      }
    }
  }
}
