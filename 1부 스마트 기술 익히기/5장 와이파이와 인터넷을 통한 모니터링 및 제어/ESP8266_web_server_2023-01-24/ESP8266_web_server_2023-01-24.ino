/*
 * ESP8266을 웹 서버로 사용
 * 파일명: ESP8266_web_server_2023-01-24.ino
 * 2023.01.24
 * 박영기
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char *ssid = "AP의 이름";
char *password = "AP의 암호";

ESP8266WebServer server(80); // 웹 서버 객체

void handleRoot() // 클라이언트 요청에 대한 처리함수
{
  server.send(200, "text/plain", "Hello from esp8266!");
}

void handleTest() // 클라이언트 요청에 대한 처리함수
{
  server.send(200, "text/plain", "Test directory");
}

void handleNotFound() // 클라이언트 요청에 대한 처리함수
{
  server.send(404, "text/plain", "File Not Found");
}

void setup(void)
{
  Serial.begin(115200);
  Serial.print("Connecting to \'");
  Serial.print(ssid);
  Serial.println("\'");

  WiFi.begin(ssid, password); // AP에 접속

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("Server IP address: ");
  Serial.println(WiFi.localIP()); // 서버(ESP8266)에 할당된 주소

  server.on("/", handleRoot); // /에 처리함수 지정
  server.on("/test", handleTest); // /test에 처리함수 지정
  server.onNotFound(handleNotFound); // 존재하지 않는 주소일 때 처리함수 지정

  server.begin(); // 서버 시작
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient(); // 클라이언트 요청 처리
}
