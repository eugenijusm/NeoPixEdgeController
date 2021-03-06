#ifndef LedConfig_h
#define LedConfig_h

#define RECTANGULAR_CONFIG  // Comment out for U config

#define FASTLED_ESP8266_D1_PIN_ORDER // https://github.com/FastLED/FastLED/wiki/ESP8266-notes
// #define FASTLED_ESP8266_RAW_PIN_ORDER

#ifdef RECTANGULAR_CONFIG
    #define NUM_LEDS_VERTICAL 60
    #define NUM_LEDS_HORIZONTAL 72
    #define NUM_LEDS_VH (NUM_LEDS_VERTICAL + NUM_LEDS_HORIZONTAL)
    #define NUM_LEDS_VHV (NUM_LEDS_VERTICAL + NUM_LEDS_HORIZONTAL + NUM_LEDS_VERTICAL)
    #define NUM_LEDS_TOTAL (NUM_LEDS_VERTICAL + NUM_LEDS_VERTICAL + NUM_LEDS_HORIZONTAL + NUM_LEDS_HORIZONTAL) // 264
    #define TOP_LEDS_PIN D8

#else
    #define NUM_LEDS_VERTICAL 60
    #define NUM_LEDS_HORIZONTAL 96
    #define NUM_LEDS_TOTAL (NUM_LEDS_VERTICAL + NUM_LEDS_VERTICAL + NUM_LEDS_HORIZONTAL) // 216
#endif

#define NUM_LEDS_VH (NUM_LEDS_VERTICAL + NUM_LEDS_HORIZONTAL) // 156 | 132
#define NUM_LEDS_HH (NUM_LEDS_HORIZONTAL / 2)
#define NUM_LEDS_HALF (NUM_LEDS_TOTAL / 2)

#define MASTER_BRIGHTNESS 100

#define LEFT_LEDS_PIN D1   // D1 = gpio 5 = black
#define RIGHT_LEDS_PIN D2  // D2 = gpio 4 = red
#define BOTTOM_LEDS_PIN D3 // D3 = gpio 0 = green

#endif
