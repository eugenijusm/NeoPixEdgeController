#include "LedUniverse.h"
#include "Dbg.h"

//CRGB LedUniverse::Leds[NUM_LEDS_TOTAL];
LedUniverse::LedUniverse(){}

void LedUniverse::Setup(){  
  FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(LedsLeft, NUM_LEDS_VERTICAL);
   #ifndef UCONFIG
      FastLED.addLeds<NEOPIXEL, TOP_LEDS_PIN>(LedsTop, NUM_LEDS_HORIZONTAL);
   #endif
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(LedsBottom, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(LedsRight, NUM_LEDS_VERTICAL);

   LtRIndexer = new LeftToRightIndexer();
   LtRIndexer->Setup(LedsLeft, LedsRight, LedsBottom);
  /*
   FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(Leds, 0, NUM_LEDS_VERTICAL);  // start index, size
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(Leds, NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(Leds, NUM_LEDS_VH, NUM_LEDS_VERTICAL);
   */
   
   FastLED.setBrightness(MASTER_BRIGHTNESS);
   FillSingleColor(CRGB::Black);   
   FastLED.show();

   #ifndef UCONFIG
     DBG("Top LEDs present!");
   #endif
   DBG("READY V:%u H:%u VH:%u Total:%u\n",NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL, NUM_LEDS_VH, NUM_LEDS_TOTAL);
}

void LedUniverse::FillSingleColor(CRGB color){
  fill_solid(LedsLeft, NUM_LEDS_VERTICAL, color);
   fill_solid(LedsRight, NUM_LEDS_VERTICAL, color);
   fill_solid(LedsBottom, NUM_LEDS_HORIZONTAL, color);
}

