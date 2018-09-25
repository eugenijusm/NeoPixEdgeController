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
  else if(_animType==AnimType::HeatFlow){
    DBG("Heat flow\n");
    Animate_HeatFlow();
  }
  else if(_animType==AnimType::Static_VPallete){
    DBG("Static v palette\n");
    View_VPallete();
  }
  FastLED.show();
}


void AnimController::View_VPallete(){
  for (uint8_t i=0; i<NUM_LEDS_HALF; i++){
    uint8_t colorIndex = map(i, 0, NUM_LEDS_HALF, 0, 255);
    CRGB color = ColorFromPalette(RainbowColors_p, colorIndex);
    _ledUniverse->VerticalIndexer->SetColor(i, color);
  }
}


void AnimController::Animate_TestRGB(){
  if (_animStep==0){
    _ledUniverse->FillSingleColor(CRGB::Red);
    AnimComplete=false;
  }
  else if (_animStep==1){
    _ledUniverse->FillSingleColor(CRGB::Green);
  }
  else if (_animStep==2){
    _ledUniverse->FillSingleColor(CRGB::Blue);
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
  _ledUniverse->FillSingleColor(color);
   
  _animStep++;   
  if(_animStep == 256){
    _animStep = 0;
    AnimComplete=true;
  }
}

void AnimController::Animate_RunningPixel(){
  if(_animStep==0){
    _ledUniverse->LtRIndexer->SetColor(NUM_LEDS_TOTAL-1, CRGB::Black);
    AnimComplete=false;
  }
  else{
    _ledUniverse->LtRIndexer->SetColor(_animStep-1, CRGB::Black);
  }
  CRGB color = ColorFromPalette(RainbowColors_p, _animStep, 255, LINEARBLEND);    // TODO: cover whole range
  _ledUniverse->LtRIndexer->SetColor(_animStep, color);

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
    CRGB paletteColor = ColorFromPalette(CloudColors_p, colorOffset); // <--- palette, e.g. CloudColors_p, RainbowColors_p
        
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

void AnimController::Animate_HeatFlow(){
  int lastIndex = NUM_LEDS_HALF - 1;
  int idx = lastIndex - _animStep;
  if(_animStep==0){
    AnimComplete=false;
   _ledUniverse->FillSingleColor(CRGB::Black); 
  }
  else{
    _ledUniverse->VerticalIndexer->SetColor(idx, _ledUniverse->VerticalIndexer->GetColor(idx+1));
  }

  int colorIndex = _animStep*2;
  //DBG("sk:%u vard:%u val:%u colorIndex:%u\n", (lastIndex-_animStep), lastIndex, (lastIndex-_animStep)/lastIndex, colorIndex);
  CRGB paletteColor = ColorFromPalette(LavaColors_p, colorIndex);  //HeatColors_p
  _ledUniverse->VerticalIndexer->SetColor(idx, paletteColor);
  
  _animStep++;
  if(_animStep > NUM_LEDS_HALF){
    _animStep=0;
    AnimComplete=true;
  }
}

void AnimController::ChangeAnim(AnimType animType){   // TODO: return default step delay?
  _ledUniverse->FillSingleColor(CRGB::Black);
  _animType = animType;
  _animStep=0;          // TODO: in most of the cases animation resets this, so it can consistently run more than once
  AnimComplete=false;
}
