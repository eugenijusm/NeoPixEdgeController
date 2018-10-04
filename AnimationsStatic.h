#ifndef AnimationsStatic_h
#define AnimationsStatic_h

#include "AnimationBase.h"

class AnimStPaletteVertical : public AnimationBase
{
    public:
        AnimStPaletteVertical(LedUniverse *ledUniverse);
        virtual void Animate();
};

class AnimStPaletteHorizontal : public AnimationBase
{
    public:
        AnimStPaletteHorizontal(LedUniverse *ledUniverse);
        virtual void Animate();
};

#endif