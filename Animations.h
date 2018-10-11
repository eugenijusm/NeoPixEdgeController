#ifndef Animations_h
#define Animations_h

#include "AnimationBase.h"

class AnimSolidColorCycle : public AnimationBase
{
    public:
        AnimSolidColorCycle(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
        virtual void Animate();        
};

class AnimRGBCycle : public AnimationBase
{
    public:
        AnimRGBCycle(LedUniverse *ledUniverse);
        virtual void Animate();
};

class AnimRunningPixel : public AnimationBase
{
    public:
        AnimRunningPixel(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
        virtual void Animate();
};

class AnimPaletteScrollLtr : public AnimationBase
{
    public:
        AnimPaletteScrollLtr(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
        virtual void Animate();
};

class AnimPaletteScrollTtb : public AnimationBase
{
    public:
        AnimPaletteScrollTtb(LedUniverse *ledUniverse, CRGBPalette16 *currentPalette);
        virtual void Animate();
};

class AnimRandomSinglePixel : public AnimationBase
{
    public:
        AnimRandomSinglePixel(LedUniverse *ledUniverse);
        virtual void Animate();
};

#endif