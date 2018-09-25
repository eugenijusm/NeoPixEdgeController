#include "SymVerticalIndexer.h"
#include "LedConfig.h"
#include "Dbg.h"

void SymVerticalIndexer::Setup(CRGB *left, CRGB *right, CRGB *bottom){//, CRGB *top){
  _left = left;
  _right = right;
  _bottom = bottom;
  //_top = top;
}

void SymVerticalIndexer::SetColor(int index, CRGB color){
  if(index < NUM_LEDS_HH){
    _bottom[NUM_LEDS_HH-index-1]= color;
    _bottom[NUM_LEDS_HH+index]= color;
    DBG("B i1:%u i2:%u\n", (NUM_LEDS_HH-index-1), (NUM_LEDS_HH+index));
  }

  else if(index >= NUM_LEDS_HH){
    int idx = index-NUM_LEDS_HH;
    _left[idx]=color;
    _right[idx]=color;
    DBG("LR idx:%u index:%u hh:%u\n", idx, index, NUM_LEDS_HH);
  }  
}

CRGB SymVerticalIndexer::GetColor(int index){
  if(index < NUM_LEDS_HH){
    return _bottom[NUM_LEDS_HH-index-1];
    // _bottom[NUM_LEDS_HH+index]    
  }

  else if(index >= NUM_LEDS_HH){
    int idx = index-NUM_LEDS_HH;
    return _left[idx];
    // _right[idx];
  }
}
