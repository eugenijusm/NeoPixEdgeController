#ifndef AnimController_h
#define AnimController_h

#include "FastLED.h"
#include "LeftToRightIndexer.h"
#include "LedUniverse.h"

enum AnimType
{ // TODO: this now holds not only animations, rename to view type or smth
  TestRGB,
  SolidColorCycle,
  RunningPixel,
  ScrollPaletteLtR,
  PaletteV,
  Static_PalleteV,
  Static_PalleteH
};

class AnimController
{
public:
  AnimController(LedUniverse *ledUniverse);
  void Animate();
  void ChangeAnim(AnimType animType);
  bool AnimComplete;
  bool IsStaticView;
  CRGBPalette16 CurrentPalette;

  void View_PalleteV();
  void View_PalleteH();

private:
  uint8_t _animStep;
  LedUniverse *_ledUniverse;
  AnimType _animType;

  void Animate_TestRGB();
  void Animate_SolidColorCycle();
  void Animate_RunningPixel();
  void Animate_ScrollPaletteLtR();
  void Animate_PaletteVertical();
};
#endif
