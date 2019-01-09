#include "SymVerticalIndexer.h"
#include "LedConfig.h"
#include "Dbg.h"

void SymVerticalIndexer::Setup(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top)
{
  _left = left;
  _right = right;
  _bottom = bottom;
  #ifdef RECTANGULAR_CONFIG
    if(top == NULL){
      DBG("TOP ARRAY IS NULL!\n");
    }
    else{
      DBG("Top array OK\n");
    }
    _top = top;
  #endif
}

void SymVerticalIndexer::SetColor(uint8_t index, CRGB color)
{
  if (index < NUM_LEDS_HH)
  {
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("B i1:%u i2:%u\n", (NUM_LEDS_HH-index-1), (NUM_LEDS_HH+index));
    #endif
    _bottom[NUM_LEDS_HH - index - 1] = color;
    _bottom[NUM_LEDS_HH + index] = color;    
  }
  else if( index >= NUM_LEDS_HH && index < (NUM_LEDS_HH + NUM_LEDS_VERTICAL))
  {
    uint8_t idx = index - NUM_LEDS_HH;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("LR idx:%u index:%u hh:%u\n", idx, index, NUM_LEDS_HH);
    #endif
    _left[idx] = color;
    _right[idx] = color;    
  }

#ifdef RECTANGULAR_CONFIG
  else
  {
    uint8_t idx1 = NUM_LEDS_HH - (NUM_LEDS_VH - index - 1) - 1;
    uint8_t idx2 = NUM_LEDS_HH + (NUM_LEDS_VH - index - 1) - 1;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("T index:%u i1:%u i2:%u\n", index, idx1, idx2);
    #endif
    _top[idx1] = color;
    _top[idx2] = color;
  }
#endif
}

// TODO: implement TOP
// CRGB SymVerticalIndexer::GetColor(uint8_t index)
// {
//   if (index < NUM_LEDS_HH)
//   {
//     return _bottom[NUM_LEDS_HH - index - 1];
//     // _bottom[NUM_LEDS_HH+index]
//   }

//   else if (index >= NUM_LEDS_HH)
//   {
//     uint8_t idx = index - NUM_LEDS_HH;
//     return _left[idx];
//     // _right[idx];
//   }
// }
