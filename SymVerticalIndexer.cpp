#include "SymVerticalIndexer.h"
#include "LedConfig.h"
#include "Dbg.h"

void SymVerticalIndexer::Setup(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top)
{
  _left = left;
  _right = right;
  _bottom = bottom;  
}

void SymVerticalIndexer::SetColor(uint8_t index, CRGB color)
{
  if (index < NUM_LEDS_HH)
  {
    _bottom[NUM_LEDS_HH - index - 1] = color;
    _bottom[NUM_LEDS_HH + index] = color;
    //DBG("B i1:%u i2:%u\n", (NUM_LEDS_HH-index-1), (NUM_LEDS_HH+index));
  }

#ifdef UCONFIG
  else
  {
    uint8_t idx = index - NUM_LEDS_HH;
    _left[idx] = color;
    _right[idx] = color;
    //DBG("LR idx:%u index:%u hh:%u\n", idx, index, NUM_LEDS_HH);
  }
#else
  else if( index >= NUM_LEDS_HH && index < (NUM_LEDS_HH + NUM_LEDS_VERTICAL))
  {
    uint8_t idx = index - NUM_LEDS_HH;
    _left[idx] = color;
    _right[idx] = color;
  }
  else
  {
    uint8_t idx = NUM_LEDS_VH - index - 1;
    _top[NUM_LEDS_HH - idx - 1] = color;
    _top[NUM_LEDS_HH + idx] = color;
  }
#endif
}

// TODO: implement TOP
CRGB SymVerticalIndexer::GetColor(uint8_t index)
{
  if (index < NUM_LEDS_HH)
  {
    return _bottom[NUM_LEDS_HH - index - 1];
    // _bottom[NUM_LEDS_HH+index]
  }

  else if (index >= NUM_LEDS_HH)
  {
    uint8_t idx = index - NUM_LEDS_HH;
    return _left[idx];
    // _right[idx];
  }
}
