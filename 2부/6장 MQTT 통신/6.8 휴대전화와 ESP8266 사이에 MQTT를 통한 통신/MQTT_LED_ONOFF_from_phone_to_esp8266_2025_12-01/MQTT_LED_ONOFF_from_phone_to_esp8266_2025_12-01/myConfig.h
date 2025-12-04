// 무선 공유기(AP,엑세스 포인트)의 이름과 비밀번호
#define AP_SSID1  "***" // 자신이 사용하는 네트워크의 SSID를 입력한다.
#define AP_PASS1  "***" // 자신이 사용하는 네트워크의 암호를 입력한다.

#define AP_SSID2  ""  // 없으면 ""
#define AP_PASS2  ""  // 없으면 ""

#define AP_SSID3  ""  // 없으면 ""
#define AP_PASS3  ""  // 없으면 ""

// MQTT 설정 
#define SERVER     "192.168.0.14"
//#define SERVER     "192.168.XXX.XXX"
#define PORT       1883

#define USER       ""  
#define PASS       ""

#define DEVICE     "ESP-{{mac}}" //{{auto}},{{mac}},{{user}}
