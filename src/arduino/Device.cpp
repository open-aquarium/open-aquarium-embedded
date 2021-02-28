#include "Device.h"

int Device::getFreeSRAM() {
  #ifdef __arm__
  // should use uinstd.h to define sbrk but Due causes a conflict
  // extern "C" char* sbrk(int incr);
  "C" char* sbrk(int incr);
  #else  // __ARM__
  // extern char *__brkval;
  char *__brkval;
  #endif  // __arm__
  char top;
  #ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
  #elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
  #else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
  #endif  // __arm__
}

/*int Device::getFreeFlash() {
  return -1;
}

int Device::getFreeEEPROM() {
  return -1;
}*/
