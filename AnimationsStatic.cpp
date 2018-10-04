#include "AnimationsStatic.h"
#include "FastLED.h"
#include "LedConfig.h"

AnimStPaletteVertical::AnimStPaletteVertical(LedUniverse *ledUniverse) : AnimationBase(ledUniverse)
{
    DefaultDelay = 5000;
}

void AnimStPaletteVertical::Animate()
{
    for (uint8_t i = 0; i < NUM_LEDS_HALF; i++)
    {
        uint8_t colorIndex = map(i, 0, NUM_LEDS_HALF, 0, 255);
        CRGB color = ColorFromPalette(RainbowColors_p, colorIndex);     // CurrentPalette
        _ledUniverse->VerticalIndexer->SetColor(i, color);
    }
    IsComplete = true;
}

// ------------------------------------------------------------------------------------------------
AnimStPaletteHorizontal::AnimStPaletteHorizontal(LedUniverse *ledUniverse) : AnimationBase(ledUniverse)
{
    DefaultDelay = 5000;
}

void AnimStPaletteHorizontal::Animate()
{
    for (uint8_t i = 0; i < NUM_LEDS_TOTAL; i++)
    {
        uint8_t colorIndex = map(i, 0, NUM_LEDS_TOTAL, 0, 255);
        CRGB color = ColorFromPalette(RainbowColors_p, colorIndex);     // CurrentPalette
        _ledUniverse->LtRIndexer->SetColor(i, color);
    }
    IsComplete = true;
}