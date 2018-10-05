#ifndef AnimationBase_h
#define AnimationBase_h

#include "LedUniverse.h"

class AnimationBase
{
public:
  AnimationBase(LedUniverse *ledUniverse);
  AnimationBase(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
  virtual ~AnimationBase();
  virtual void Animate() = 0;
  uint8_t DefaultDelay;
  bool IsComplete;

protected:
  uint8_t _animStep;
  LedUniverse *_ledUniverse;
  CRGBPalette16 *_currentPalette;
};

#endif