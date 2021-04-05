#ifndef OA_CONVERTER_H
#define OA_CONVERTER_H

#include <Arduino.h>

// #include <sstream>

class Converter {
  public:
    static String toString(float value);
    static String toString(uint8_t value);
    static String toString(uint32_t value);
};

#endif
