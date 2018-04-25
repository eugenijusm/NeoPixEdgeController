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
  
#define OTA_ON 0  // 0

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
   #ifdef SERIAL_DBG
   Serial.begin(115200);
   Serial.println("Booting");
   #endif

   FastLED.addLeds<NEOPIXEL, LEFT_LEDS_PIN>(leftLeds, NUM_LEDS_VERTICAL);
   #ifndef UCONFIG
   Serial.println("TOP LEDS!!!");
   FastLED.addLeds<NEOPIXEL, TOP_LEDS_PIN>(topLeds, NUM_LEDS_HORIZONTAL);
   #endif
   FastLED.addLeds<NEOPIXEL, BOTTOM_LEDS_PIN>(bottomLeds, NUM_LEDS_HORIZONTAL);
   FastLED.addLeds<NEOPIXEL, RIGHT_LEDS_PIN>(rightLeds, NUM_LEDS_VERTICAL);   
   
   FastLED.setBrightness(MASTER_BRIGHTNESS);
   setAllOff();
   FastLED.show();

   // pinMode(MODE_CHANGE_PIN, INPUT_PULLUP);
   pinMode(OTA_ENABLE_PIN, INPUT_PULLUP);
   if(digitalRead(OTA_ENABLE_PIN)==0){
     ota_enabled = 1;
   }

   if(ota_enabled == 1 || OTA_ON == 1){
     #ifdef SERIAL_DBG
     Serial.println("Init OTA");
     #endif
     pinMode(BUILTIN_LED_PIN, OUTPUT);
     digitalWrite(BUILTIN_LED_PIN, LOW);  // led inverted => led on
     delay(3000);
     digitalWrite(BUILTIN_LED_PIN, HIGH);

      #ifdef SERIAL_DBG
     Serial.println("Wifi CLI");
     #endif
     // Wifi client mode
     WiFi.mode(WIFI_STA);
     WiFi.begin(ssid, password);

     #ifdef SERIAL_DBG
     Serial.println("Wifi begin");
     WiFi.begin(ssid, password);
     #endif
     while (WiFi.status() != WL_CONNECTED){ //waitForConnect
       #ifdef SERIAL_DBG
       Serial.println("Connecting..");
       #endif
       delay ( 500 );       
     }
  
    #ifdef SERIAL_DBG
     Serial.println("Connected");
     #endif  
  
    ArduinoOTA.setHostname(host);
    ArduinoOTA.onStart([]() {
        digitalWrite(BUILTIN_LED_PIN, HIGH);
    });
    #ifdef SERIAL_DBG
     Serial.println("A");
     #endif
    
    ArduinoOTA.onEnd([]() {
      digitalWrite(BUILTIN_LED_PIN, LOW);
    });
    ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
    #ifdef SERIAL_DBG
     Serial.println("B");
     #endif
    ArduinoOTA.begin();
    #ifdef SERIAL_DBG
    Serial.println("OTA ready");
    #endif
  }

  btnMillis = millis()+1000;
  animMillis = millis()+3000;
  //animMeteor.Setup(true); // invert animation, can be included into animate
}

void loop() {
  if(ota_enabled == 1 || OTA_ON == 1){
    ArduinoOTA.handle();
  }

  // animMeteor.Animate(CRGB::Blue, 3, 20, false);
  // animMeteor.Animate(CRGB::Yellow, 3, 50, true);
    #ifdef SERIAL_DBG
    Serial.println("Main loop");
    #endif
   // EVERY_N_SECONDS(1){monitorModeButton();}
   // EVERY_N_SECONDS(2){handleAnimation();}
   //if(btnMillis>millis()){
   //   monitorModeButton();
   //   btnMillis = millis()+1000;
   //}
   if(animMillis>millis()){
    // handleAnimation();
    ledAnim.SolidRainbowCycle();
    Serial.print("pre show");
    FastLED.show();  
    Serial.println("post show");
    animMillis = millis()+3000;
   }
}

void monitorModeButton(){
  #ifdef SERIAL_DBG
    Serial.println("Btn monitor");
    #endif
  if(digitalRead(MODE_CHANGE_PIN)==0){
    #ifdef SERIAL_DBG
    Serial.println("Btn pressed");
    #endif
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

  #ifdef SERIAL_DBG
  Serial.print(animStep);
  Serial.print(" -- ");
  #endif

  animStep = animStep + 1;
  if(animStep == 216){
    animStep = 0;
  }
}

void paletteScroll(){
  int colorOffset = animStep;
  for(int i=0; i<NUM_LEDS_TOTAL; i++){
    Serial.print("Setting led ");
    Serial.print(i);
    Serial.print(" offset ");
    Serial.print(colorOffset);    
    CRGB paletteColor = ColorFromPalette(currentPalette, colorOffset, 255, LINEARBLEND);
    Serial.print(" color ");
    Serial.print(paletteColor);
    leftToRightIndexer.SetColor(i, paletteColor);
    colorOffset += 1;
  }
  animStep++;
  if(animStep > 255){ // TODO: check if this overflows automatically
    animStep=0;
  }
  Serial.print("animstep ");
  Serial.println(animStep);
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
