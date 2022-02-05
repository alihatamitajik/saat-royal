/*
   -- SaRoyAl --   
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "SaRoyAl"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,19,0,1,0,30,1,13,13,5,
  131,1,1,2,20,4,1,2,31,83,
  116,97,116,117,115,0,131,0,22,2,
  24,4,2,2,31,77,111,100,117,108,
  97,116,105,111,110,0,131,0,47,2,
  14,4,3,2,31,67,111,108,111,114,
  115,0,2,1,10,10,43,5,3,2,
  26,31,31,82,97,110,100,111,109,0,
  109,97,110,117,97,108,0,6,0,21,
  21,20,20,3,2,26,6,0,21,46,
  20,20,3,2,26,6,0,21,71,20,
  20,3,2,26,66,130,19,22,23,18,
  1,2,26,2,1,5,49,20,6,1,
  2,26,31,31,79,78,0,79,70,70,
  0,7,52,28,49,13,6,1,2,26,
  2,7,52,42,49,13,6,1,2,26,
  2,2,1,4,26,15,6,2,2,26,
  31,31,79,78,0,79,70,70,0,129,
  0,32,26,11,5,2,31,68,97,116,
  101,0,3,131,4,13,22,8,2,2,
  26,129,0,32,15,23,4,2,31,67,
  108,111,99,107,32,77,111,100,101,0,
  2,1,4,37,15,6,2,2,26,31,
  31,79,78,0,79,70,70,0,129,0,
  32,38,25,4,2,31,84,101,109,112,
  101,114,97,116,117,114,101,32,0,2,
  1,4,48,15,6,2,2,26,31,31,
  79,78,0,79,70,70,0,129,0,32,
  49,25,4,2,31,66,97,116,116,101,
  114,121,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t isRandom; // =1 if switch ON and =0 if OFF 
  uint8_t outer_r; // =0..255 Red color value 
  uint8_t outer_g; // =0..255 Green color value 
  uint8_t outer_b; // =0..255 Blue color value 
  uint8_t mid_r; // =0..255 Red color value 
  uint8_t mid_g; // =0..255 Green color value 
  uint8_t mid_b; // =0..255 Blue color value 
  uint8_t inner_r; // =0..255 Red color value 
  uint8_t inner_g; // =0..255 Green color value 
  uint8_t inner_b; // =0..255 Blue color value 
  uint8_t isAlarm; // =1 if switch ON and =0 if OFF 
  int16_t alarm_h;  // 32767.. +32767 
  int16_t alarm_m;  // 32767.. +32767 
  uint8_t isDate; // =1 if switch ON and =0 if OFF 
  uint8_t clockMode; // =0 if select position A, =1 if position B, =2 if position C, ... 
  uint8_t isTemp; // =1 if switch ON and =0 if OFF 
  uint8_t isBattery; // =1 if switch ON and =0 if OFF 

    // output variables
  int8_t battery_level; // =0..100 level position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

// Variables
bool isTriggered = false;
bool isOperating = false;

void setup() 
{
  RemoteXY_Init ();
  initTEMP();
  initStrip();
  Serial.begin(115200);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  pinMode(D3,INPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,INPUT);
  pinMode(A0,INPUT);

}

// Variables to check timer
long lastCheckpoint;
uint8_t threshold;

// State Variables for Operate
uint8_t state;

void operate(){
  // TODO
  isOperating = false;
}

void loop() 
{ 
  RemoteXY_Handler ();


  // if we are not trigered and clock is not showing
  // capture clap
  if(!isTriggered && !isOperating){
    checkTriger();
  } else if (isTriggered){
    // else if we are trigered call the operation once
    // operation should 
    testStrip();
    isTriggered = false;
    isOperating = true;
    // Show data once, next calls should be done by timer
    lastCheckpoint = millis();
    threshold = 200;
    state = 0;
  } else if (isOperating) {
    if (millis() - lastCheckpoint >  threshold){
      operate();
    }
  }
}
