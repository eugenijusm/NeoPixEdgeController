#ifndef LedUniverse_h
#define LedUniverse_h

#include "FastLED.h"
#include "LedConfig.h"

class LedUniverse{
  public:
    LedUniverse();
    void Setup();
    CRGB Leds[NUM_LEDS_TOTAL];
};

#endif
