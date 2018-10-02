// button debounce
// https://gist.github.com/erikpena/a01ace32f7bef4a5ec52
// attach interrupt
// http://esp8266.github.io/Arduino/versions/2.1.0-rc2/doc/reference.html

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>

#include "FastLED.h"

#include "LedConfig.h"
#include "WifiConfig.h"

#include "AnimController.h"

#include "LedUniverse.h"
#include "Dbg.h"

#define OTA_ON 0 // Set to 1 to Force OTA

#define BUILTIN_LED_PIN 2 // D4
#define OTA_ENABLE_PIN 14 // D5
uint8_t ota_enabled = 0;
#define MODE_CHANGE_PIN 13 // 13 = D7; 12 = D6

LedUniverse ledUniverse;
AnimController animController(&ledUniverse);

uint32_t btnMillis = millis();
uint32_t animMillis = millis();

#define ANIM_COUNT 2
AnimType _demoAnims[] = {AnimType::TestRGB, AnimType::PaletteV, /*AnimType::RunningPixel, AnimType::SolidColorCycle, AnimType::ScrollPaletteLtR*/};
uint8_t _animIndex = 0;

ESP8266WebServer webServer(80);

void setup()
{
  //WiFi.persistent(false);
  setup_wifi(); // must be done as early as possible; stops working if led is touched first :/
#ifdef DEBUG_ESP_PORT
  Serial.begin(115200);
#endif
  DBG("Start\n");
  ledUniverse.Setup();
  // pinMode(MODE_CHANGE_PIN, INPUT_PULLUP);
  setup_OTA();
  SPIFFS.begin();
  setup_WebServer();
  animController.ChangeAnim(_demoAnims[_animIndex]);
}

void loop()
{
  if (ota_enabled == 1 || OTA_ON == 1)
  {
    ArduinoOTA.handle();
  }

  webServer.handleClient();

  if (!animController.IsStaticView && millis() > animMillis)
  {
    animController.Animate();
    if (animController.AnimComplete)
    {
      if (_animIndex == ANIM_COUNT - 1)
      {
        _animIndex = 0;
      }
      else
      {
        _animIndex++;
      }
      animController.ChangeAnim(_demoAnims[_animIndex]);
    }
    animMillis = millis() + 300;
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

void webServer_serveFile()
{
  File file = SPIFFS.open(webServer.uri(), "r");
  webServer.streamFile(file, "text/html");
  file.close();
}

void handlePallete()
{
  animController.ChangeAnim(AnimType::Static_PalleteV);
  animController.View_PalleteV(); // NOTE: maybe it's better to keep animation loop and play around with isAnimationComplete flag; instead of invoking this directly?
  webServer.send(200, "text/plain", "Ok");
}

void handlePalleteH()
{
  animController.ChangeAnim(AnimType::Static_PalleteH);
  String qPalleteCode = webServer.arg("pallete");
  switch (qPalleteCode.toInt())
  {
  case 1:
    animController.CurrentPalette = RainbowColors_p;
    break;
  case 2:
    animController.CurrentPalette = CloudColors_p;
    break;
  case 3:
    animController.CurrentPalette = LavaColors_p;
    break;
  case 4:
    animController.CurrentPalette = OceanColors_p;
    break;
  case 5:
    animController.CurrentPalette = ForestColors_p;
    break;
  case 6:
    animController.CurrentPalette = PartyColors_p;
    break;
  case 7:
    animController.CurrentPalette = HeatColors_p;
    break;
  default:
    break;
    //animController.CurrentPalette = RainbowStripeColors_p;
  }
  animController.View_PalleteH();
  webServer.send(200, "text/plain", "Ok");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";
  for (uint8_t i = 0; i < webServer.args(); i++)
  {
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }
  webServer.send(404, "text/plain", message);
}

void setup_wifi()
{
  DBG("Wifi Setup");

  // Wifi client mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    DBG(".");
    delay(1000);
  }

  DBG("Wifi Ok");
}

void setup_WebServer()
{
  webServer.on("/PaletteChange.html", webServer_serveFile);
  webServer.on("/PaletteChange.js", webServer_serveFile);
  webServer.on("/", []() {
    webServer.send(200, "text/plain", "Neopixel Edge Controller");
  });

  // ----- Api -----
  webServer.on("/api/palette", webServer_updatePalette);
  webServer.on("/palette", handlePallete);
  webServer.on("/paletteH", handlePalleteH);

  webServer.onNotFound(handleNotFound);
  webServer.begin();
  DBG("HTTP server started");
}

void webServer_updatePalette()
{
  uint8_t c1r1r = webServer.arg("clr1[r]").toInt();
  uint8_t c1r1g = webServer.arg("clr1[g]").toInt();
  uint8_t c1r1b = webServer.arg("clr1[b]").toInt();
  DBG("----------- r:%u g:%u b:%u", c1r1r, c1r1g, c1r1b);
  CRGB color1 = CRGB(c1r1r, c1r1g, c1r1b);
  animController.CurrentPalette = CRGBPalette16(color1, CRGB::Yellow);
}

/*
void monitorModeButton(){
  DBG("Btn monitor");
  if(digitalRead(MODE_CHANGE_PIN)==0){
    DBG("Btn pressed");
  }
}
*/

void setBuiltinLed(bool turnedOn)
{
  if (turnedOn)
  {
    digitalWrite(BUILTIN_LED_PIN, LOW); // led inverted => led on
  }
  else
  {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
  }
}

void setup_OTA()
{
  pinMode(OTA_ENABLE_PIN, INPUT_PULLUP);
  if (digitalRead(OTA_ENABLE_PIN) == 0)
  {
    ota_enabled = 1;
  }

  if (ota_enabled == 1 || OTA_ON == 1)
  {
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
}
