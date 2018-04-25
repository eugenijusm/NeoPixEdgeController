#include "LeftToRightIndexer.h"
#include "LedConfig.h"

LeftToRightIndexer::LeftToRightIndexer(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top){
  _left = left;
  _right = right;
  _bottom = bottom;
  _top = top;
  idx = 0;
}

void LeftToRightIndexer::SetColor(int index, CRGB color){
  if(index < NUM_LEDS_VERTICAL){
    idx = NUM_LEDS_VERTICAL-index-1;
    _left[idx] = color;
    #ifdef SERIAL_DBG
    Serial.println("- a");
    #endif
  }

  else if(index >= NUM_LEDS_VERTICAL && index < NUM_LEDS_VH){
    idx = NUM_LEDS_VH-1-index;
    _bottom[idx] = color;
    #ifdef SERIAL_DBG
    Serial.println("- b");
    #endif
  }

  else{
    idx = index - NUM_LEDS_VH;
    _right[idx] = color;
    #ifdef SERIAL_DBG
    Serial.println("- c");
    #endif
  }
}

