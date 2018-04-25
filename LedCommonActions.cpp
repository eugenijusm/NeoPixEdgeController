#include "LedCommonActions.h"

LedCommonActions::LedCommonActions(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top){
  _left = left;
  _right = right;
  _bottom = bottom;
  _top = top;
}

void LedCommonActions::FillColor(CRGB color){
  for(int i=0; i<NUM_LEDS_HORIZONTAL; i++){
    _bottom[i] = color;
    #ifndef UCONFIG
      _top[i] = color;
    #endif
    if (i < NUM_LEDS_VERTICAL){
      _left[i] = color;
      _right[i] = color;
    }
  }
}

