/*
 * AP에 연결하기
 * 파일명: connect_to_AP_2023-01-22.ino
 * 
 * 2023.01.22
 * 박영기
 */

#include <ESP8266WiFi.h>

char *ssid = "Younggi"; 
char *password = "gondwana1";

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA); // 보드를 스테이션 모드로 사용한다.
  WiFi.disconnect(); // 이전의 연결을 끊는다.

  Serial.print("Connecting to \'");
  Serial.print(ssid);
  Serial.println("\'");

  WiFi.begin(ssid, password); // AP에 접속한다.

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.println("WiFi connected...");
  Serial.println();
  
  Serial.println("* AP information");
  Serial.print("SSID : ");
  Serial.println(WiFi.SSID());
  Serial.print("Subnet mask : ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP : ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS IP : ");
  Serial.println(WiFi.dnsIP());
  Serial.println();
  
  Serial.println("* Station information");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}

void loop()
{
}
