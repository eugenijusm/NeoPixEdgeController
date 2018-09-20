#ifndef LeftToRightIndexer_h
#define LeftToRightIndexer_h

#include "Arduino.h"
#include "FastLED.h"

class LeftToRightIndexer{
  public:
    LeftToRightIndexer(CRGB *left, CRGB *right, CRGB *bottom);
    //LeftToRightIndexer(CRGB *left, CRGB *right, CRGB *bottom, CRGB *top=NULL);
    void SetColor(int index, CRGB color);
  private:
    CRGB *_left;
    CRGB *_right;
    CRGB *_bottom;
    //CRGB *_top;
    int idx;
};

#endif
