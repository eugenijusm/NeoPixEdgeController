#include "AnimController.h"
#include "LedConfig.h"
#include "Dbg.h"

AnimController::AnimController(LedUniverse *ledUniverse){
  _ledUniverse = ledUniverse;
  _animType=AnimType::TestRGB;
  _animStep=0;
  AnimComplete=false;
}

void AnimController::Animate(){ // TODO: extract to exact animation classes
  if(_animType==AnimType::TestRGB){
    DBG("RGB\n");
    Animate_TestRGB();
  }
  else if(_animType==AnimType::SolidColorCycle){
    DBG("Solid\n");
    Animate_SolidColorCycle();
  }
  else if(_animType==AnimType::RunningPixel){
    DBG("Runnin\n");
    Animate_RunningPixel();
  }
  else if(_animType==AnimType::ScrollPaletteLtR){
    DBG("Scroll LtR\n");
    Animate_ScrollPaletteLtR();
  }  
  FastLED.show();
}

void AnimController::Animate_TestRGB(){
  if (_animStep==0){
    fill_solid(_ledUniverse->LedsLeft, NUM_LEDS_VERTICAL, CRGB::Red);  
    AnimComplete=false;
  }
  else if (_animStep==1){
    fill_solid(_ledUniverse->LedsLeft, NUM_LEDS_VERTICAL, CRGB::Green);
  }
  else if (_animStep==2){
    fill_solid(_ledUniverse->LedsLeft, NUM_LEDS_VERTICAL, CRGB::Blue);
  }
  _animStep++;
  if(_animStep>2){
    AnimComplete=true;
    _animStep=0;
  }
}

void AnimController::Animate_SolidColorCycle(){
  if(_animStep==0){
    AnimComplete=false;
  }
  
  CRGB color = ColorFromPalette(RainbowColors_p, _animStep, 255, LINEARBLEND);
  fill_solid(_ledUniverse->LedsRight, NUM_LEDS_VERTICAL, color);
   
  _animStep++;   
  if(_animStep == 256){
    _animStep = 0;
    AnimComplete=true;
  }
}

void AnimController::Animate_RunningPixel(){
  if(_animStep==0){
    _ledUniverse->LtRIndexer->SetColor(NUM_LEDS_TOTAL-1, CRGB::Black);
    //_leds[NUM_LEDS_TOTAL-1]= CRGB::Black;
    AnimComplete=false;
  }
  else{
    _ledUniverse->LtRIndexer->SetColor(_animStep-1, CRGB::Black);
    //_leds[_animStep-1]= CRGB::Black;
  }
  CRGB color = ColorFromPalette(RainbowColors_p, _animStep, 255, LINEARBLEND);    // TODO: cover whole range
  _ledUniverse->LtRIndexer->SetColor(_animStep, color);
  //_leds[_animStep]= color;

  _animStep++;
  if(_animStep == NUM_LEDS_TOTAL){
    _animStep = 0;
    AnimComplete=true;
  }
}

void AnimController::Animate_ScrollPaletteLtR(){
  if(_animStep==0){
    AnimComplete=false;
  }
  
  int colorOffset = _animStep;
  for(int i=0; i<NUM_LEDS_TOTAL; i++){    
    CRGB paletteColor = ColorFromPalette(RainbowColors_p, colorOffset); // <--- palette, e.g. CloudColors_p
        
    _ledUniverse->LtRIndexer->SetColor(i, paletteColor);
    colorOffset += 1;
    //DBG("Led:%u Offset:%u\n", i, colorOffset);
  }
  
  _animStep++;
  if(_animStep > 255){
    _animStep=0;
    AnimComplete=true;
  }
}


void AnimController::ChangeAnim(AnimType animType){   // TODO: return default step delay?
  fill_solid(_ledUniverse->LedsLeft, NUM_LEDS_VERTICAL, CRGB::Black);
  fill_solid(_ledUniverse->LedsRight, NUM_LEDS_VERTICAL, CRGB::Black);
  fill_solid(_ledUniverse->LedsBottom, NUM_LEDS_HORIZONTAL, CRGB::Black);
  _animType = animType;
  _animStep=0;
  AnimComplete=false;
}
