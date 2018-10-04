#include "AnimationBase.h"

AnimationBase::AnimationBase(LedUniverse *ledUniverse)
{
    _ledUniverse = ledUniverse;    
    _animStep = 0;
    IsComplete = false;
    _ledUniverse->FillSingleColor(CRGB::Black);
}

AnimationBase::AnimationBase(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette) : AnimationBase(ledUniverse)
{
    _currentPallete = currentPalette;    
}

AnimationBase::~AnimationBase() {}