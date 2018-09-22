#ifndef AnimController_h
#define AnimController_h

#include "FastLED.h"
#include "LeftToRightIndexer.h"
#include "LedUniverse.h"

enum AnimType{
  TestRGB,
  SolidColorCycle,
  RunningPixel,
  ScrollPaletteLtR
};

class AnimController{
  public:
    AnimController(LedUniverse *ledUniverse);
    void Animate();
    void ChangeAnim(AnimType animType);
    bool AnimComplete;
  private:
    int _animStep;
    LedUniverse *_ledUniverse;
    AnimType _animType;

    void Animate_TestRGB();
    void Animate_SolidColorCycle();
    void Animate_RunningPixel();
    void Animate_ScrollPaletteLtR();
};
#endif
