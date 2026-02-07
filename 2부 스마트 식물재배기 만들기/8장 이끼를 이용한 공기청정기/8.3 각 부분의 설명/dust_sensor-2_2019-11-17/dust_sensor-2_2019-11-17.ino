/*
 * 미세먼지센서 시험
 * 
 * 2019.11.17
 * 박영기
 */

int ledPower = 0;
int dustPin = A0;

int delayTime = 280;
int delayTime2 = 40;
float offTime = 9680;

float vo, dustVal;

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
  pinMode(dustPin, INPUT);

  Serial.println("Air Quality");
}

void loop() 
{
  delayMicroseconds(delayTime);

  vo = analogRead(dustPin);
  dustVal = 0.1*vo;

  delayMicroseconds(delayTime2);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(offTime);

//  float dustDensity = (0.172*(dustVal*(5/1024.0))-0.0999)*1000;
  float dustDensity = (dustVal-0.8)/0.005;

  Serial.print("dustVal = ");
  Serial.print(dustVal);
  Serial.print(", dustDensity = ");
  Serial.print(dustDensity);
  Serial.print("ug/m3 ");
  Serial.println();

  delay(5000);
}