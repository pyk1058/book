/* 앱인벤트로 작성한 스마트폰 앱으로 LOLIN 또는 NodeMCU의 LED 점멸
 * WiFi 이용
 * control_WiFi_2023-03-08.ino
 * 2023.03.08
 * 박영기
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char *ssid = "***";
char *password = "***";

int LED_pin = 4; //D2

ESP8266WebServer server(80);

void handleON()
{
  server.send(200, "text/plain", "Turned ON");
  Serial.println("LED was turned ON");
  digitalWrite(LED_pin, HIGH);
}

void handleOFF()
{
  server.send(200, "text/plain", "Turned OFF");
  Serial.println("LED was turned OFF");
  digitalWrite(LED_pin, LOW);
}

void setup(void)
{
  Serial.begin(115200);

  Serial.print("Connecting to \'");
  Serial.print(ssid);
  Serial.println("\'");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");

  Serial.print("Server IP address: ");
  Serial.println(WiFi.localIP());

// server.on(URL 주소, 처리함수): on 함수는 클라이언트의 요청에 따라 주소와 처리할 함수를 연결
  server.on("/on", handleON); 
  server.on("/off", handleOFF);

  server.begin(); // 웹 서버 시작
  Serial.println("HTTP server started");

  pinMode(LED_pin, OUTPUT);
  digitalWrite(LED_pin, LOW);
}

void loop()
{
  server.handleClient(); // 클라이언트와 연결을 생성하고, 요청을 처리
}
