// button debounce
// https://gist.github.com/erikpena/a01ace32f7bef4a5ec52
// attach interrupt
// http://esp8266.github.io/Arduino/versions/2.1.0-rc2/doc/reference.html

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>

#include "FastLED.h"

#include "LedConfig.h"
#include "WifiConfig.h"

#include "AnimController.h"

#include "LedUniverse.h"
#include "Dbg.h"

#define MODE_CHANGE_PIN 13    // 13 = D7
#define OPTION_CHANGE_PIN 12  // 12 = D6
uint8_t CtrlCurrentAnimation = 0;
uint8_t CtrlCurrentPalette = 0;

// --------------------------------------------------------
// Rygos cfg
#define ENABLE_BUTTON_CONTROL
#define WIFI_IS_AP
// --------------------------------------------------------


#define OTA_ON 0 // Set to 1 to Force OTA

#define BUILTIN_LED_PIN 2 // D4
#define OTA_ENABLE_PIN 14 // D5
uint8_t ota_enabled = 0;

#define WS_OK_NOCONTENT 204

LedUniverse ledUniverse;
AnimController animController(&ledUniverse);

uint32_t btnMillis = millis();
uint32_t animMillis = millis();
uint16_t animDelay = 1000;

ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup()
{
  //WiFi.mode(WIFI_OFF);
#ifdef DEBUG_ESP_PORT
  Serial.begin(115200);
#endif
  DBG("Start\n");
  //WiFi.persistent(false);
  setup_wifi(); // must be done as early as possible; stops working if led is touched first :/
  ledUniverse.Setup();
  pinMode(MODE_CHANGE_PIN, INPUT_PULLUP);
  pinMode(OPTION_CHANGE_PIN, INPUT_PULLUP);

  SPIFFS.begin();
  setup_WebServer();

  //setup_OTA();
  httpUpdater.setup(&webServer, "/upd");
}

void loop()
{
  // if (ota_enabled == 1 || OTA_ON == 1)
  // {
  //   ArduinoOTA.handle();
  // }

  webServer.handleClient();

  if (millis() > animMillis)
  {
    animController.Animate();    
    animMillis = millis() + animDelay;
  }

  #ifdef ENABLE_BUTTON_CONTROL
  EVERY_N_SECONDS(1){ monitorButtons(); }
  #endif
}

void webServer_serveFile()
{
  File file = SPIFFS.open(webServer.uri(), "r");
  webServer.streamFile(file, "text/html");
  file.close();
}

void webServer_serveIndex()
{
  File file = SPIFFS.open("Index.html", "r");
  webServer.streamFile(file, "text/html");
  file.close();
}

void webServer_updatePalette()    // TODO: delete previous? need to differentiate between builtin and custom?
{
  // uint8_t c1r1r = webServer.arg("clr1[r]").toInt();
  // uint8_t c1r1g = webServer.arg("clr1[g]").toInt();
  // uint8_t c1r1b = webServer.arg("clr1[b]").toInt();
  // DBG("----------- r:%u g:%u b:%u", c1r1r, c1r1g, c1r1b);
  animController.CurrentPalette = CRGBPalette16(
      CRGB(
          webServer.arg("clr1[r]").toInt(),
          webServer.arg("clr1[g]").toInt(),
          webServer.arg("clr1[b]").toInt()),
      CRGB(
          webServer.arg("clr2[r]").toInt(),
          webServer.arg("clr2[g]").toInt(),
          webServer.arg("clr2[b]").toInt()));
  webServer.send(WS_OK_NOCONTENT);
}

void webServer_updateDelay()
{
  uint8_t changeType = webServer.arg("ChangeType").toInt();
  switch (changeType)
    {
    case 0:
      animDelay = 0;
      break;
    case 1:
      if(animDelay>1000){
        animDelay -= 1000;
      }
      break;
    case 2:
      if(animDelay>100){
        animDelay -= 100;
      }
      break;
    case 3:
      animDelay += 100;
      break;
    case 4:
      animDelay += 1000;
      break;
    }
  webServer.send(WS_OK_NOCONTENT);
}

void webServer_updateAnimation()
{
  uint8_t animTypeInt = webServer.arg("AnimType").toInt();
  DBG("animTypeInt:%u\n", animTypeInt);
  AnimType animType = static_cast<AnimType>(animTypeInt); // TODO: do smth better than double cast
  int yy = static_cast<int>(animType);
  DBG("X%uX\n", yy);
  animController.ChangeAnim(animType);
  animDelay = animController.GetDefaultDelay();
  webServer.send(WS_OK_NOCONTENT);
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
#ifdef WIFI_IS_AP
  WiFi.softAP(ap_ssid, ap_password);
  DBG("Wifi AP");
#else
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
#endif
}

void setup_WebServer()
{
  webServer.on("/PaletteChange.html", webServer_serveFile);
  webServer.on("/PaletteChange.js", webServer_serveFile);
  webServer.on("/Index.html", webServer_serveFile);
  webServer.on("/Index.js", webServer_serveFile);
  webServer.on("/ApiService.js", webServer_serveFile);
  webServer.on("/", webServer_serveIndex); // TODO: doesn't work :/

  // ----- Api -----
  webServer.on("/api/palette", webServer_updatePalette);
  webServer.on("/api/animation", webServer_updateAnimation);
  webServer.on("/api/delay", webServer_updateDelay);

  webServer.onNotFound(handleNotFound);
  webServer.begin();
  DBG("HTTP server started");
}

#ifdef ENABLE_BUTTON_CONTROL
void monitorButtons(){
  DBG("Btn MON");
  if(digitalRead(MODE_CHANGE_PIN)==0){
    CtrlCurrentAnimation++;
    if(CtrlCurrentAnimation>7){ CtrlCurrentAnimation=0; }
    AnimType animType = static_cast<AnimType>(CtrlCurrentAnimation);
    animController.ChangeAnim(animType);
    DBG("Anim CH");
  }
  if(digitalRead(OPTION_CHANGE_PIN)==0){
    CtrlCurrentPalette++;
    if(CtrlCurrentPalette > 6){ CtrlCurrentPalette=0; }
    switch(CtrlCurrentPalette)
    {
      case 0:
        animController.CurrentPalette = RainbowColors_p;
        break;
      case 1:
        animController.CurrentPalette = CloudColors_p;
        break;
      case 2:
        animController.CurrentPalette = LavaColors_p;
        break;
      case 3:
        animController.CurrentPalette = OceanColors_p;
        break;
      case 4:
        animController.CurrentPalette = ForestColors_p;
        break;
      case 5:
        animController.CurrentPalette = PartyColors_p;
        break;
      case 6:
        animController.CurrentPalette = HeatColors_p;
        break;
    }
    DBG("Option CH");
  }
}
#endif

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

/*
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
    
    // ArduinoOTA.onStart([]() {
    //   setBuiltinLed(true);
    //   //DBG("Ota start");
    // });
    
    // ArduinoOTA.onEnd([]() {
    //   setBuiltinLed(false);
    //   //DBG("Ota end");
    // });
    
    ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
    ArduinoOTA.begin();

    DBG("OTA ready");
  }
}
*/
