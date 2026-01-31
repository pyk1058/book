/*
 * 온습도 데이터를 스마트폰으로 보내기
 * temperature_humidity_sending_to_phone_2023-02-19.ino
 * 
 * 2023.02.19
 * 박영기
 */

#include <ESP8266WiFi.h> // ESP8266WiFi 라이브러리
#include <ESP8266WebServer.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22

char *ssid = "Younggi"; // SSID
char *password = "gondwana1"; // password

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80); // 웹 서버의 포트를 80에 맞춘다.

void setup()
{
  Serial.begin(9600); // 시리얼 통신의 baud rate를 9600에 맞춘다.
  Serial.println("ESP8266과 DHT22로 온습도 읽기");
  
  dht.begin(); // 온습도 센서의 동작을 시작한다.
  
  Serial.print("Connecting to \'");
  Serial.print(ssid);
  Serial.println("\'");

  WiFi.begin(ssid, password); // WiFi 연결을 시작한다.

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("."); // 연결되기 전까지 .을 표시한다.
  }
  Serial.println();
  Serial.println("WiFi connected."); // WiFi에 연결되었음을 표시한다.
  
  Serial.print("Server IP address: "); 
  Serial.println(WiFi.localIP()); // 연결된 IP를 표시한다.

  server.on("/", handleRoot); // 스마트폰에서 접속하면 루트 디렉토리의 'handleRoot' 함수를 실행한다.

  server.begin(); // 서버를 시작한다.
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient(); // 클라이언트의 요청이 있는 경우 클라이언트와의 연결을 생성하고 요청을 처리한다.
}

void handleRoot()
{
  delay(2000); // 2초 대기.
    
  float h = dht.readHumidity(); // 습도를 읽어 변수 h에 저장한다.
  float t = dht.readTemperature(); // 온도를 읽어 변수 t에 저장한다.

  server.sendHeader("Refresh", "10"); // 10초마다 보낸다.

  if (isnan(h) || isnan(t))
  {
    server.send(200, "text/plain", "Error in reading sensor."); // 데이터를 읽지 못하면 스마트폰에 메세지를 보낸다.
    return;
  }

  String message = ""; // 메세지 만들기
  message += t;
  message += ",";
  message += h;
  
  server.send(200, "text/plain", message); // 스마트폰으로 메세지를 보낸다.
}
