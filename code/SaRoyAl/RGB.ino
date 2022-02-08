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

uint8_t map60(int8_t i){
  i %= 60;
  if(i < 0) i += 60;
  if (i >= 0 && i < 27) {
    return i + 69;
  } else if (i >= 27 && i < 57) {
    return i-27 ;
   } else {
    return i + 9;
   };
}

uint8_t map24(int8_t i) {
  i %= 24;
  if(i < 0) i += 24;
  if (i >= 0 && i < 6) {
    return i + 60;
  } else {
    return i + 36 ;
  }
}

uint8_t map12(int8_t i) {
  i %= 12;
  if(i < 0) i += 24;
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
    case SWING_MODE: displayClockSwing(); break;
    case SIMPLE_MODE: displaySimpleClock(); break;
    default: 
      displaySimpleClock(); break;
  }
}

void initStrip() {
  strip.begin();
  strip.setBrightness(50);
  strip.show();
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

void displayClockSwing() {
  // if completed three swing
  if(k==2){
    updateTime();
    for(index = 0; index < noww.hour()%12; index++){
      strip.setPixelColor(map12(index), Prefrences.color12);
    }
    for(index = 0; index < noww.minute()/2.5; index++){
      strip.setPixelColor(map24(index), Prefrences.color24);
    }
    for(index = 0; index < noww.second(); index++){
      strip.setPixelColor(map60(index), Prefrences.color60);
    }
    threshold = 1000;
  } else {
     if (j==0) {
      if (i_loc <= noww.hour()%12){
        for(index = 0; index < i_loc; index++){
          strip.setPixelColor(map12(index), Prefrences.color12);
        }
        i_loc++;
        threshold = 60;
      } else {
        j++;
        i_loc = 0;
      }
     } else if (j ==1) {
      if (i_loc <= noww.minute()/2.5){
        for(index = 0; index < i_loc; index++){
          strip.setPixelColor(map24(index), Prefrences.color24);
        }
        i_loc++;
      } else {
        j++;
        i_loc = 0;
      }
     } else if (j ==2) {
      if (i_loc <= noww.second()){
        for(index = 0; index < i_loc; index++){
          strip.setPixelColor(map60(index), Prefrences.color60);
        }
        i_loc++;
      } else {
        j++;
        i_loc = 0;
      }
     } else {
      k = 2;
     }
  }
  
  strip.show();
  if (millis() - startState > SHOW_TIME + 3000) {goNextState(); threshold = 0;}
}

void displayDate() {
  updateTime();
  int index;
  auto date= Prefrences.isJalali? gregorianToJalali(noww.year(), noww.month(), noww.day()):
                                                    DateStruct{noww.year(), noww.month(), noww.day()};
  
  for(index = 0; index < date.day; index++) strip.setPixelColor(map60(index), Prefrences.color60);
  for(index = 0; index <date.month; index++)strip.setPixelColor(map12(index), Prefrences.color12);
  
  for(index=0; index<24; index++) { 
      int pixelHue = i_loc + (index * 65536L / 24);
      strip.setPixelColor(map24(index), strip.gamma32(strip.ColorHSV(pixelHue)));
  }

  for ( index =  0; index < date.day; index++ )
    if( index % 5 == 0)
      strip.setPixelColor(map60(index), strip.Color(32,40,40));

      
  strip.show();

  threshold = 20;
  i_loc += 256;
  if (millis() - startState > SHOW_TIME) {goNextState(); threshold = 0;}
}

void setBrightnessStrip() {
  strip.setBrightness(Prefrences.brightness);
}


void showAlarmClock(uint8_t h, uint8_t m) {
  for ( index =  0; index < 60; index++ )
    if( index % 5 == 0)
      strip.setPixelColor(map60(index), strip.Color(32,40,40));
    else
      strip.setPixelColor(map60(index), 0);
  
  strip.setPixelColor(map60(h), Prefrences.color60);
  strip.setPixelColor(map60(m), Prefrences.color24);
  strip.show();
}


void displayPercentage() {
    if (i_loc < batteryPercent) {
        for (int index = 0; index < i_loc; index++ ) {
          int pixelHue = (index * 65536L / 60);
          strip.setPixelColor(map60(index), strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        threshold = 50;
        i_loc++;
        strip.show();
    } else {
      threshold = 1000;
    }
    if (millis() - startState > SHOW_TIME) {goNextState(); threshold = 0;}
}


void displayTemprature() {
  if (temprature < 0) {
    if (i_loc < -1*temprature) {
        for (int index = 0; index > -1*i_loc; index-- ) {
          int pixelHue = ((40 - index) * 65536L / 80);
          strip.setPixelColor(map60(index+15), strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        threshold = 50;
        i_loc++;
        strip.show();
    } else {
      threshold = 1000;
    }
  } else {
    if (i_loc < temprature) {
        for (int index = 0; index < i_loc; index++ ) {
          if (index < 40) {
            int pixelHue = ((40 - index) * 65536L / 80);
            strip.setPixelColor(map60(index+15), strip.gamma32(strip.ColorHSV(pixelHue)));
          } else {
            strip.setPixelColor(map60(index+15), strip.Color(255,0,0));
          }
        }
        threshold = 50;
        i_loc++;
        strip.show();
    } else {
      threshold = 1000;
    }
  }
    
  if (millis() - startState > SHOW_TIME) {goNextState(); threshold = 0;}
}


void displayRainbow() {
  
  for(int index=0; index<strip.numPixels(); index++) { 
      int pixelHue = i_loc + (index * 65536L / strip.numPixels());
      strip.setPixelColor(index, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show();

  threshold = 10;
  i_loc += 256;
  if (millis() - startState > SHOW_TIME) {goNextState();; threshold = 0;}
}


void displayPackman() {
  for (index = 0; index < strip.numPixels() ; index++){
    strip.setPixelColor(index, 0);
  }
  for(i_loc = 0; i_loc < 60; i_loc++)
  {
      strip.setPixelColor(map60(i_loc), strip.Color(255,255,0));
  }
  for(i_loc = 15; i_loc < 15+j; i_loc++)
  {
      strip.setPixelColor(i_loc+j, 0);
      strip.setPixelColor(i_loc, 0);
      strip.setPixelColor(i_loc-j, 0);
  }
  strip.show();
  j = j + k;
  if(j >= 5 || j == 0 ) k *= -1;

  threshold = 60;
  if (millis() - startState > SHOW_TIME) {goNextState(); threshold = 0;}
}

void turnOff() {
  for (index = 0; index < strip.numPixels() ; index++){
    strip.setPixelColor(index, 0);
  }
  strip.show();
}
