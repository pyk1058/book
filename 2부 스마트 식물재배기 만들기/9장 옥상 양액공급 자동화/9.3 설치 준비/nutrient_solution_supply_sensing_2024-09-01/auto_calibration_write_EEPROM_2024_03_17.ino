
void auto_calibration() {
  switch(step) {
    case '1':
      select_sensor = LOW;
      v_empty = calibration(select_sensor);
      Serial.printf("Calibration: v_empty = %d \n", v_empty);
      break;

    case '2': 
      select_sensor = LOW;
      i_select = 2;
      put_to_EEPROM(select_sensor, i_select);
      break;

    case '3':
      select_sensor = HIGH;
      i_select = 3;
      put_to_EEPROM(select_sensor, i_select);
      break;

    default:
      break;
  }
}

int calibration(int select) {
  int level;
  int v = 0;

  digitalWrite(select_pin, select); // MUX relay가 min 또는 max을 선택한다.
  delay(500);

  for (int i = 1; i <= 5; i++) {
    digitalWrite(Vout_pin, HIGH);
    level = analogRead(Vin_pin);
    digitalWrite(Vout_pin, LOW);
    v = v + level;
  }

  v = (int)v/5;
  return v;
}

void put_to_EEPROM(int select_sensor, int i) {
  EEPROM.begin(EEPROM_SIZE); // ESP8266에서는 begin() 명령을 사용해야 시작된다. 

  v_cal = calibration(select_sensor);
  v_ref[i] = int ((v_empty + v_cal) / 2);
  Serial.printf("v_ref[%d] = %d \n", i, v_ref[i]);

  EEPROM.put(v_address[i], v_ref[i]);
  EEPROM.commit(); // ESP8266에서는 commit() 명령을 사용해야 EEPROM에 실제로 저장된다.
  EEPROM.get(v_address[i], v_get_EEPROM);
  EEPROM.end(); // ESP8266에서는 end() 명령을 사용해야 끝난다. 
}