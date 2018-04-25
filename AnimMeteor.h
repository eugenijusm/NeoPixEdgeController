#ifndef AnimMeteor_h
#define AnimMeteor_h

//#include "Arduino.h"
#include "FastLED.h"

class AnimMeteor{
  public:
    AnimMeteor(CRGB *ledArray, int stripSize);
    void Animate(CRGB color, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay);
    void AnimateInverted(CRGB color, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay);
    void Setup(bool isInverted);
  private:
    void FadeToBlack(int ledNo, byte fadeValue);
    CRGB *_ledArray;
    int _animationStep;
    int _ledStripSize;
    int _animationStartIndex;
    int _animationEndIndex;
    bool _isInverted;
};

#endif
