/*
  1. 휴대폰에서 설정 시각을 받으면 분으로 환산하여 EEPROM에 저장한다.
  2. 휴대폰에서 값을 보내지 않으면 EEPROM에 저장된 값을 읽어서 설정 시각으로 사용한다.
*/

void phone_time_receive() { // sensing_timer.isOn(sensing_period) 안에서 call한다.
  client = server.available(); // Check if a client has connected
  if (!client) { 
    EEPROM.begin(EEPROM_SIZE); // ESP8266에서는 begin() 명령을 사용해야 시작된다.
    EEPROM.get(address_LED_on, t_LED_set_on);
    EEPROM.get(address_LED_off, t_LED_set_off);
    EEPROM.end();
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {   
    delay(1);
  }
  Serial.println("client available");

  // Read the first line of the request
  request = client.readStringUntil('\r');
  client.flush();
  Serial.print("request: "); Serial.println(request);
  int index_time_receive = request.indexOf("key=");
  String S_time_receive = request.substring(index_time_receive + 4);
  int key_time_receive = S_time_receive.toInt();

  if (key_time_receive == 1) {
    response_connect();
    return;
  }

  if (key_time_receive == 2) {
    response_IP();
    return;
  }

  if (key_time_receive == 3) {
    response_time();
    return;
  }

  if (key_time_receive == 4) {
    THW_to_phone();
    return;
  }
}

void response_connect() {
	client.print("HTTP/1.1 200 OK");
	client.print("Content-Type: text/html \r\n\r\n");
  client.print("C: Phone is connected to ESP8266.");
  Serial.println("C: Phone is connected to ESP8266.");
}

void response_IP() {
	client.print("HTTP/1.1 200 OK");
	client.print("Content-Type: text/htm \r\n\r\n");
  client.print("I: New IP is saved.");
  Serial.println("I: New IP is saved.");
}

void response_time() {
  Index[0] = request.indexOf("ON_hour="); // "number="가 있는지 찾는다. 못 찾으면 -1을 반환한다.
  Index[1] = request.indexOf("ON_min="); // "number="가 있는지 찾는다. 못 찾으면 -1을 반환한다. 
  Index[2] = request.indexOf("OFF_hour="); // "number="가 있는지 찾는다. 못 찾으면 -1을 반환한다.
  Index[3] = request.indexOf("OFF_min="); // "number="가 있는지 찾는다. 못 찾으면 -1을 반환한다. 


  for (int i = 0; i <= 3; i++) {
    String_t[i] = request.substring(Index[i] + space[i]); // "number="가 6글자이므로 7번째부터의 문자열을 복사한다.
    Time[i] = String_t[i].toInt();
  }

  t_LED_set_on = 60*Time[0] + Time[1];
  t_LED_set_off = 60*Time[2] + Time[3];

  EEPROM.begin(EEPROM_SIZE); // ESP8266에서는 begin() 명령을 사용해야 시작된다.
  EEPROM.put(address_LED_on, t_LED_set_on);
  EEPROM.put(address_LED_off, t_LED_set_off);
  Serial.printf("EEPROM.put; t_LED_on: %d \n", t_LED_set_on);
  Serial.printf("EEPROM.put; t_LED_off: %d \n", t_LED_set_off); 
  EEPROM.commit(); // ESP8266에서는 commit() 명령을 사용해야 EEPROM에 실제로 저장된다.
  EEPROM.end();

  first_LED = 1;

	client.print("HTTP/1.1 200 OK");
	client.print("Content-Type: text/htm \r\n\r\n");
  client.print("S: Time is saved.");
  Serial.println("S: Time is saved.");
}

void THW_to_phone() {
	client.print("HTTP/1.1 200 OK");
	client.print("Content-Type: text/html r\n\r\n");
	client.printf("T %4.1f H %4.1f W %d", T, H, W);
//  Serial.printf("T %4.1f H %4.1f W %d \n", T, H, W);
}

