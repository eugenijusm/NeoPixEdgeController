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

#include "LeftToRightIndexer.h"
#include "LedCommonActions.h"
#include "LedAnim.h"

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

// CRGB leds[NUM_LEDS_TOTAL];  // wrapper array

LeftToRightIndexer leftToRightIndexer(leftLeds, rightLeds, bottomLeds);
LedCommonActions ledCommonActions(leftLeds, rightLeds, bottomLeds);
LedAnim ledAnim(&ledCommonActions);

// AnimMeteor animMeteor(leftLeds, NUM_LEDS_VERTICAL);
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

   setup_fastLed();

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

  
  //simple_palette_scroll(100);
  //handleAnimation();
  RunningPixel();
  FastLED.show();
  delay(300);
  
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

void setup_fastLed(){
  FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(leftLeds, NUM_LEDS_VERTICAL);
   #ifndef UCONFIG
      FastLED.addLeds<NEOPIXEL, TOP_LEDS_PIN>(topLeds, NUM_LEDS_HORIZONTAL);
   #endif
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(bottomLeds, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(rightLeds, NUM_LEDS_VERTICAL);   
   
   FastLED.setBrightness(MASTER_BRIGHTNESS);
   setAllOff();
   FastLED.show();

   #ifndef UCONFIG
     DBG("Top LEDs present!");
   #endif
   DBG("READY V:%u H:%u VH:%u Total:%u\n",NUM_LEDS_VERTICAL, NUM_LEDS_HORIZONTAL, NUM_LEDS_VH, NUM_LEDS_TOTAL);
}

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

void setBuiltinLed(bool turnedOn){
  if (turnedOn){
    digitalWrite(BUILTIN_LED_PIN, LOW);  // led inverted => led on
  }
  else{
    digitalWrite(BUILTIN_LED_PIN, HIGH);
  }
}

void handleAnimation(){  
  if(animationType==0){
    currentPalette = RainbowColors_p;
    paletteScroll();
  }
  else if (animationType==1){
    currentPalette = CloudColors_p;
    paletteScroll();
  }
  else{
    ledAnim.SolidRainbowCycle();
  } 
}

void RunningPixel(){
  setAllOff();  // TODO: can be significantly improved with prevIndex

  leftToRightIndexer.SetColor(animStep, CRGB::Blue);

  DBG("step %u\n", animStep);

  animStep = animStep + 1;
  if(animStep == 216){
    animStep = 0;
  }
}

void paletteScroll(){
  int colorOffset = animStep;
  for(int i=0; i<NUM_LEDS_TOTAL; i++){    
    CRGB paletteColor = ColorFromPalette(currentPalette, colorOffset, 255, LINEARBLEND);
    
    DBG("Led:%u Offset:%u\n", i, colorOffset);
    leftToRightIndexer.SetColor(i, paletteColor);
    colorOffset += 1;
  }
  animStep++;
  if(animStep > 255){ // TODO: check if this overflows automatically
    animStep=0;
  }
  DBG("step %u\n", animStep);
}

void simple_palette_scroll(uint8_t colorOffset){
  for(int i=0; i<NUM_LEDS_VERTICAL; i++){
    leftLeds[i] = ColorFromPalette(RainbowColors_p, colorOffset, 255, LINEARBLEND);
    rightLeds[i] = ColorFromPalette(RainbowColors_p, colorOffset, 255, LINEARBLEND);
    colorOffset += 4;
  }

  for(int i=0; i<NUM_LEDS_HORIZONTAL; i++){
    bottomLeds[i] = CRGB::Red;
  }
}

void setAllOff(){
  for(int i=0; i<NUM_LEDS_VERTICAL; i++){
    leftLeds[i] = CRGB::Black;
    rightLeds[i] = CRGB::Black;
  }

  for(int i=0; i<NUM_LEDS_HORIZONTAL; i++){
    bottomLeds[i] = CRGB::Black;
  }
}
