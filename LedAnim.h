#ifndef LedAnim_h
#define LedAnim_h

#include "FastLED.h"
#include "LedCommonActions.h"

class LedAnim{
  public:
    LedAnim(LedCommonActions *ledCommonActions);//CRGB *left, CRGB *right, CRGB *bottom, CRGB *top=NULL);
    void SolidRainbowCycle();
  private:
    LedCommonActions *_ledCommonActions;
    int _animStep;
};
#endif
