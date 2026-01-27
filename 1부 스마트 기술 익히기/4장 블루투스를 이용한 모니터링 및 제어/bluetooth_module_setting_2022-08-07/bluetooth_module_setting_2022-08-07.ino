/*
 * 블루투스를 통해, 
 * 시리얼 모니터에 입력한 문자를 스마트폰에 표시하고,
 * 스마트폰에 입력한 문자를 시리얼 모니터에 표시한다.
 * 
 * 파일명: bluetooth_module_setting_2022-08-07.ino
 * 
 * 2022.08.07
 * 박영기
 */
 
#include <SoftwareSerial.h> // 소프트웨어로 시리얼 통신을 하는 라이브러리를 포함한다.

SoftwareSerial BTSerial(2, 3); // 시리얼 통신을 하는 함수를 BTSerial이라 한다.
                               // 수신은 2번 핀, 발신은 3번 핀으로 한다.

void setup() 
{
  Serial.begin(9600); // 시리얼 통신을 시작한다. 시리얼 모니터 사용.
  BTSerial.begin(9600); // 블루투스 통신을 시작한다. 휴대폰 사용.
}

void loop()
{
  if (BTSerial.available()) // 블루투스 모듈로 신호가 들어오면,
    Serial.write(BTSerial.read()); // 블루투스 모듈로 읽은 신호를 시리얼 모니터에 표시한다.

  if (Serial.available()) // 시리얼 모니터에 쓴 문자가 있으면,
    BTSerial.write(Serial.read()); // 블루투스 모듈이 신호를 보낸다. 휴대폰으로 볼 수 있다.
}
