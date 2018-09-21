#ifndef AnimController_h
#define AnimController_h

#include "FastLED.h"

class AnimController{
  public:
    AnimController(CRGB *leds);
    void Animate();
    void ChangeAnim(int animIndex);
    //void SolidRainbowCycle();
  private:
    int _animStep;
    CRGB *_leds;
    int _currentAnim;
};
#endif
