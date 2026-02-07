/*  양액이 부족하면 LED가 깜빡이고 버저가 운다.
 *  
 *  2019.10.26
 *  박영기
 */

int level_sensor = 2; // 수위센서의 신호선을 아두이노의 D2에 연결한다.
int buzzer = 7; // 버저의 신호선을 아두이노의 D7에 연결한다.
int LED = 11; // LED의 (+) 단자에 연결된 저항을 아두이노의 D11에 연결한다.

void setup()
{
  pinMode(level_sensor, INPUT); // level_sensor 입력으로 사용한다.
  pinMode(buzzer, OUTPUT); // buzzer는 출력으로 사용한다.
  pinMode(LED, OUTPUT); // LED는 출력으로 사용한다.
}

void loop()
{
  int level;
  
  level = digitalRead(level_sensor); // 수위센서의 값을 읽어서 level에 보관한다.

  if (level == LOW) // level의 값이 LOW이면 (수위가 낮으면)
  {
    digitalWrite(LED, HIGH); // LED에 HIGH를 공급한다. (LED를 켠다.)
    tone(buzzer, 2000); // buzzer를 2kHz 주파수로 울린다.
    delay(1000); // 그대로 1초를 유지한다.

    digitalWrite(LED, LOW); // LED에 LOW를 공급한다. (LED를 끈다.)
    noTone(buzzer); // buzzer를 끈다.
  }
  delay(1000); // 1초 동안 상태를 유지한다.
} // 되돌아간다.
