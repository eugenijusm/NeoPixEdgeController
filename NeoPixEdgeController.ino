// button debounce
// https://gist.github.com/erikpena/a01ace32f7bef4a5ec52
// attach interrupt
// http://esp8266.github.io/Arduino/versions/2.1.0-rc2/doc/reference.html

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FastLED.h"

#include "LedConfig.h"
#include "WifiConfig.h"

//#include "LeftToRightIndexer.h"
#include "LedCommonActions.h"
#include "AnimController.h"

#include "LedUniverse.h"
#include "AnimMeteor.h"

#include "Dbg.h"
 
#define OTA_ON 0  // Set to 1 to Force OTA

#define BUILTIN_LED_PIN 2 // D4
#define OTA_ENABLE_PIN 14 // D5
int ota_enabled = 0;
#define MODE_CHANGE_PIN 13 // 13 = D7; 12 = D6


CRGB leftLeds[NUM_LEDS_VERTICAL];
CRGB rightLeds[NUM_LEDS_VERTICAL];
CRGB bottomLeds[NUM_LEDS_HORIZONTAL];
#ifndef UCONFIG
CRGB topLeds[NUM_LEDS_HORIZONTAL];
#endif

LedUniverse ledUniverse;
AnimController animController(&ledUniverse);

//LeftToRightIndexer leftToRightIndexer(leftLeds, rightLeds, bottomLeds);
LedCommonActions ledCommonActions(leftLeds, rightLeds, bottomLeds);

AnimMeteor animMeteor(bottomLeds, NUM_LEDS_HORIZONTAL);

int animStep=0;
int animationType=0;
#define MAX_ANIMATION_TYPE 2

CRGBPalette16 currentPalette;

uint32_t btnMillis;
uint32_t animMillis;

void setup() {

  WiFi.persistent(false); // Do not persist Wifi settings; doesn't seem to work :/
  
   #ifdef DEBUG_ESP_PORT
      Serial.begin(115200);      
   #endif
   DBG("Start\n");

   ledUniverse.Setup();
   
   // pinMode(MODE_CHANGE_PIN, INPUT_PULLUP);
   
   pinMode(OTA_ENABLE_PIN, INPUT_PULLUP);
   if(digitalRead(OTA_ENABLE_PIN)==0){
     ota_enabled = 1;
   }

   if(ota_enabled == 1 || OTA_ON == 1){
     setup_wifi();  // must be done as early as possible; stops working if led is touched first :/
     DBG("Init OTA");

     pinMode(BUILTIN_LED_PIN, OUTPUT);
     setBuiltinLed(true);
     delay(1000);
     setBuiltinLed(false);

    ArduinoOTA.setHostname(host);
    ArduinoOTA.onStart([]() {
      setBuiltinLed(true);
      DBG("Ota start");
    });
    
    ArduinoOTA.onEnd([]() {
      setBuiltinLed(false);
      DBG("Ota end");
    });
    
    ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
    ArduinoOTA.begin();

    DBG("OTA ready");    
  }

  btnMillis = millis()+1000;
  animMillis = millis()+3000;
  //animMeteor.Setup(true); // invert animation, can be included into animate
}

void loop() {
  if(ota_enabled == 1 || OTA_ON == 1){
    ArduinoOTA.handle();
  }
  
  animController.ChangeAnim(AnimType::TestRGB);
  while(!animController.AnimComplete){
    animController.Animate();
    delay(1000);
  }
  
  animController.ChangeAnim(AnimType::RunningPixel);
  while(!animController.AnimComplete){
    animController.Animate();
    delay(100);
  }
  
  animController.ChangeAnim(AnimType::SolidColorCycle);
  while(!animController.AnimComplete){
    animController.Animate();
    delay(100);
  }
  
  animController.ChangeAnim(AnimType::ScrollPaletteLtR);
  while(!animController.AnimComplete){
    animController.Animate();
    delay(300);
  }

  
  // animMeteor.Animate(CRGB::Yellow, 3, 50, true);
   // EVERY_N_SECONDS(1){monitorModeButton();}
   // EVERY_N_SECONDS(2){handleAnimation();}
   //if(btnMillis>millis()){
   //   monitorModeButton();
   //   btnMillis = millis()+1000;
   //}


   /*
   if(animMillis>millis()){
        ledAnim.SolidRainbowCycle(); // TODO: should be like this
    FastLED.show();  
    animMillis = millis()+3000;
   }
   */
}


void setup_wifi(){
  DBG("Wifi Setup");

  // Wifi client mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
     
  while (WiFi.status() != WL_CONNECTED){
    DBG(".");
    delay (1000);       
  }
  
  DBG("Wifi Ok");
}
/*
void monitorModeButton(){
  DBG("Btn monitor");
  if(digitalRead(MODE_CHANGE_PIN)==0){
    DBG("Btn pressed");

    if(animationType==MAX_ANIMATION_TYPE){
      animationType=0;
    }
    else{
      animationType++;
    }
    animStep=0;
    handleAnimation();
  }
}
*/

void setBuiltinLed(bool turnedOn){
  if (turnedOn){
    digitalWrite(BUILTIN_LED_PIN, LOW);  // led inverted => led on
  }
  else{
    digitalWrite(BUILTIN_LED_PIN, HIGH);
  }
}
