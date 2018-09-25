// button debounce
// https://gist.github.com/erikpena/a01ace32f7bef4a5ec52
// attach interrupt
// http://esp8266.github.io/Arduino/versions/2.1.0-rc2/doc/reference.html

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FastLED.h"

#include "LedConfig.h"
#include "WifiConfig.h"

#include "AnimController.h"

#include "LedUniverse.h"
#include "Dbg.h"
 
#define OTA_ON 0  // Set to 1 to Force OTA

#define BUILTIN_LED_PIN 2 // D4
#define OTA_ENABLE_PIN 14 // D5
int ota_enabled = 0;
#define MODE_CHANGE_PIN 13 // 13 = D7; 12 = D6

LedUniverse ledUniverse;
AnimController animController(&ledUniverse);

uint32_t btnMillis=millis();
uint32_t animMillis=millis();

#define ANIM_COUNT 2
AnimType _demoAnims[]={AnimType::TestRGB, AnimType::HeatFlow, /*AnimType::RunningPixel, AnimType::SolidColorCycle, AnimType::ScrollPaletteLtR*/};
int _animIndex=0;

ESP8266WebServer server(80);

void setup() {

  WiFi.persistent(false); // Do not persist Wifi settings; doesn't seem to work :/
  setup_wifi();  // must be done as early as possible; stops working if led is touched first :/
  
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
     DBG("Init OTA");

     pinMode(BUILTIN_LED_PIN, OUTPUT);
     setBuiltinLed(true);
     delay(1000);
     setBuiltinLed(false);

    ArduinoOTA.setHostname(host);
    /*
    ArduinoOTA.onStart([]() {
      setBuiltinLed(true);
      //DBG("Ota start");
    });
    
    ArduinoOTA.onEnd([]() {
      setBuiltinLed(false);
      //DBG("Ota end");
    });
    */
    ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
    ArduinoOTA.begin();

    DBG("OTA ready");    
  }

  server.on("/palette", handlePallete);
  server.on("/", []() {
    server.send(200, "text/plain", "Neopixel Edge Controller");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  
  animController.ChangeAnim(_demoAnims[_animIndex]); 
}

void loop() {
  if(ota_enabled == 1 || OTA_ON == 1){
    ArduinoOTA.handle();
  }

  server.handleClient();

  if(millis()>animMillis){
    animController.Animate();
    if(animController.AnimComplete){
      if(_animIndex == ANIM_COUNT - 1){
        _animIndex=0;
      }
      else{
        _animIndex++;
      }
      animController.ChangeAnim(_demoAnims[_animIndex]);
    }
    animMillis = millis()+3000;
  }
  
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

void handlePallete(){
  animController.ChangeAnim(AnimType::Static_VPallete);
  server.send(200, "text/plain", "Ok");
}

void handleNotFound() {
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
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
    //animStep=0;
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
