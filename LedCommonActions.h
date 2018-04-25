#ifndef LedCommonActions_h
#define LedCommonActions_h

#include "FastLED.h"
#include "LedConfig.h"

class LedCommonActions{
  public:
    LedCommonActions(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top=NULL);
    void FillColor(CRGB color);  // This assumes that horizontal strips are longer
  private:
    CRGB *_left;
    CRGB *_right;
    CRGB *_bottom;
    CRGB *_top;
};

#endif
