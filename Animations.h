#ifndef Animations_h
#define Animations_h

#include "AnimationBase.h"

class AnimSolidColorCycle : public AnimationBase
{
    public:
        AnimSolidColorCycle(LedUniverse *ledUniverse);
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
        AnimRunningPixel(LedUniverse *ledUniverse);
        virtual void Animate();
};

class AnimPaletteScrollLtr : public AnimationBase
{
    public:
        AnimPaletteScrollLtr(LedUniverse *ledUniverse);
        virtual void Animate();
};

class AnimPaletteScrollTtb : public AnimationBase
{
    public:
        AnimPaletteScrollTtb(LedUniverse *ledUniverse);
        virtual void Animate();
};

#endif