#ifndef Dbg_h
#define Dbg_h

#ifdef DEBUG_ESP_PORT
  #define DBG_INDEXERS_VERBOSE
  #define DBG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
  //#define DBG_VAL(txt,val) DEBUG_ESP_PORT.printf(txt+":%u\n", val)  // TMP define???

#else
  #define DBG(...)
#endif

#endif
