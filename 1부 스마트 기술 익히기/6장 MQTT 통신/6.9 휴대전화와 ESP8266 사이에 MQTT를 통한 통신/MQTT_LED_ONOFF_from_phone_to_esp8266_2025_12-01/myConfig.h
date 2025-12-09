// 무선 공유기(AP,엑세스 포인트)의 이름과 비밀번호
#define AP_SSID1  "***" // 자신의 환경에 맞게 입력한다.
#define AP_PASS1  "***" // 자신의 환경에 맞게 입력한다.

#define AP_SSID2  ""  // 없으면 ""
#define AP_PASS2  ""  // 없으면 ""

#define AP_SSID3  ""  // 없으면 ""
#define AP_PASS3  ""  // 없으면 ""

// MQTT 설정 
#define SERVER     "192.168.0.**" // RPi의 IP 주소를 넣는다.
//#define SERVER     "192.168.XXX.XXX"
#define PORT       1883

#define USER       ""  
#define PASS       ""
#define DEVICE     "ESP-{{mac}}" //{{auto}},{{mac}},{{user}}
