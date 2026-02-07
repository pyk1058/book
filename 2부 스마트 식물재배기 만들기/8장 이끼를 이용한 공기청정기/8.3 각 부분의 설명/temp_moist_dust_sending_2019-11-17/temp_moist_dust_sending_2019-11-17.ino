/*
 * 온습도, 먼지농도를 ThingSpeak로 보내기
 * 
 * 2019.11.17
 * 박영기
 * 
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <DHT11.h>
int pin = 2;
DHT11 dht11(pin); 

char *ssid = "***";
char *password = "***";

int ledPower = 0;
int dustPin = A0;
int delayTime = 280;
int delayTime2 = 40;
float offTime = 9680;
float vo, dustVal;

void setup()
{
  Serial.begin(115200);

  pinMode(ledPower, OUTPUT);
  pinMode(dustPin, INPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print("Connecting to \'");
  Serial.print(ssid);
  Serial.println("\'");

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
}

void loop() 
{
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }

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

  HTTPClient httpClient;

  String host = "http://api.thingspeak.com";
  String url = "/update?key=";
  String key = "***";
  String field1 = "&field1=";
  String field2 = "&field2=";
  String field3 = "&field3=";

  httpClient.begin(host + url + key + field1 + temp + field2 + humi + field3 + dustDensity);
  int httpCode = httpClient.GET();

  if (httpCode > 0)
  {
    Serial.printf("[HTTP] Temperature & Humidity data was logged...\n");
  }
  else
  {
    Serial.printf("[HTTP] Connection failed : %s\n", httpClient.errorToString(httpCode).c_str());
  }

  httpClient.end();

  delay(20000);
}