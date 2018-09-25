#ifndef SymVerticalIndexer_h
#define SymVerticalIndexer_h

#include "Arduino.h"
#include "FastLED.h"

class SymVerticalIndexer{
  public:
    void Setup(CRGB *left, CRGB *right, CRGB *bottom);
    //LeftToRightIndexer(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top=NULL);
    void SetColor(int index, CRGB color);
    CRGB GetColor(int index);
  private:
    CRGB *_left;
    CRGB *_right;
    CRGB *_bottom;
    //CRGB *_top;
};

#endif
