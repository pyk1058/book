/*
 * AP의 목록을 시리얼 모니터에 보여주는 프로그램.
 * 파일명: AP_listing_2023-01-14.ino
 * 
 * 2023.01.14
 * 박영기
 */
 
#include <ESP8266WiFi.h>

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA); // ESP8266을 스테이션 모드로 사용한다.
  WiFi.disconnect(); // 연결되어 있던 와이파이를 끊는다.

  Serial.println("Ready to scan...");
}

void loop()
{
  Serial.println("Scanning start...");

  int n = WiFi.scanNetworks(); // 스캔에 성공한 갯수를 n에 저장한다.
  Serial.println("Scanning completed...");

  if (n == 0)
  {
    Serial.println("AP is NOT found...");
  }
  else
  {
    Serial.print(n);
    Serial.println(" APs are found...");

    for (int i = 0; i < n; i++)
    {
      Serial.print(i + 1);
      Serial.print(" : ");
      Serial.println(WiFi.SSID(i)); // 와이파이의 SSID를 출력한다.
    }
  }
  delay(5000);
}
