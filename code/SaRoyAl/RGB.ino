#define RGBs D5
#define NUM_PIXELS 96
#define SHOW_TIME 5000

// Neopixel Library
// For strips
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Map Variables to use less memory with high readability
#define index var1
#define hourOffset var1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, RGBs, NEO_GRB + NEO_KHZ800);

uint8_t map60(uint8_t i){
  i %= 60;
  if (i >= 0 && i < 27) {
    return i + 69;
  } else if (i >= 27 && i < 57) {
    return i-27 ;
   } else {
    return i + 9;
   };
}

uint8_t map24(uint8_t i) {
  i %= 24;
  if (i >= 0 && i < 6) {
    return i + 60;
  } else {
    return i + 36 ;
  }
}

uint8_t map12(uint8_t i) {
  i %= 12;
  if (i >= 0 && i < 3) {
    return i + 39;
  } else {
    return 27 + i;
  }
}


#define FF_MODE 0
#define SWING_MODE 1
#define SIMPLE_MODE 2
void displayTime() {
  switch(Prefrences.modeClock){
    case FF_MODE: displayTimeFullFace(Prefrences.ffMode); break;
    case SWING_MODE: break;
    case SIMPLE_MODE: displaySimpleClock(); break;
    default: 
      displaySimpleClock(); break;
  }
}

void initStrip() {
  strip.begin();
  strip.setBrightness();
}

/* 
 *  Color Scheme
 */
uint32_t colorScheme[] = {
  strip.Color(0, 128, 128),   //0
  strip.Color(240,40,0),      //1
  strip.Color(128,128,0),     //2
  strip.Color(64, 80, 80),    //3
  strip.Color(0,0,180),       //4
  strip.Color(0,0,128),       //5
  strip.Color(255, 0, 0),     //6
  strip.Color(8, 0, 0),       //7
  strip.Color(8,0, 8),        //8
  strip.Color(32,40,40),      //9
  strip.Color(0,0,8),         //10
  strip.Color(0,0,4),         //11
  strip.Color(32,40,40),      //12
};

/*
 * Fullface Clock With Three Color Scheme
 */
uint8_t FFDataScheme[][3] = {
         {0,1,2},
         {3,4,5},
         {5,2,2}};

uint32_t FFFaceScheme[][4] = {
         {7,8,9,10},
         {11,11,12,11},
         {7,8,9,10}};

void drawClockFace(uint8_t mode) {
  for ( index =  0; index < 12; index++ )
    strip.setPixelColor(map12(index), colorScheme[FFFaceScheme[mode][0]] );
     
  for ( index =  0; index < 24; index++ )
    strip.setPixelColor(map24(index), colorScheme[FFFaceScheme[mode][1]] );
    
  for ( index =  0; index < 60; index++ )
    if( index % 5 == 0)
      strip.setPixelColor(map60(index), colorScheme[FFFaceScheme[mode][2]]); 
    else
      strip.setPixelColor(map60(index), colorScheme[FFFaceScheme[mode][3]]); 
}

void displayTimeFullFace(uint8_t mode) {
  updateTime();           
  drawClockFace(mode); 
  if (noww.minute() > 30) 
    var1 = 1;
  else 
    var1 = 0;
    
  strip.setPixelColor(map60(noww.second()), colorScheme[FFDataScheme[mode][0]] );
  strip.setPixelColor(map60(noww.minute()), colorScheme[FFDataScheme[mode][1]] );
  strip.setPixelColor(map24(((noww.hour()%12)*2) + var1), colorScheme[FFDataScheme[mode][2]] );
  strip.setPixelColor(map12((noww.hour()%12)), colorScheme[FFDataScheme[mode][2]]);
  strip.setPixelColor(map12(((noww.hour()%12) + var1) % 12 ), colorScheme[FFDataScheme[mode][2]] ); 
  
  strip.show();

  threshold = 1000;
  if (millis() - startState > SHOW_TIME) {goNextState();threshold = 0;}
}

void displaySimpleClock() {
  updateTime();
  for ( index =  0; index < 60; index++ )
    if( index % 5 == 0)
      strip.setPixelColor(map60(index), strip.Color(32,40,40));
    else
      strip.setPixelColor(map60(index), 0);
  
  strip.setPixelColor(map60(noww.second()), Prefrences.color60);
  strip.setPixelColor((int)(map24(noww.minute()/2.5)), Prefrences.color24);
  var1 = noww.hour();
  if (var1 < 13)
    strip.setPixelColor(map12(var1), Prefrences.color12);
  else
    strip.setPixelColor(map12(var1), Prefrences.color12N);

  strip.show();
  threshold = 1000;
  if (millis() - startState > SHOW_TIME) {goNextState();threshold = 0;}
}

void displayDate() {
  updateTime();
  
}

void displayRainbow() {
  
}

void turnOff() {
  for (index = 0; index < strip.numPixels() ; index++){
    strip.setPixelColor(index, 0);
  }
  strip.show();
}
