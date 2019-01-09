#include "Animations.h"
#include "FastLED.h"
#include "DBG.h"

AnimSolidColorCycle::AnimSolidColorCycle(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette)
    : AnimationBase(ledUniverse, currentPalette)
{
    DefaultDelay = 300;
}

void AnimSolidColorCycle::Animate()
{
    if (_animStep == 0)
    {
        IsComplete = false;
    }

    CRGB color = ColorFromPalette(*_currentPalette, _animStep, 255, LINEARBLEND);
    _ledUniverse->FillSingleColor(color);

    if (_animStep == 255)
    {
        _animStep = 0;
        IsComplete = true;
    }
    else
    {
        _animStep++;
    }
}

// ------------------------------------------------------------------------------------------------

AnimRGBCycle::AnimRGBCycle(LedUniverse *ledUniverse) : AnimationBase(ledUniverse)
{
    DefaultDelay = 3000;
}

void AnimRGBCycle::Animate()
{
    if (_animStep == 0)
    {
        _ledUniverse->FillSingleColor(CRGB::Red);
        IsComplete = false;
    }
    else if (_animStep == 1)
    {
        _ledUniverse->FillSingleColor(CRGB::Green);
    }
    else if (_animStep == 2)
    {
        _ledUniverse->FillSingleColor(CRGB::Blue);
    }
    _animStep++;
    if (_animStep > 2)
    {
        IsComplete = true;
        _animStep = 0;
    }
}

// ------------------------------------------------------------------------------------------------
AnimRunningPixel::AnimRunningPixel(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette)
    : AnimationBase(ledUniverse, currentPalette)
{
    DefaultDelay = 50;
}

void AnimRunningPixel::Animate()
{
    if (_animStep == 0)
    {
        _ledUniverse->LtRIndexer->SetColor(NUM_LEDS_TOTAL - 1, CRGB::Black);
        IsComplete = false;
    }
    else
    {
        _ledUniverse->LtRIndexer->SetColor(_animStep - 1, CRGB::Black);
    }
    CRGB color = ColorFromPalette(*_currentPalette, _animStep, 255, LINEARBLEND); // TODO: cover whole range
    _ledUniverse->LtRIndexer->SetColor(_animStep, color);

    _animStep++;
    if (_animStep == NUM_LEDS_TOTAL)
    {
        _animStep = 0;
        IsComplete = true;
    }
}

// ------------------------------------------------------------------------------------------------
AnimPaletteScrollLtr::AnimPaletteScrollLtr(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette)
    : AnimationBase(ledUniverse, currentPalette)
{
    DefaultDelay = 300;
}

void AnimPaletteScrollLtr::Animate()
{
    if (_animStep == 0)
    {
        IsComplete = false;
    }

    uint8_t colorOffset = _animStep;
    for (uint8_t i = 0; i < NUM_LEDS_TOTAL; i++)
    {
        CRGB paletteColor = ColorFromPalette(*_currentPalette, colorOffset);

        _ledUniverse->LtRIndexer->SetColor(i, paletteColor);
        colorOffset += 1;
        //DBG("Led:%u Offset:%u\n", i, colorOffset);
    }

    _animStep++;
    if (_animStep > 255)
    {
        _animStep = 0;
        IsComplete = true;
    }
}

// ------------------------------------------------------------------------------------------------
AnimPaletteScrollTtb::AnimPaletteScrollTtb(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette)
    : AnimationBase(ledUniverse, currentPalette)
{
    DefaultDelay = 300;
}

void AnimPaletteScrollTtb::Animate()
{
    if (_animStep == 0)
    {
        IsComplete = false;
    }
    for (uint8_t i = 0; i < NUM_LEDS_HALF; i++)
    {
        uint8_t offsetVal = i + _animStep; // animStep acts as pallete offset
        uint8_t colorIndex;

        if (offsetVal < NUM_LEDS_HALF)
        {
            colorIndex = map(offsetVal, 0, NUM_LEDS_HALF, 0, 255);
        }
        else
        {
            colorIndex = map(offsetVal - NUM_LEDS_HALF, 0, NUM_LEDS_HALF, 0, 255);
        }
        //DBG("i:%u step:%u colorIndx:%u offsetVal:%u\n", i, _animStep, colorIndex, offsetVal);
        CRGB color = ColorFromPalette(*_currentPalette, colorIndex);
        _ledUniverse->VerticalIndexer->SetColor(i, color);
    }

    _animStep++;
    if (_animStep > NUM_LEDS_HALF)
    {
        _animStep = 0;
        IsComplete = true;
    }
}

// ------------------------------------------------------------------------------------------------

AnimRandomSinglePixel::AnimRandomSinglePixel(LedUniverse *ledUniverse) : AnimationBase(ledUniverse)
{
    DefaultDelay = 200;
}

void AnimRandomSinglePixel::Animate()
{
    if (_animStep == 0)
    {
        IsComplete = false;
    }

    _ledUniverse->LtRIndexer->SetColor(random8(NUM_LEDS_TOTAL), CHSV(random8(),255,255));

    _animStep++;
    if (_animStep = 255)
    {
        IsComplete = true;
        _animStep = 0;
    }
}
