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
      CurrentAnimation = new AnimSolidColorCycle(_ledUniverse, &CurrentPalette);
      DBG("Solid cycle\n");
      break;
    case AnimType::RunningPixel:    
      CurrentAnimation = new AnimRunningPixel(_ledUniverse, &CurrentPalette);
      DBG("Runnin\n");
      break;
    case AnimType::ScrollPaletteLtR:
      CurrentAnimation = new AnimPaletteScrollLtr(_ledUniverse, &CurrentPalette);
      DBG("Scroll LtR\n");
      break;
    case AnimType::PaletteV:    
      CurrentAnimation = new AnimPaletteScrollTtb(_ledUniverse, &CurrentPalette);
      DBG("Palette vertical\n");
      break;    
    case AnimType::Static_PalleteH:    
      CurrentAnimation = new AnimStPaletteHorizontal(_ledUniverse, &CurrentPalette);
      DBG("Palette static H\n");
      break;
    case AnimType::Static_PalleteV:    
      CurrentAnimation = new AnimStPaletteVertical(_ledUniverse, &CurrentPalette);
      DBG("Palette static V\n");
      break;
    case AnimType::RandomSinglePixel:
      CurrentAnimation = new AnimRandomSinglePixel(_ledUniverse);
      DBG("Random single pixel\n");
      break;
    default:    
      CurrentAnimation = new AnimRGBCycle(_ledUniverse);  // Fallback, needed to prevent null ref.
      DBG("Something wrong!");
      break;    
  }
  _animType = animType;
}

uint16_t AnimController::GetDefaultDelay()
{
  return CurrentAnimation->DefaultDelay;
}