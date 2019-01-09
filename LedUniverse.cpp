#include "LedUniverse.h"
#include "Dbg.h"

//CRGB LedUniverse::Leds[NUM_LEDS_TOTAL];
LedUniverse::LedUniverse() {}

void LedUniverse::Setup()
{
  LtRIndexer = new LeftToRightIndexer();
    VerticalIndexer = new SymVerticalIndexer();
  

  FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(LedsLeft, NUM_LEDS_VERTICAL);
  FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(LedsBottom, NUM_LEDS_HORIZONTAL);
  FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(LedsRight, NUM_LEDS_VERTICAL);

#ifdef RECTANGULAR_CONFIG
  FastLED.addLeds<NEOPIXEL, TOP_LEDS_PIN>(LedsTop, NUM_LEDS_HORIZONTAL);
  LtRIndexer->Setup(LedsLeft, LedsRight, LedsBottom, LedsTop);
  VerticalIndexer->Setup(LedsLeft, LedsRight, LedsBottom, LedsTop);
#else
  LtRIndexer->Setup(LedsLeft, LedsRight, LedsBottom);
  VerticalIndexer->Setup(LedsLeft, LedsRight, LedsBottom);
#endif

  /*
   * Single array approach, not suitable because both vertical strips indexing physically starts at the bottom
   FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(Leds, 0, NUM_LEDS_VERTICAL);  // start index, size
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(Leds, NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(Leds, NUM_LEDS_VH, NUM_LEDS_VERTICAL);
   */

  FastLED.setBrightness(MASTER_BRIGHTNESS);
  FillSingleColor(CRGB::Black);
  FastLED.show();

#ifdef RECTANGULAR_CONFIG
  DBG("Rectangular cconfiguration!\n");
#endif
  DBG("READY V:%u H:%u VH:%u Total:%u\n", NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL, NUM_LEDS_VH, NUM_LEDS_TOTAL);
}

void LedUniverse::FillSingleColor(CRGB color)
{
  fill_solid(LedsLeft, NUM_LEDS_VERTICAL, color);
  fill_solid(LedsRight, NUM_LEDS_VERTICAL, color);
  fill_solid(LedsBottom, NUM_LEDS_HORIZONTAL, color);
  //#ifdef RECTANGULAR_CONFIG
  //fill_solid(LedsTop, NUM_LEDS_HORIZONTAL, color);
  //#endif
}
