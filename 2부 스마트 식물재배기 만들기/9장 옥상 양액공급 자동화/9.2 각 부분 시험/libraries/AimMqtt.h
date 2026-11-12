#ifndef AIM_MQTT_H
#define AIM_MQTT_H

#ifdef NO_CONFIG
#undef  USE_CONFIG
#else
#undef  USE_CONFIG
#define USE_CONFIG
#endif

#ifndef AP_SSID1 
#define AP_SSID1 "" 
#define AP_PASS1 "" 
#endif

#ifndef AP_SSID2 
#define AP_SSID2 "" 
#define AP_PASS2 "" 
#endif

#ifndef AP_SSID3 
#define AP_SSID3 "" 
#define AP_PASS3 "" 
#endif

#ifndef FINGER_PRINT
#define FINGER_PRINT ""
#endif

#ifndef PREFIX
#define PREFIX ""
#endif

#ifndef TOPIC_PREFIX_L
#define TOPIC_PREFIX_L 64
#endif

#ifndef LONG_TOPIC_L  
#define LONG_TOPIC_L  64
#endif

//#include "pubsubclient-master/src/PubSubClient.h"
#include <PubSubClient.h>

class AimMqtt : public PubSubClient {

public:
  AimMqtt();  
  AimMqtt(void (*funcSub)(),void (*funcCallback)(const String& topic,const String& payload));

  void ap();
  void ap(const char* ssid,const char* pass);
  void ap(const char* ssid[],const char* pass[],unsigned int sizeOfSsid);

  void ap(const String& ssid,const String& pass);
  void ap(const String ssid[],const String pass[],unsigned int sizeOfSsid);

  void begin();
  void begin(void (*funcSub)(),void (*funcCallback)(const String& topic,const String& payload));
  void begin(void (*funcSub)(),void (*funcCallback)(String topic,String payload));

  void setSub(void (*funcSub)());
  void setCallback(void (*funcCallback)(const String& topic,const String& payload));
  void setCallback(void (*funcCallback)(String topic,String payload));

  void mqttCallbackBase(char* topic, byte* payload, unsigned int length);

  void run();  

  int  verifyBool(const String& payload);
  bool toBool(const String& payload);

  void ifEmptySetConfig(); // void syncAutoConfig();
  void syncConfig();
  void configHelp();  
  bool setConfig(const String& key,const String& val);
  bool configSerial(const String& key,const String& val);
  void mqtt_connect();
  void dateTime(char* dt,size_t size);
  void wifi_connect();

  void setPrefix(String prefix);
  char* getPrefix();
  void setLongTopic(char buf[],int bufLen,const char* topic);

  void printSub(const char* topic,int QoS,bool success);
  void printPub(const char* topic,const char* payload,bool success);

  void setMsgPrintOn();
  void setMsgPrintOff();
  bool isMsgPrintOn();
  
  void longTopic_Sub_Print(const char* topic,int QoS);
  void longTopic_Pub_Print(const char* topic,const char* payload,bool retained);

  void sub(const char* topic,int QoS);
  void sub(const String& topic,int QoS);
  
  // char *
  void pub(const char* topic,  const char* payload,boolean retained);
  void pub(const String& topic,const char* payload,boolean retained);
  
  // const String&
  void pub(const char* topic,  const String&  payload,boolean retained);  
  void pub(const String& topic,const String&  payload,boolean retained);

  // int
  void pub(const char* topic,  int payload,boolean retained);
  void pub(const String& topic,int payload,boolean retained);

  // unsigned long
  void pub(const char* topic,  unsigned long payload,boolean retained);
  void pub(const String& topic,unsigned long payload,boolean retained);
  
  //float
  void pub(const char* topic,  float payload,boolean retained);
  void pub(const String& topic,float payload,boolean retained);

  // float,precision
  void pub(const char* topic,  float payload,int precision,boolean retained);
  void pub(const String& topic,float payload,int precision,boolean retained);

private: 
  void (*funcSub)();
  void (*funcCallback)(const String& topic,const String& payload);
  void (*funcCallbackCopy)(String topic,String payload);
  int cbType = 0; // callback type >> 0: ref(const String&), 1: copy(String)

  bool funcSubDefined = false;
  bool funcCallbackDefined = false;

  bool initFst = true;

  char prefix[TOPIC_PREFIX_L];

  bool msgPrintOn = true;

  int WifiBeginMode = 0 ; // 1 : AP_SSID1~3
                          // 2 : char* ap_ssid
                          // 3 : char* ap_ssid[]
                          // 12: String ap_ssid
                          // 13: String ap_ssid[]
  String ssidStr[3] = {"","",""};
  String passStr[3] = {"","",""};
  int ssidStrNo = 0;
  bool beginDone = false;
  bool configHelp_performed = false;

  void updateWifiMqttInfo();
  void expandMqttDevice();
  String _ssid[3];
  String _pass[3];
  int    _ssidNo = 1;
  String _mqttServer;
  int    _mqttPort;
  String _mqttUser;
  String _mqttPass;
  String _mqttDevice;
  String _fingerPrint;
  String _prefix;

  String _mqttPassEncrypted;

  const  char* name_AP_SSID1 = "AP_SSID1";
  const  char* name_AP_PASS1 = "AP_PASS1";
  const  char* name_AP_SSID2 = "AP_SSID2";
  const  char* name_AP_PASS2 = "AP_PASS2";
  const  char* name_AP_SSID3 = "AP_SSID3";
  const  char* name_AP_PASS3 = "AP_PASS3";

  const  char* name_SERVER = "SERVER";
  const  char* name_PORT   = "PORT";
  const  char* name_FP     = "FP";
  const  char* name_USER   = "USER";
  const  char* name_PASS   = "PASS";
  const  char* name_DEVICE = "DEVICE";
  const  char* name_PREFIX = "PREFIX";
};

#define CONFC(x) aimConfig.getString(x).c_str()
#define CONFS(x) aimConfig.getString(x)
#define CONFI(x) aimConfig.getInt(x)
#define CONFF(x) aimConfig.getFloat(x)

#define SYNCC(name,data) aimConfig.syncString(name,data,sizeof(data))
#define SYNCS(name,data) aimConfig.syncString(name,data)
#define SYNCI(name,data) aimConfig.syncInt(name,data)
#define SYNCF(name,data) aimConfig.syncFloat(name,data)
#define SETC(name,data) aimConfig.setString(name,data)
#define SETS(name,data) aimConfig.setString(name,data)
#define SETI(name,data) aimConfig.setInt(name,data)
#define SETF(name,data) aimConfig.setFloat(name,data)

#define EMPTYS(name,data) aimConfig.ifEmptySetString(name,data)
#define EMPTYI(name,data) aimConfig.ifEmptySetInt(name,data)
#define EMPTYF(name,data) aimConfig.ifEmptySetFloat(name,data)

#ifdef FUNC_WIFI_BEFORE
void FUNC_WIFI_BEFORE();
#endif

#ifdef FUNC_WIFI_OK
void FUNC_WIFI_OK();
#endif

#ifdef FUNC_MQTT_BEFORE
void FUNC_MQTT_BEFORE();
#endif

#ifdef FUNC_MQTT_OK
void FUNC_MQTT_OK();
#endif

#ifdef FUNC_MQTT_RETRY
void FUNC_MQTT_RETRY();
#endif

#ifndef DEVICE_SIZE
#define DEVICE_SIZE 24
#endif

#ifndef SERVER_SIZE
#define SERVER_SIZE 24
#endif

#ifndef USER_SIZE
#define USER_SIZE 24
#endif

#ifndef PASS_SIZE
#define PASS_SIZE 48
#endif

#ifndef DEVICE_SIZE
#define DEVICE_SIZE 32
#endif

#ifndef MQTT_OBJECT 
#define MQTT_OBJECT mqtt
#endif

#if defined(ESP8266)

#include <ESP8266WiFi.h>      // WiFi LIB
#include <ESP8266WiFiMulti.h> // for multiple SSIDs

#elif defined(ESP32)

#include <WiFi.h>      // WiFi LIB
#include <WiFiMulti.h> // for multiple SSIDs

#endif

// WiFi objects
WiFiClient espClient;       // WiFi object

#if defined(ESP8266)
ESP8266WiFiMulti wifiMulti; // multiple SSID object
#elif defined(ESP32)
WiFiMulti wifiMulti;        // multiple SSID object
#endif 

#if defined(USE_CONFIG)

#define AIM_CONFIG_FILE "AimMqtt__.json"
#include <AimConfig.h>  // JSON Config
AimConfig aimConfig(AIM_CONFIG_FILE);
//#define AIM_PASS_DUMMY
//#if defined(AIM_PASS_DUMMY)
//#include <AimPassDummy.h>
//#else
#include <AimPass.h>
//#endif

AimPass aimPass;

#endif // #if defined(USE_CONFIG)

AimMqtt  *ptrAimMqtt;

static void mqttCallback(char* topic, byte* payload, unsigned int length) {
  ptrAimMqtt->mqttCallbackBase(topic,payload,length); 
}

void AimMqtt::updateWifiMqttInfo() {

  if (WifiBeginMode > 0 ) {
    _ssid[0] = ssidStr[0]; 
    _ssid[1] = ssidStr[1]; 
    _ssid[2] = ssidStr[2]; 

    _pass[0] = passStr[0]; 
    _pass[1] = passStr[0]; 
    _pass[2] = passStr[0]; 
  } else {
    _ssid[0] = AP_SSID1; 
    _ssid[1] = AP_SSID2; 
    _ssid[2] = AP_SSID3; 

    _pass[0] = AP_PASS1; 
    _pass[1] = AP_PASS2; 
    _pass[2] = AP_PASS3; 
  }
  
  _ssid[0].trim();
  _ssid[1].trim();
  _ssid[2].trim();

   _ssidNo = 1;
  if(_ssid[1].length() > 0) _ssidNo = 2;
  if(_ssid[2].length() > 0) _ssidNo = 3; 

  _mqttServer  = SERVER;
  _mqttPort    = PORT;
  _mqttUser    = USER;
  _mqttPass    = PASS;
  _mqttDevice  = DEVICE; //{{auto}},{{mac}},{{user}}}
  _fingerPrint = FINGER_PRINT; 
  _prefix      = PREFIX;
}

void AimMqtt::expandMqttDevice() {
  // {{AUTO}} -> 6digits hexa
  if (_mqttDevice.indexOf("{{auto}}") >= 0 ||
      _mqttDevice.indexOf("{{AUTO}}") >= 0) {
    randomSeed(micros());      
    String id = String(random(0xffffff), HEX);
    _mqttDevice.replace("{{auto}}",id);
    _mqttDevice.replace("{{AUTO}}",id);
  }

  if (_mqttDevice.indexOf("{{mac}}") >= 0 ||
      _mqttDevice.indexOf("{{MAC}}") >= 0) {
    String id = WiFi.macAddress();
    _mqttDevice.replace("{{mac}}",id);
    _mqttDevice.replace("{{MAC}}",id);
  }
  
  if (_mqttDevice.indexOf("{{user}}") >= 0 ||
      _mqttDevice.indexOf("{{USER}}") >= 0) {
    _mqttDevice.replace("{{user}}",_mqttUser);
    _mqttDevice.replace("{{USER}}",_mqttUser);
  }
}

AimMqtt::AimMqtt(void (*INfuncSub)(),void (*INfuncCallback)(const String& topic,const String& payload)) {
  this->funcSub = INfuncSub;
  this->funcCallback = INfuncCallback;
  funcSubDefined = true;
  funcCallbackDefined = true;
  ptrAimMqtt = this;
  prefix[0] = '\0';
  //updateWifiMqttInfo();
}

AimMqtt::AimMqtt() {
  ptrAimMqtt = this;
  prefix[0] = '\0';
  //updateWifiMqttInfo();
}

void AimMqtt::ap() {
  WifiBeginMode = 1;
  ssidStr[0] = AP_SSID1;
  passStr[0] = AP_PASS1;
  ssidStr[1] = AP_SSID2;
  passStr[1] = AP_PASS2;
  ssidStr[2] = AP_SSID3;
  passStr[2] = AP_PASS3;

  begin();
  beginDone = true;
}

void AimMqtt::ap(const char* ssid,const char* pass) {
  WifiBeginMode = 2;
  ssidStr[0] = ssid;
  passStr[0] = pass;
  ssidStr[1] = "";
  passStr[1] = "";
  ssidStr[2] = "";
  passStr[2] = "";

  begin();
  beginDone = true;
}



void AimMqtt::ap(const char* ssid[],const char* pass[],unsigned int sizeOfSsid) {
  WifiBeginMode = 3;
  char* charForLen;
  ssidStrNo = sizeOfSsid / sizeof(charForLen);
  if (ssidStrNo > 3) {
    ssidStrNo = 3;
  }

  for(int i = 0; i < ssidStrNo; i++ ) {
    ssidStr[i] = ssid[i];
    passStr[i] = pass[i];
  }  

  begin();
  beginDone = true;
}

void AimMqtt::ap(const String& ssid,const String& pass) {
  WifiBeginMode = 12;
  ssidStrNo = 1;
  ssidStr[0] = ssid;
  passStr[0] = pass;

  begin();
  beginDone = true;
}

void AimMqtt::ap(const String ssid[],const String pass[],unsigned int sizeOfSsid) {
  WifiBeginMode = 13;
  String strForLen;
  ssidStrNo = sizeOfSsid / sizeof(strForLen);
  if (ssidStrNo > 3) {
    ssidStrNo = 3;
  }

  for(int i = 0; i < ssidStrNo; i++ ) {
    ssidStr[i] = ssid[i];
    passStr[i] = pass[i];
  }

  begin();
  beginDone = true;  
}

void AimMqtt::begin() {
  
  /*
  if (!configHelp_performed) {
    configHelp();
  }
  */

  updateWifiMqttInfo();

  #if defined(USE_CONFIG)  
  ifEmptySetConfig();
  syncConfig();
  #endif // #if defined(USE_CONFIG)
  
  expandMqttDevice();
  setPrefix(_prefix); 

  if (initFst) {
    initFst = false;
    wifi_connect();
    return;
  }
}

void AimMqtt::begin(void (*INfuncSub)(),void (*INfuncCallback)(const String& topic,const String& payload)) {
  this->funcSub = INfuncSub;
  this->funcCallback = INfuncCallback;
  funcSubDefined = true;
  funcCallbackDefined = true;

  if (!beginDone) { 
    beginDone = true;
    begin();
  }
}

void AimMqtt::begin(void (*INfuncSub)(),void (*INfuncCallback)(String topic,String payload)) {
  this->funcSub = INfuncSub;
  this->funcCallbackCopy = INfuncCallback;
  cbType = 1; // copy
  funcSubDefined = true;
  funcCallbackDefined = true;

  if (!beginDone) { 
    beginDone = true;
    begin();
  }
}

void AimMqtt::setSub(void (*INfuncSub)()) {
  this->funcSub = INfuncSub;
  funcSubDefined = true;
}

void AimMqtt::setCallback(void (*INfuncCallback)(const String& topic,const String& payload)) {
  this->funcCallback = INfuncCallback;
  funcCallbackDefined = true;
}

void AimMqtt::setCallback(void (*INfuncCallback)(String topic,String payload)) {
  this->funcCallbackCopy = INfuncCallback;
  cbType = 1; // copy
  funcCallbackDefined = true;
}

void AimMqtt::run() {
  if (initFst) {
    initFst = false;
    wifi_connect();
    return;
  }

  if (!connected()) { 
    mqtt_connect();
  }
  PubSubClient::loop();  
}

int AimMqtt::verifyBool(const String& INpayload) {
  String payload = INpayload;
  payload.toLowerCase();
  if (payload == "0"   || payload == "off" || payload == "false" || payload == "no" || payload == "n" ) {
    return false; 
  }
  else if (payload == "1" || payload == "on" || payload == "true" || payload == "yes" || payload == "y") {
    return true;  
  } 
  else {
    return -1;
  }
}  

bool AimMqtt::toBool(const String& INpayload) {
  return verifyBool(INpayload);
}  

void AimMqtt::dateTime(char* dt,size_t size) {
  char date_time[] = __DATE__ "_" __TIME__;

  for (size_t i = 0; i < strlen(date_time); i++) {
    if(date_time[i] == ' ') {
      date_time[i] = '_';
    }
  }
  strlcpy(dt,date_time,size);
}



void AimMqtt::ifEmptySetConfig() {

  #if defined(USE_CONFIG)

  char dt[32];
  dateTime(dt,sizeof(dt));

  //EMPTYS(dt,dt);
  if (strcmp(CONFC(dt),dt) == 0) {
    #if defined(SHOW_PASS)
    Serial.printf("JSON found and matched.\n"); 
    #endif   
  }
  else {
    #if defined(SHOW_PASS)
    Serial.printf("New JSON will be created.\n");
    #endif
    aimConfig.erase();
    EMPTYS(dt,dt);
  }
  
  EMPTYS(name_AP_SSID1,_ssid[0].c_str());

  String pass1 = aimPass.encHW(_pass[0].c_str());

  EMPTYS(name_AP_PASS1,pass1);

  EMPTYS(name_AP_SSID2,_ssid[1].c_str());

  String pass2 = aimPass.encHW(_pass[1].c_str());

  EMPTYS(name_AP_PASS2,pass2);

  EMPTYS(name_AP_SSID3,_ssid[2].c_str());

  String pass3 = aimPass.encHW(_pass[2].c_str());

  EMPTYS(name_AP_PASS3,pass3);

  EMPTYS(name_SERVER,  _mqttServer);
  EMPTYI(name_PORT,    _mqttPort);
  EMPTYS(name_FP,      _fingerPrint);

  EMPTYS(name_USER,    _mqttUser);

  String pass = aimPass.encHW(_mqttPass);
  _mqttPassEncrypted =  pass; 
  EMPTYS(name_PASS,    pass);

  EMPTYS(name_DEVICE,  _mqttDevice);
  EMPTYS(name_PREFIX,  _prefix);

  //mqtt.setPrefix(CONFC(name_PREFIX)); 
  //setPrefix(CONFC(name_PREFIX)); 

  #endif // #if defined(USE_CONFIG)  
}

void AimMqtt::syncConfig() {
  
  #if defined(USE_CONFIG)
  
  String tStr;

  SYNCS(name_AP_SSID1,_ssid[0]);  
  SYNCS(name_AP_PASS1,tStr);
  _pass[0] = aimPass.decHW(tStr);
  
  SYNCS(name_AP_SSID2,_ssid[1]);
  SYNCS(name_AP_PASS2,tStr);
  _pass[1] = aimPass.decHW(tStr);

  SYNCS(name_AP_SSID3,_ssid[2]);
  SYNCS(name_AP_PASS3,tStr);
  _pass[2] = aimPass.decHW(tStr);

  SYNCS(name_SERVER,  _mqttServer);
  SYNCI(name_PORT,    _mqttPort);
  SYNCS(name_FP,      _fingerPrint);

  SYNCS(name_USER,    _mqttUser);

  SYNCS(name_PASS,    tStr);
  String _mqttPass = aimPass.decHW(tStr);

  SYNCS(name_DEVICE,  _mqttDevice);
  SYNCS(name_PREFIX,  _prefix);

  #endif // #if defined(USE_CONFIG) 

}

void AimMqtt::wifi_connect() {

  // Delay while Serial ready
  delay(100);
  
  // WiFi station mode
  WiFi.mode(WIFI_STA);

  #if defined(SHOW_PASS)
  Serial.printf("\nConnecting to [%s][%s],[%s][%s],[%s][%s]\n",
                 _ssid[0].c_str(),_pass[0].c_str(),_ssid[1].c_str(),_pass[1].c_str(),_ssid[2].c_str(),_pass[2].c_str()); 
  #else
  Serial.printf("\nConnecting to [%s],[%s],[%s]\n",
                 _ssid[0].c_str(),_ssid[1].c_str(),_ssid[2].c_str());    
  #endif   

  #ifdef FUNC_WIFI_BEFORE
  FUNC_WIFI_BEFORE();
  #endif
  
  wifiMulti.addAP(_ssid[0].c_str(),_pass[0].c_str());
  wifiMulti.addAP(_ssid[1].c_str(),_pass[1].c_str());
  wifiMulti.addAP(_ssid[2].c_str(),_pass[2].c_str());
  
  // Repeat if not connected
  int runCnt = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (++runCnt > 60) {
      runCnt = 0;
      Serial.println();
    }
  }

  Serial.print(F("\nConnected to ["));
  Serial.print(WiFi.SSID());
  Serial.print(F("],IP="));
  Serial.println(WiFi.localIP());
  Serial.println(F("-----"));
  #ifdef FUNC_WIFI_OK
  FUNC_WIFI_OK();
  #endif  
}

void AimMqtt::mqtt_connect() {
  
  // Repeat if not connected
  while (!connected()) {
    
    #ifdef FUNC_MQTT_BEFORE
    FUNC_MQTT_BEFORE();
    #endif
    Serial.println(F("Attempting MQTT connection..."));

    Serial.printf((char*)F("device=[%s]\n"),_mqttDevice.c_str());

    Serial.printf("SERVER=[%s],PORT=%d\n",_mqttServer.c_str(),_mqttPort);
    setServer(_mqttServer.c_str(),_mqttPort);

    PubSubClient::setCallback(mqttCallback);
    setClient (espClient);

    connect(_mqttDevice.c_str(),_mqttUser.c_str(),_mqttPass.c_str());
    
    // If not connected
    if (connected()) {
      Serial.println(F("MQTT connected\n-----"));
      // The function defined in user pogram
      // called for every connection to MQTT
      //myMqttSub();
      if (funcSubDefined) {
        funcSub();
      }
      #ifdef FUNC_MQTT_OK
      FUNC_MQTT_OK();
      #endif
    } 
    else {
      Serial.print(F("failed, rc="));
      Serial.print(state());
      Serial.println(F(" try again in 5 seconds"));
      #ifdef FUNC_MQTT_RETRY
      FUNC_MQTT_RETRY();
      #endif
      // Delay 5 sec
      delay(5000);
    }    
  } 
}

void AimMqtt::mqttCallbackBase(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic); 
  
  String payloadStr((char*)0);  
  // Reserve max string length 
  // to prevent heap memory fragmentation
  payloadStr.reserve(length);

  // Variable type 'byte' can be casted to 'char'
  // A point variable can be used as array
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  //if (MQTT_OBJECT.isMsgPrintOn()) {
  if (isMsgPrintOn()) {
    Serial.printf("RCV> [%s],[%s]\n",topicStr.c_str(),payloadStr.c_str());
  }

  String p = getPrefix();
  if (p.length() > 0 ) {
    if (topicStr.indexOf(p) == 0 ) {
      topicStr = topicStr.substring(p.length());
    }
  }

  if (funcCallbackDefined) {
    if (cbType == 0) {
      funcCallback(topicStr, payloadStr);
    }
    else if (cbType == 1) {
      funcCallbackCopy(topicStr, payloadStr);
    }
  }
}

void AimMqtt::configHelp() { 
  /*
  if (configHelp_performed) {
    return;
  } 
  */

  // Delay while Serial ready
  delay(500);  

  #if defined(USE_CONFIG)

  //Serial.println(F("-----"));   
  Serial.println(F("Input > 'key val' for new config"));
  Serial.println(F("  Valid keys are as follows."));
  Serial.println(F("    prefix")); 
  Serial.println(F("    ssid1, pass1, ssid2, pass2, ssid3, pass3 "));
  Serial.println(F("    server,port,  fp "));
  Serial.println(F("    user,  pass,  device\n")); 
  Serial.println(F("Input > 'disp' to display config list"));
  Serial.println(F("Input > 'erase' to erase ConfigJson file for AimMqtt"));  
  Serial.println(F("Input > 'restart' to restart"));
  Serial.println(F("Input > 'help' for help"));   
  Serial.println(F("-----"));

  #else 

  //Serial.println(F("-----"));   
  Serial.println(F("\"mqtt.configHelp();\" can not be perfomrd."));
  Serial.println(F("  if you want it to be performed,"));
  Serial.println(F("  select \"Flash Size:(fs:1MB...)\" or more 'fs' in the \"Tool\" menu,"));
  Serial.println(F("  and define in the source code as follows:"));
  Serial.println(F("    #define USE_CONFIG")); 
  Serial.println(F("    #include <AimMqtt.h>;"));
  Serial.println(F("-----"));

  #endif // #if defined(USE_CONFIG)

  //configHelp_performed = true;
}

// setLongTopic
void AimMqtt::setLongTopic(char buf[],int bufLen,const char* topic) {
  if (strlen(prefix) == 0) {
    strlcpy(buf,topic,bufLen);
  }
  else if (topic[0] == '/' && topic[1] == '/') {
    strlcpy(buf,topic+2,bufLen);
  }
  else {
    snprintf(buf,bufLen,"%s%s",prefix,topic);
  }
  String bufStr = buf;
  bufStr.replace("{{user}}",USER);
  bufStr.replace("{{USER}}",USER);
  snprintf(buf,bufLen,"%s",bufStr.c_str());
}

void AimMqtt::setPrefix(String INprefix) {

  String prefixStr = INprefix;
  prefixStr.trim();
  String user;
  String server;
  
  #if defined(USE_CONFIG)
  user   = CONFS(name_USER);
  server = CONFS(name_SERVER);
  #else
  user   = USER;
  server = SERVER;

  #endif // #if defined(USE_CONFIG)
  
  if (prefixStr.indexOf("{{user}}") >= 0) {
    prefixStr.replace("{{user}}",user);
  }
  else if (prefixStr.indexOf("{{USER}}") >= 0) {
    prefixStr.replace("{{USER}}",user);
  }

  strlcpy(prefix,prefixStr.c_str(),sizeof(prefix));
  int len = strlen(prefix);

  if (len == 0) {
    prefix[0] = '\0';
  }
  else if (prefix[len-1] != '/' && prefix[len-1] != '.') {
    if (server == "io.adafruit.com") {
      strlcat(prefix,".",sizeof(prefix));
    }
    else {
      strlcat(prefix,"/",sizeof(prefix));
    }
  }

}

char* AimMqtt::getPrefix() {  
  return prefix;
}

bool AimMqtt::setConfig(const String& INkey,const String& INval) {
  String key = INkey;
  String val = INval;
  key.trim();
  val.trim();

  #if defined(USE_CONFIG)

  bool validConfigKey = true;
  String newKey = key;
  newKey.toLowerCase();

  if (newKey == "prefix") {
    SETS(name_PREFIX,val);
    //mqtt.setPrefix(CONFC(name_PREFIX)); 
    setPrefix(CONFC(name_PREFIX));
    Serial.printf((char*)F("New PREFIX > [%s]\n"),CONFC(name_PREFIX));
  }
  else if (newKey == "enc") {
    String encrypted = aimPass.encHW(val);
    String decrypted = aimPass.decHW(encrypted);
    Serial.printf((char*)F("enc > [%s],[%s],[%s]\n"),val.c_str(),encrypted.c_str(),decrypted.c_str());
  }
  else if (newKey == "dec") {
    String decrypted = aimPass.decHW(val);
    String encrypted = aimPass.encHW(decrypted);
    Serial.printf((char*)F("dec > [%s],[%s],[%s]\n"),val.c_str(),decrypted.c_str(),encrypted.c_str());
  }
  else if (newKey == "ssid1") {
    SETS(name_AP_SSID1,val);
    Serial.printf((char*)F("New AP_SSID1 > [%s]\n"),CONFC(name_AP_SSID1));
  }
  else if (newKey == "pass1") {
    SETS(name_AP_PASS1,aimPass.encHW(val));
    Serial.printf((char*)F("New AP_PASS1 > [%s]\n"),
                  aimPass.decHW(CONFC(name_AP_PASS1)).c_str());
  }
  else if (newKey == "ssid2") {
    SETS(name_AP_SSID2,val);
    Serial.printf((char*)F("New AP_SSID2 > [%s]\n"),CONFC(name_AP_SSID2));
  }
  else if (newKey == "pass2") {
    SETS(name_AP_PASS2,aimPass.encHW(val));
    Serial.printf((char*)F("New AP_PASS2 > [%s]\n"),
                  aimPass.decHW(CONFC(name_AP_PASS2)).c_str());
  }
  else if (newKey == "ssid3") {
    SETS(name_AP_SSID3,val);
    Serial.printf((char*)F("New AP_SSID3 > [%s]\n"),CONFC(name_AP_SSID3));
  }
  else if (newKey == "pass3") {
    SETS(name_AP_PASS3,aimPass.encHW(val));
    Serial.printf((char*)F("New AP_PASS3 > [%s]\n"),
                  aimPass.decHW(CONFC(name_AP_PASS3)).c_str());
  }
  else if (newKey == "server") {
    SETS(name_SERVER,val);
    Serial.printf((char*)F("New SERVER > [%s]\n"),CONFC(name_SERVER));
  }
  else if (newKey == "port") {
    SETI(name_PORT,val.toInt());
    Serial.printf((char*)F("New PORT > [%d]\n"),CONFC(name_PORT));
  }
  else if (newKey == "fp") {
    SETS(name_FP,val);
    Serial.printf((char*)F("New FP > [%s]\n"),CONFC(name_FP));
  }
  else if (newKey == "user") {
    SETS(name_USER,val);
    Serial.printf((char*)F("New USER > [%s]\n"),CONFC(name_USER));
  }
  else if (newKey == "pass") {
    SETS(name_PASS,aimPass.encHW(val));
    Serial.printf((char*)F("New PASS > [%s]\n"),
                  aimPass.decHW(CONFC(name_PASS)).c_str());
  }
  else if (newKey == "device") {
    SETS(name_DEVICE,val);
    Serial.printf((char*)F("New DEVICE > [%s]\n"),CONFC(name_DEVICE));
  }
  else if(newKey == "help") {
    //AimConfigHelp(); 
    configHelp();    
  }
  else if(newKey == "disp") {

    char dt[32];
    dateTime(dt,sizeof(dt));
    String v1;
    String v2;
  
    Serial.println(F("-----"));    
    Serial.printf((char*)F("Config list in file[%s]..\n"),AIM_CONFIG_FILE);

    #if defined(SHOW_TIME)
    Serial.printf((char*)F("__TIME_DATE__=[%s]\n"),CONFC(dt));
    #endif

    v1 = CONFS(name_AP_SSID1);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("AP_SSID1=[%s]\n"),v1.c_str());

    v1 = CONFS(name_AP_PASS1);
    if(v1 == "null") {
      v1 = "";
      v2 = "";
    }
    else {
      v2 = aimPass.decHW(v1);;
    }    
    #if defined(SHOW_PASS)
    Serial.printf((char*)F("AP_PASS1=[%s][%s]\n"),v2.c_str(),v1.c_str());
    #else
    Serial.printf((char*)F("AP_PASS1=[%s]\n"),(v1 == "")?"":"*****");
    #endif

    v1 = CONFS(name_AP_SSID2);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("AP_SSID2=[%s]\n"),v1.c_str());

    v1 = CONFS(name_AP_PASS2);
    if(v1 == "null") {
      v1 = "";
      v2 = "";
    }
    else {
      v2 = aimPass.decHW(v1);;
    }    
    #if defined(SHOW_PASS)
    Serial.printf((char*)F("AP_PASS2=[%s][%s]\n"),v2.c_str(),v1.c_str());
    #else
    Serial.printf((char*)F("AP_PASS2=[%s]\n"),(v1 == "")?"":"*****");
    #endif

    v1 = CONFS(name_AP_SSID3);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("AP_SSID3=[%s]\n"),v1.c_str());

    v1 = CONFS(name_AP_PASS3);
    if(v1 == "null") {
      v1 = "";
      v2 = "";
    }
    else {
      v2 = aimPass.decHW(v1);;
    }    
    #if defined(SHOW_PASS)
    Serial.printf((char*)F("AP_PASS3=[%s][%s]\n"),v2.c_str(),v1.c_str());
    #else
    Serial.printf((char*)F("AP_PASS3=[%s]\n"),(v1 == "")?"":"*****");
    #endif

    v1 = CONFS(name_SERVER);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("SERVER=[%s]\n"),v1.c_str());

    v1 = String(CONFI(name_PORT));
    if(v1 == "0") {
      v1 = "";
    }    
    Serial.printf((char*)F("PORT=[%s]\n"),v1.c_str());

    v1 = CONFS(name_FP);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("FP=[%s]\n"),v1.c_str());

    v1 = CONFS(name_USER);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("USER=[%s]\n"),v1.c_str());

    v1 = CONFS(name_PASS);
    if(v1 == "null") {
      v1 = "";
      v2 = "";
    }
    else {
      v2 = aimPass.decHW(v1);;
    }    
    #if defined(SHOW_PASS)
    Serial.printf((char*)F("PASS=[%s][%s]\n"),v2.c_str(),v1.c_str());
    #else
    Serial.printf((char*)F("PASS=[%s]\n"),(v1 == "")?"":"*****");
    #endif

    v1 = CONFS(name_DEVICE);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("DEVICE=[%s]\n"),v1.c_str());

    v1 = CONFS(name_PREFIX);
    if(v1 == "null") {
      v1 = "";
    }    
    Serial.printf((char*)F("PREFIX=[%s]\n"),v1.c_str());
    Serial.println(F("-----"));
  }
  else if(newKey == "erase") {
    aimConfig.erase();
    Serial.println(F("ConfigJSON file for AimMqtt erased."));
  }  
  else if(newKey == "restart") {
    Serial.println(F("Device will be restart in 3 seconds"));
    delay(2000);
    ESP.restart();
  } 
  else {
    validConfigKey = false;
  }
  
  if (newKey != "erase" && newKey != "restart") {
    syncConfig(); 
  }
  
  return validConfigKey;

  #else 

  //Serial.println(F("-----"));   
  Serial.println(F("\"mqtt.configSerial();\" can not be perfomrd."));
  Serial.println(F("  if you want it to be performed,"));
  Serial.println(F("  select \"Flash Size:(fs:1MB...)\" or more 'fs' in the \"Tool\" menu,"));
  Serial.println(F("  and define in the source code as follows:"));
  Serial.println(F("    #define USE_CONFIG")); 
  Serial.println(F("    #include <AimMqtt.h>;"));
  Serial.println(F("-----"));
  #endif // #if defined(USE_CONFIG) 
  return true;
}

bool AimMqtt::configSerial(const String& key,const String& val) {
  return setConfig(key,val);
}

// printSub
void AimMqtt::printSub(const char* topic,int QoS,bool success) {
  if (msgPrintOn == false) {
    return;
  }

  static const char* successStr[2] = {",failed",""};
  Serial.printf("SUB: [%s],QoS=%d%s\n",topic,QoS,successStr[success]);
}

// printPub
void AimMqtt::printPub(const char* topic,const char* payload,bool success) {
  if (msgPrintOn == false) {
    return;
  }

  static const char* successStr[2] = {",failed",""};
  Serial.printf("PUB: [%s],[%s]%s\n",topic,payload,successStr[success]);
}

void AimMqtt::setMsgPrintOn() {
  msgPrintOn = true;
}

void AimMqtt::setMsgPrintOff() {
  msgPrintOn = false;
}

bool AimMqtt::isMsgPrintOn() {
  return msgPrintOn;
}

// longTopic_sub_Print
void AimMqtt::longTopic_Sub_Print(const char* topic,int QoS) {
  char longTopic[LONG_TOPIC_L];
  setLongTopic(longTopic,sizeof(longTopic),topic);
  bool success = subscribe(longTopic,QoS);
  printSub(longTopic,QoS,success);
}

//sub
void AimMqtt::sub(const char* topic,int QoS) {
  longTopic_Sub_Print(topic,QoS);
}

void AimMqtt::sub(const String& topic,int QoS) {
  longTopic_Sub_Print(topic.c_str(),QoS);
}

// longTopic_Pub_Print
void AimMqtt::longTopic_Pub_Print(const char* topic,const char* payload,bool retained) {
  char longTopic[LONG_TOPIC_L];
  setLongTopic(longTopic,sizeof(longTopic),topic);
  bool success = publish(longTopic,payload,retained);
  printPub(longTopic,payload,success);
}

// const char*
void AimMqtt::pub(const char* topic,const char* payload,boolean retained = false) {
  longTopic_Pub_Print(topic,payload,retained);
}

void AimMqtt::pub(const String& topic,const char* payload,boolean retained = false) {
  longTopic_Pub_Print(topic.c_str(),payload,retained);
}

// const String&
void AimMqtt::pub(const char* topic,const String&  payload,boolean retained = false) {
  longTopic_Pub_Print(topic,payload.c_str(),retained);
}

void AimMqtt::pub(const String& topic,const String&  payload,boolean retained = false) {
  longTopic_Pub_Print(topic.c_str(),payload.c_str(),retained);
}

// int 
void AimMqtt::pub(const char* topic,int payload,boolean retained = false) {
  longTopic_Pub_Print(topic,String(payload).c_str(),retained);
}
void AimMqtt::pub(const String& topic,int payload,boolean retained = false) {
  longTopic_Pub_Print(topic.c_str(),String(payload).c_str(),retained);
}

// unsigned long 
void AimMqtt::pub(const char* topic,unsigned long payload,boolean retained = false) {
  longTopic_Pub_Print(topic,String(payload).c_str(),retained);
}
void AimMqtt::pub(const String& topic,unsigned long payload,boolean retained = false) {
  longTopic_Pub_Print(topic.c_str(),String(payload).c_str(),retained);
}

// float 
void AimMqtt::pub(const char* topic,float payload,boolean retained = false) {
  longTopic_Pub_Print(topic,String(payload).c_str(),retained);
}
void AimMqtt::pub(const String& topic,float payload,boolean retained = false) {
  longTopic_Pub_Print(topic.c_str(),String(payload).c_str(),retained);
}

// float, precision
void AimMqtt::pub(const char* topic,float payload,int precision, boolean retained = false) {
  longTopic_Pub_Print(topic,String(payload,precision).c_str(),retained);
}
void AimMqtt::pub(const String& topic,float payload,int precision, boolean retained = false) {
  longTopic_Pub_Print(topic.c_str(),String(payload,precision).c_str(),retained);
}

void AimMqttSetup() {

}
void AimMqttLoop() {

}

#endif // #ifndef AIM_MQTT_H