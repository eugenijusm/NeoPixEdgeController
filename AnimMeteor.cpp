#include "AnimMeteor.h"

AnimMeteor::AnimMeteor(CRGB *ledArray, int stripSize){
  _ledArray = ledArray;
  _ledStripSize = stripSize;
  Setup(false);
}

void AnimMeteor::Animate(CRGB color, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay){
  // TODO: refactor to reuse routine from main file
  if(_animationStep==_animationStartIndex){
    for(int i=0; i<_ledStripSize; i++){
      _ledArray[i] = CRGB::Black;
    }
  }
  
  // fade brightness all LEDs one step
  for(int j=0; j<_ledStripSize; j++) {
    if( (!meteorRandomDecay) || (random(10)>5) ) {
      FadeToBlack(j, meteorTrailDecay );        
    }
  }   

  // draw meteor
  for(int j = 0; j < meteorSize; j++) {
    int pixelIndex;
    if(_isInverted){
      pixelIndex =_animationStep-j;
    }
    else{
      pixelIndex =_animationStep+j;
    }
    
    if( ( pixelIndex <_ledStripSize) && (pixelIndex >= 0) ) {
      _ledArray[pixelIndex]=color;
    }
  }

  if(_isInverted){
    _animationStep--;
  }
  else{
    _animationStep++;  
  }
  
  if(_animationStep== _animationEndIndex){
    _animationStep=_animationStartIndex;
  }
}

void AnimMeteor::Setup(bool isInverted){
  _isInverted = isInverted;
  if(isInverted){
    _animationStep=_ledStripSize;
    _animationStartIndex=_ledStripSize;
    _animationEndIndex=-_ledStripSize;
  }
  else{
    _animationStep=0;
    _animationStartIndex=0;
    _animationEndIndex=(_ledStripSize + _ledStripSize);
  }
}

void AnimMeteor::FadeToBlack(int ledNo, byte fadeValue) {
  _ledArray[ledNo].fadeToBlackBy( fadeValue ); 
}

