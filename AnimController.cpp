#include "AnimController.h"
#include "Dbg.h"

#include "Animations.h"
#include "AnimationsStatic.h"

AnimController::AnimController(LedUniverse *ledUniverse)
{
  _ledUniverse = ledUniverse;
  CurrentPalette = RainbowColors_p;
  ChangeAnim(AnimType::TestRGB);
}

void AnimController::Animate()
{
  CurrentAnimation->Animate();
  AnimComplete = CurrentAnimation->IsComplete; // TODO: temporary?
  FastLED.show();
}

void AnimController::ChangeAnim(AnimType animType)
{
  if (CurrentAnimation != NULL)
  {
    delete (CurrentAnimation);
    DBG("Deleted\n");
  }
  int yy = static_cast<int>(animType);
  DBG("Y%uY\n", yy);
  switch (animType)
  {
    case AnimType::TestRGB:    
      CurrentAnimation = new AnimRGBCycle(_ledUniverse);
      DBG("RGB\n");
      break;
    case AnimType::SolidColorCycle:    
      CurrentAnimation = new AnimSolidColorCycle(_ledUniverse);
      DBG("Solid cycle\n");
      break;
    case AnimType::RunningPixel:    
      CurrentAnimation = new AnimRunningPixel(_ledUniverse);
      DBG("Runnin\n");
      break;
    case AnimType::ScrollPaletteLtR:
      CurrentAnimation = new AnimPaletteScrollLtr(_ledUniverse);
      DBG("Scroll LtR\n");
      break;
    case AnimType::PaletteV:    
      CurrentAnimation = new AnimPaletteScrollTtb(_ledUniverse);
      DBG("Palette vertical\n");
      break;    
    case AnimType::Static_PalleteH:    
      CurrentAnimation = new AnimStPaletteHorizontal(_ledUniverse);
      DBG("Palette static H\n");
      break;
    case AnimType::Static_PalleteV:    
      CurrentAnimation = new AnimStPaletteVertical(_ledUniverse, &CurrentPalette);
      DBG("Palette static V\n");
      break;    
    default:    
      CurrentAnimation = new AnimRunningPixel(_ledUniverse);
      DBG("Something wrong!");
      break;    
  }
  _animType = animType;
}

uint16_t AnimController::GetDefaultDelay()
{
  return CurrentAnimation->DefaultDelay;
}