#include "Device.h"

/*Device::Device() {
  
}*/

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

float Device::getDeviceInbuiltTemperature() {
  // https://playground.arduino.cc/Main/InternalTemperatureSensor/
  #if defined(__AVR_ATmega168A__) || defined(__ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;
  return t;
  #else
  return OA_MIN_FLOAT; //std::numeric_limits<float>::lowest();
  #endif
}
