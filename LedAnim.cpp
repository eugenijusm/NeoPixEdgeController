#include "LedAnim.h"

LedAnim::LedAnim(LedCommonActions *ledCommonActions){
  _ledCommonActions = ledCommonActions;
  _animStep=0;
}

void LedAnim::SolidRainbowCycle(){
  CRGB color = ColorFromPalette(RainbowColors_p, _animStep, 255, LINEARBLEND);
  _ledCommonActions->FillColor(color);

  _animStep++;
  if(_animStep == 256){
    _animStep = 0;
  }
}

// TODO: animStep reset

