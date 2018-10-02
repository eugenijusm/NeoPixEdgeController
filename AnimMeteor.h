#ifndef AnimMeteor_h
#define AnimMeteor_h

//#include "Arduino.h"
#include "FastLED.h"

class AnimMeteor
{
public:
  AnimMeteor(CRGB *ledArray, uint8_t stripSize);
  void Animate(CRGB color, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay);
  void AnimateInverted(CRGB color, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay);
  void Setup(bool isInverted);

private:
  void FadeToBlack(uint8_t ledNo, byte fadeValue);
  CRGB *_ledArray;
  uint8_t _animationStep;
  uint8_t _ledStripSize;
  uint8_t _animationStartIndex;
  uint8_t _animationEndIndex;
  bool _isInverted;
};

#endif
