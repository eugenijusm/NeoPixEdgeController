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
  else if(_animType==AnimType::PaletteV){
    //DBG("Palette vertical\n");
    //CurrentPalette=RainbowColors_p;
    Animate_PaletteVertical();
  }  
  FastLED.show();
}

void AnimController::ChangeAnim(AnimType animType){   // TODO: return default step delay?
  _ledUniverse->FillSingleColor(CRGB::Black);
  _animType=animType;  
  _animStep=0;
  AnimComplete=false;
  if(animType==AnimType::TestRGB || animType==AnimType::SolidColorCycle || animType==AnimType::RunningPixel || animType==AnimType::ScrollPaletteLtR || animType==AnimType::  PaletteV){
    IsStaticView=false; 
  }
  else{
    IsStaticView=true;
  }
}

// ------------------------------------------------- Static views ------------------------------------------------------

void AnimController::View_PalleteV(){
  for (uint8_t i=0; i<NUM_LEDS_HALF; i++){
    uint8_t colorIndex = map(i, 0, NUM_LEDS_HALF, 0, 255);
    CRGB color = ColorFromPalette(CurrentPalette, colorIndex);
    _ledUniverse->VerticalIndexer->SetColor(i, color);
  }
  FastLED.show();
}

void AnimController::View_PalleteH(){
  for (uint8_t i=0; i<NUM_LEDS_TOTAL; i++){
    uint8_t colorIndex = map(i, 0, NUM_LEDS_TOTAL, 0, 255);
    CRGB color = ColorFromPalette(CurrentPalette, colorIndex);
    _ledUniverse->LtRIndexer->SetColor(i, color);
  }
  FastLED.show();
}

// ------------------------------------------------- Animations------------------------------------------------------

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
  
  uint8_t colorOffset = _animStep;
  for(uint8_t i=0; i<NUM_LEDS_TOTAL; i++){    
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

// animStep acts as pallete offset
void AnimController::Animate_PaletteVertical(){
  for (uint8_t i=0; i<NUM_LEDS_HALF; i++){
    uint8_t offsetVal=i+_animStep;
    uint8_t colorIndex;
  
    if(offsetVal < NUM_LEDS_HALF){
      colorIndex = map(offsetVal, 0, NUM_LEDS_HALF, 0, 255);    
      //DBG("A");
    }
    else{
      colorIndex = map(offsetVal-NUM_LEDS_HALF, 0, NUM_LEDS_HALF, 0, 255);    
      //DBG("B");
    }
    //DBG("i:%u step:%u colorIndx:%u offsetVal:%u\n", i, _animStep, colorIndex, offsetVal);
    CRGB color = ColorFromPalette(CurrentPalette, colorIndex);
    _ledUniverse->VerticalIndexer->SetColor(i, color);
  }  
  
  
  _animStep++;
  if(_animStep > NUM_LEDS_HALF){
    _animStep=0;
    AnimComplete=true;
  }
}
