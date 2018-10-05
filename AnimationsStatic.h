#ifndef AnimationsStatic_h
#define AnimationsStatic_h

#include "AnimationBase.h"

class AnimStPaletteVertical : public AnimationBase
{
    public:
        AnimStPaletteVertical(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
        virtual void Animate();
};

class AnimStPaletteHorizontal : public AnimationBase
{
    public:
        AnimStPaletteHorizontal(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
        virtual void Animate();
};

#endif