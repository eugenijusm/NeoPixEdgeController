#include "AnimController.h"

#include "LedConfig.h"

AnimController::AnimController(CRGB *leds){
  _leds = leds;
  _currentAnim=0;
  _animStep=0;
}

void AnimController::Animate(){ // TODO: extract to exact animation classes
  if (_animStep==0){
    fill_solid(_leds, NUM_LEDS_TOTAL, CRGB::Red);  
  }
  else if (_animStep==1){
    fill_solid(_leds, NUM_LEDS_TOTAL, CRGB::Green);
  }
  else if (_animStep==2){
    fill_solid(_leds, NUM_LEDS_TOTAL, CRGB::Blue);
  }
  _animStep++;
  if(_animStep>2){
    _animStep=0;
  }
  FastLED.show();
}

void AnimController::ChangeAnim(int animIndex){   // TODO: return default step delay?
  _currentAnim = animIndex;
  _animStep=0;  
}
/*
void LedAnim::SolidRainbowCycle(){
  CRGB color = ColorFromPalette(RainbowColors_p, _animStep, 255, LINEARBLEND);
  _ledCommonActions->FillColor(color);

  _animStep++;
  if(_animStep == 256){
    _animStep = 0;
  }
}
*/
