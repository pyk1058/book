/* 시리얼 모니터로 아두이노 보드의 LED를 ON/OFF한다.
 *  
 *  2022.07.20
 *  박영기
 */

int LED_pin = 13; // 출력할 핀의 이름은 LED_pin이라 하고, 13번 핀으로 지정한다.
char on_off; // 읽어들인 문자를 보관하는 변수

void setup()
{
  Serial.begin(9600); // 시리얼 통신을 시작한다.
  pinMode(LED_pin, OUTPUT); // LED_pin을 출력으로 지정한다.
  digitalWrite(LED_pin, LOW); // LED_pin에 0V를 보낸다.
  Serial.println("1: LED ON, 0: LED OFF"); // 시리얼 모니터에 "1: LED ON, 0: LED OFF"를 출력한다.
}

void loop()
{
  if (Serial.available()) // 시리얼 모니터로 읽은 문자가 있으면
  {
    on_off = Serial.read(); // 시리얼 모니터로 읽은 문자를 on_off에 저장한다.
    
    switch(on_off) // on_off의 값에 따라 해당되는 case문을 실행한다.
    {
 
      case '1': // on_off에 저장된 문자가 '1'인 경우
        digitalWrite(LED_pin, HIGH); // LED_pin에 5V를 보낸다.
        Serial.println("LED ON"); // 시리얼 모니터에 "LED ON"을 출력한다.
        break; // 이 경우를 끝낸다.

      case '0': // on_off에 저장된 문자가 '0'인 경우
        digitalWrite(LED_pin, LOW); // LED_pin에 0V를 보낸다.
        Serial.println("LED OFF"); // 시리얼 모니터에 "LED OFF"을 출력한다.
        break; // 이 경우를 끝낸다.

      default: // 위 경우가 아닌 경우
        Serial.println("잘못 입력했습니다."); // 시리얼 모니터에 "잘못 입력했습니다."를 출력한다.
        break; // 이 경우를 끝낸다.
    }
  }
}
