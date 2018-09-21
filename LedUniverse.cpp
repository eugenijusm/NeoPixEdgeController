#include "LedUniverse.h"
#include "Dbg.h"

//CRGB LedUniverse::Leds[NUM_LEDS_TOTAL];
LedUniverse::LedUniverse(){}

void LedUniverse::Setup(){
  /*
   * Old Separate arrays implementation
  FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(leftLeds, NUM_LEDS_VERTICAL);
   #ifndef UCONFIG
      FastLED.addLeds<NEOPIXEL, TOP_LEDS_PIN>(topLeds, NUM_LEDS_HORIZONTAL);
   #endif
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(bottomLeds, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(rightLeds, NUM_LEDS_VERTICAL);
   */

   FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(Leds, 0, NUM_LEDS_VERTICAL);  // start index, size
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(Leds, NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(Leds, NUM_LEDS_VH, NUM_LEDS_VERTICAL);
   
   FastLED.setBrightness(MASTER_BRIGHTNESS);
   fill_solid(Leds, NUM_LEDS_TOTAL, CRGB::Black);
   FastLED.show();

   #ifndef UCONFIG
     DBG("Top LEDs present!");
   #endif
   DBG("READY V:%u H:%u VH:%u Total:%u\n",NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL, NUM_LEDS_VH, NUM_LEDS_TOTAL);
}

