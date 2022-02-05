
#define RGBs D5
#define NUM_PIXELS 96

// Neopixel Library
// For strips
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, RGBs, NEO_GRB + NEO_KHZ800);

void initStrip() {
  strip.begin();
}

static void testStrip(){
  chase(strip.Color(255, 0, 255)); // Red
}

static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
  }
}
