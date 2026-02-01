/*
 * 온도와 습도를 ThingSpeak로 보낸다.
 * esp8266_ThingSpeak_2023-03-03.ino
 * 2023.03.03
 * 박영기
 */

#include <DHT.h>
#include <ESP8266WiFi.h>
 
String apiKey = "***"; // ThinSpeak의 Write API key를 넣는다.
const char* ssid = "***"; // WiFi AP 이름
const char* password = "***"; // WiFi 암호
 
const char* server = "api.thingspeak.com";
#define DHTPIN 4 // D2 pin on Nodemcu // 온습도 센서의 데이터 선을 NodeMCU의 D2에 연결
 
DHT dht(DHTPIN, DHT22, 11);
WiFiClient client;

void setup() 
{                
  Serial.begin(115200);
  delay(10);
  dht.begin();
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
}
 
 
void loop() 
{   
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  if (client.connect(server,80)) 
  {
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String((int)t);
           postStr +="&field2=";
           postStr += String((int)h);
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(h);
     Serial.println("% send to Thingspeak");    
  }
  client.stop();
   
  Serial.println("Waiting...");  
  delay(10000); // 10초 동안 기다린다.
}
