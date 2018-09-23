#ifndef LedUniverse_h
#define LedUniverse_h

#include "FastLED.h"
#include "LedConfig.h"

#include "LeftToRightIndexer.h"

class LedUniverse{
  public:
    LedUniverse();
    void Setup();
    void FillSingleColor(CRGB color);
    CRGB LedsLeft[NUM_LEDS_VERTICAL];
    CRGB LedsBottom[NUM_LEDS_HORIZONTAL];
    CRGB LedsRight[NUM_LEDS_VERTICAL];
    LeftToRightIndexer *LtRIndexer; 
};

#endif
