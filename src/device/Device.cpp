#include "Device.h"

/*Device::Device() {
  
}*/

uint16_t Device::getFreeSRAM() {
  return (int) SP - (int) (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

uint16_t Device::getSRAMUsage() {
  return this->getTotalSRAM() - this->getFreeSRAM();
}

uint16_t Device::getTotalSRAM() {
//  return 8192; // 8 KB
  return (int) RAMEND - (int) &__data_start;
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
  return -273.15; // Absolute Zero Celcius //OA_MIN_FLOAT; //std::numeric_limits<float>::lowest();
  #endif
}

String Device::getCPU() {
  return F("ATmega2560");
}

uint8_t Device::getCPUSpeed() {
  return 16; // MHz
}

uint8_t Device::getAnalogIO() {
  return 16;
}

uint8_t Device::getDigitalIO() {
  return 54;
}

uint8_t Device::getDigitalPWM() {
  return 15;
}

uint16_t Device::getTotalEEPROM() {
  return 4096; // 4KB
}

uint32_t Device::getTotalFlash() {
  return 33554432; // 32MB
}
