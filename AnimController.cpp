#include "AnimController.h"
#include "Dbg.h"

#include "Animations.h"
#include "AnimationsStatic.h"

AnimController::AnimController(LedUniverse *ledUniverse)
{
  _ledUniverse = ledUniverse;
  ChangeAnim(AnimType::TestRGB);
}

void AnimController::Animate()
{ 
  CurrentAnimation->Animate();
  AnimComplete = CurrentAnimation->IsComplete;  // TODO: temporary?
  FastLED.show();
}

void AnimController::ChangeAnim(AnimType animType)
{
  if (CurrentAnimation != NULL)
  {
    delete (CurrentAnimation);
    DBG("Deleted\n");
  }

  if (animType == AnimType::TestRGB)
  {
    CurrentAnimation = new AnimRGBCycle(_ledUniverse);
    DBG("RGB\n");
  }
  else if (animType == AnimType::SolidColorCycle)
  {
    CurrentAnimation = new AnimSolidColorCycle(_ledUniverse);
    DBG("Solid cycle\n");
  }
  else if (_animType == AnimType::RunningPixel)
  {
    CurrentAnimation = new AnimRunningPixel(_ledUniverse);
    DBG("Runnin\n");    
  }
  else if (_animType == AnimType::ScrollPaletteLtR)
  {
    CurrentAnimation = new AnimPaletteScrollLtr(_ledUniverse);
    DBG("Scroll LtR\n");    
  }
  else if (_animType == AnimType::PaletteV)
  {
    CurrentAnimation = new AnimPaletteScrollTtb(_ledUniverse);
    DBG("Palette vertical\n");
  }
  else if (_animType == AnimType::Static_PalleteH){
    CurrentAnimation = new AnimStPaletteHorizontal(_ledUniverse);
    DBG("Palette static H\n");
  }
  else if (_animType == AnimType::Static_PalleteV){
    CurrentAnimation = new AnimStPaletteVertical(_ledUniverse);
    DBG("Palette static V\n");
  }
  else{
    DBG("Something wrong!");
  }

  _animType = animType;  
}