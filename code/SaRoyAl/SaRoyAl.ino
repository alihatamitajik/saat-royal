/*
   -- SaRoyAl --   
*/

#include <ESP8266WiFi.h>
#include <string.h>
#include <RTClib.h>
#include <iostream>
#include <string>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Variables
// State Variables for Operate
uint8_t state = 0;
bool isOperating = false;
long lastCheckpoint;
long alarmCheck = millis();
int32_t threshold = 1000;
// Mic
bool isTriggered = false;
uint8_t batteryPercent;
int8_t temprature;

// Time
DateTime noww;

// To Be Used As Local Counters
long startState;
uint32_t i_loc;
uint32_t j;
uint32_t k;
uint8_t var1;

bool shouldLog = false;
int8_t log_temprature;
uint8_t log_battery;

// Settings
struct {
  // Alaram
  uint8_t alarmH;
  uint8_t alarmM;
  // Booleans
  uint8_t isAlarm;
  uint8_t isShowClock;
  uint8_t isShowDate;
  uint8_t isShowBattery;
  uint8_t isTemprature;
  uint8_t isShowRainbow;
  uint8_t isShowPackman;
  // Modes
  uint8_t modeClock;  // 0: FF, 1: Swing, 2: Simple
  uint8_t ffMode;
  uint32_t color60;
  uint32_t color24;
  uint32_t color12;
  uint32_t color12N;
  uint8_t  brightness;
  uint8_t isJalali;
} Prefrences ={0,0,0,1,1,1,1,1,1,2,0,16711680,65280,255,25500, 50, 0};


AsyncWebServer server(80);


const char* ssid = "ESP-SaRoyAl";          // Your WiFi SSID
const char* password = "LaYorSa-PSE";  // Your WiFi Password

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

/**
 * This function is to prepare battery percentage and temprature for
 * Status request. Prepration is time taking and cause problem for sending
 * requests so they should be prepared beforehand.
 */
void handleGetReady (AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "WAIT 2000ms ...");
  shouldLog= true;
}

/**
 * Send status of the clock to the app on 192.168.4.1/status
 */
void handleGetSatus (AsyncWebServerRequest *request) {
  updateTime();
  char form[] = "DDD, DD MMM YYYY hh:mm:ss";
  String stat;
  stat += "{\"Current\":\"" +     String(noww.toString(form)) + "\",";
  stat += "\"temprature\":" +     String(log_temprature) + ",";
  stat += "\"battery\":" +        String(log_battery) + "}";
  request->send(200, "application/json", stat);
}

/**
 * send settings to switches on 192.168.4.1/setting
 */
void handleGetSetting (AsyncWebServerRequest *request) {
  String stat;
  stat += "{\"isShowClock\":" +   String(Prefrences.isShowClock) + ",";
  stat += "\"isShowDate\":" +     String(Prefrences.isShowDate) + ",";
  stat += "\"isShowBattery\":" +  String(Prefrences.isShowBattery) + ",";
  stat += "\"isTemprature\":" +   String(Prefrences.isTemprature) + ",";
  stat += "\"isShowRainbow\":" +  String(Prefrences.isShowRainbow) + ",";
  stat += "\"isShowPackman\":" +  String(Prefrences.isShowPackman) + ",";
  stat += "\"isJalali\":" +       String(Prefrences.isJalali) + "}";
  request->send(200, "application/json", stat);
}

/*
 * Alarm settings on 192.168.4.1/alarm
 */
void handleGetAlarm (AsyncWebServerRequest *request) {
  String stat;
  stat += "{\"alarmH\":" +        String(Prefrences.alarmH) + ",";
  stat += "\"alarmM\":" +         String(Prefrences.alarmM) + ",";
  stat += "\"isAlarm\":" +        String(Prefrences.isAlarm) + "}";
  request->send(200, "application/json", stat);
}

void handleGetColors (AsyncWebServerRequest *request) {
  String stat;
  stat += "{\"modeClock\":" +     String(Prefrences.modeClock) + ",";
  stat += "\"ffMode\":" +         String(Prefrences.ffMode) + ",";
  stat += "\"color60\":" +        String(Prefrences.color60) + ",";
  stat += "\"color24\":" +        String(Prefrences.color24) + ",";
  stat += "\"color12\":" +        String(Prefrences.color12) + ",";
  stat += "\"color12N\":" +       String(Prefrences.color12N) + ",";
  stat += "\"brightness\":" +     String(Prefrences.brightness) + "}";
  request->send(200, "application/json", stat);
}

void handlePostData(AsyncWebServerRequest *request){
  String message;
  bool found= false;
  
  if (request->hasParam("alarmH", true)) {
      message = request->getParam("alarmH", true)->value();
      Prefrences.alarmH = message.toInt();
      found = true;
  } 
  
  if (request->hasParam("alarmM", true)) {
      message = request->getParam("alarmM", true)->value();
      Prefrences.alarmM = message.toInt();
      found = true;
  }
  
  if (request->hasParam("isAlarm", true)) {
      message = request->getParam("isAlarm", true)->value();
      Prefrences.isAlarm = message.toInt();
      found = true;
  }
  
  if (request->hasParam("isShowClock", true)) {
      message = request->getParam("isShowClock", true)->value();
      Prefrences.isShowClock = message.toInt();
      found = true;
  }
  
  if (request->hasParam("isShowDate", true)) {
      message = request->getParam("isShowDate", true)->value();
      Prefrences.isShowDate = message.toInt();
      found = true;
  }
  
  if (request->hasParam("isShowBattery", true)) {
      message = request->getParam("isShowBattery", true)->value();
      Prefrences.isShowBattery = message.toInt();
      found = true;
  }
  
  if (request->hasParam("isShowRainbow", true)) {
      message = request->getParam("isShowRainbow", true)->value();
      Prefrences.isShowRainbow = message.toInt();
      found = true;
  }
  
  if (request->hasParam("isShowPackman", true)) {
      message = request->getParam("isShowPackman", true)->value();
      Prefrences.isShowPackman = message.toInt();
      found = true;
  }
  
  if (request->hasParam("modeClock", true)) {
      message = request->getParam("modeClock", true)->value();
      Prefrences.modeClock = message.toInt();
      found = true;
  }
  
  if (request->hasParam("color60", true)) {
      message = request->getParam("color60", true)->value();
      Prefrences.color60 = message.toInt();
      found = true;
  }
  
  if (request->hasParam("color24", true)) {
      message = request->getParam("color24", true)->value();
      Prefrences.color24 = message.toInt();
      found = true;
  }

  if (request->hasParam("color12N", true)) {
      message = request->getParam("color12N", true)->value();
      Prefrences.color12N = message.toInt();
      found = true;
  }

  if (request->hasParam("ffMode", true)) {
      message = request->getParam("ffMode", true)->value();
      Prefrences.ffMode = message.toInt();
      found = true;
  }
  
  if (request->hasParam("color12", true)) {
      message = request->getParam("color12", true)->value();
      Prefrences.color12 = message.toInt();
      found = true;
  }

  if (request->hasParam("brightness", true)) {
      message = request->getParam("brightness", true)->value();
      Prefrences.brightness = message.toInt();
      setBrightnessStrip();
      found = true;
  }

  if (request->hasParam("isJalali", true)) {
      message = request->getParam("isJalali", true)->value();
      Prefrences.isJalali = message.toInt();
      found = true;
  }

  if (request->hasParam("isTemprature", true)) {
      message = request->getParam("isTemprature", true)->value();
      Prefrences.isTemprature = message.toInt();
      found = true;
  }
  
  if (found)
    request->send(200, "text/plain", "OK");
  else
    request->send(400, "text/plain", "No Parameter Found");
}

void handleAdjustClock(AsyncWebServerRequest *request) {
  bool valid = true;
  uint8_t YYYY,MM,DD, hh,mm,ss; 
  if (valid &= request->hasParam("YYYY", true)) 
    YYYY = request->getParam("YYYY", true)->value().toInt();
  if (valid &= request->hasParam("MM", true)) 
    MM = request->getParam("MM", true)->value().toInt();
  if (valid &= request->hasParam("DD", true)) 
    DD = request->getParam("DD", true)->value().toInt();
  if (valid &= request->hasParam("hh", true)) 
    hh = request->getParam("hh", true)->value().toInt();
  if (valid &= request->hasParam("mm", true)) 
    mm = request->getParam("mm", true)->value().toInt();
  if (valid &= request->hasParam("ss", true)) 
    ss = request->getParam("ss", true)->value().toInt();

  if (valid) {
    request->send(200, "text/plain", "OK");
    adjustClock(YYYY, MM, DD, hh, mm, ss);
  } else
    request->send(400, "text/plain", "Some Parameters Missing");
}

void initServer() {
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP); 
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Sa@RoyAl Controller");
  });

  server.on("/ready", HTTP_GET, handleGetReady);
  server.on("/status", HTTP_GET, handleGetSatus);
  server.on("/setting", HTTP_GET, handleGetSetting);
  server.on("/alarm", HTTP_GET, handleGetAlarm);
  server.on("/colors", HTTP_GET, handleGetColors);
  
  // parameters are in the name of the Prefrences structure
  server.on("/post", HTTP_POST, handlePostData);

  server.on("/operate", [](AsyncWebServerRequest *request){
        isTriggered = true;
        request->send(200, "text/plain", "OK");
  });

  server.on("/adjust", HTTP_POST, handleAdjustClock);
  server.onNotFound(notFound);

  server.begin();
}

void setup() 
{
  Serial.begin(115200);
#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  initRTC();
  initTEMP();
  initStrip();
  turnOff();
  initServer();
}

void loop() 
{ 
  operate();
}
