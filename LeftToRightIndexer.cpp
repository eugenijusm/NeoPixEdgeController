#include "LeftToRightIndexer.h"
#include "LedConfig.h"
#include "Dbg.h"

void LeftToRightIndexer::Setup(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top)
{
  _left = left;
  _right = right;
  _bottom = bottom;
  #ifdef RECTANGULAR_CONFIG
    _top = top;
  #endif
}

void LeftToRightIndexer::SetColor(uint8_t index, CRGB color)
{
  if (index < NUM_LEDS_VERTICAL)
  {
    uint8_t idx = NUM_LEDS_VERTICAL - index - 1;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("L Index:%u Idx:%u\n", index, idx);
    #endif
    _left[idx] = color;    
  }

#ifdef RECTANGULAR_CONFIG
  else if (index >= NUM_LEDS_VERTICAL && index < NUM_LEDS_VH)
  {
    uint8_t idx = index - NUM_LEDS_VERTICAL;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("B Index:%u Idx:%u\n", index, idx);
    #endif
    _bottom[idx] = color;    
  }
  else if (index >= NUM_LEDS_VH && index < NUM_LEDS_VHV)
  {
    uint8_t idx = index - NUM_LEDS_VH;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("R Index:%u Idx:%u\n", index, idx);
    #endif
    _right[idx] = color;    
  }
  else{
    uint8_t idx = NUM_LEDS_HORIZONTAL - (index - NUM_LEDS_VHV) - 1;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("T Index:%u Idx:%u\n", index, idx);
    #endif
    _top[idx] = color;    
  }


#else
  else if (index >= NUM_LEDS_VERTICAL && index < NUM_LEDS_VH)
  {
    uint8_t idx = NUM_LEDS_VH - 1 - index;
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("B Index:%u Idx:%u\n", index, idx);
    #endif
    _bottom[idx] = color;    
  }
  else
  {
    uint8_t idx = index - NUM_LEDS_VH;    
    #ifdef DBG_INDEXERS_VERBOSE
      DBG("R Index:%u Idx:%u\n", index, idx);
    #endif
    _right[idx] = color;    
  }
#endif
}
