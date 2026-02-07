#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <DHT11.h>
int pin = 2;
DHT11 dht11(pin); 

char *ssid = "***";
char *password = "***";

void setup()
{
  Serial.begin(115200);

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
  delay(2000); //delay for reread

  HTTPClient httpClient;

  String host = "http://api.thingspeak.com";
  String url = "/update?key=";
  String key = "***";
  String field1 = "&field1=";
  String field2 = "&field2=";

  httpClient.begin(host + url + key + field1 + temp + field2 + humi);
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
