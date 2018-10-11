#ifndef AnimController_h
#define AnimController_h

#include "LedUniverse.h"
#include "AnimationBase.h"

enum class AnimType
{
  TestRGB,
  SolidColorCycle,
  RunningPixel,
  ScrollPaletteLtR,
  PaletteV,
  Static_PalleteV,
  Static_PalleteH,
  RandomSinglePixel
};

class AnimController
{
public:
  AnimController(LedUniverse *ledUniverse);
  void Animate();
  void ChangeAnim(AnimType animType);
  uint16_t GetDefaultDelay();
  bool AnimComplete;
  CRGBPalette16 CurrentPalette;

  AnimationBase *CurrentAnimation;

private:
  LedUniverse *_ledUniverse;
  AnimType _animType;

};
#endif
