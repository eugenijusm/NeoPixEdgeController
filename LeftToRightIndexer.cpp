#include "LeftToRightIndexer.h"
#include "LedConfig.h"
#include "Dbg.h"

void LeftToRightIndexer::Setup(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top)
{ //, CRGB *top){
  _left = left;
  _right = right;
  _bottom = bottom;
  #ifndef UCONFIG
  _top = top;
  #endif
}

void LeftToRightIndexer::SetColor(uint8_t index, CRGB color)
{
  if (index < NUM_LEDS_VERTICAL)
  {
    uint8_t idx = NUM_LEDS_VERTICAL - index - 1;
    _left[idx] = color;
    //DBG("L\n");
  }

  else if (index >= NUM_LEDS_VERTICAL && index < NUM_LEDS_VH)
  {
    uint8_t idx = NUM_LEDS_VH - 1 - index;
    _bottom[idx] = color;
    //DBG("B\n");
  }

#ifdef UCONFIG
  else
  {
    uint8_t idx = index - NUM_LEDS_VH;
    // DBG("index:%u VH:%u idx:%u\n", index, NUM_LEDS_VH, idx);
    _right[idx] = color;
    //DBG("R\n");
  }
#else
  else if (index >= NUM_LEDS_VH && index < NUM_LEDS_VHV)
  {
    uint8_t idx = index - NUM_LEDS_VH;
    _right[idx] = color;
  }
  else{
    uint8_t idx = NUM_LEDS_VERTICAL - (index - NUM_LEDS_VHV);
    _top[idx] = color;
  }
#endif
}
