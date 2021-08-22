#ifndef OA_CONVERTER_H
#define OA_CONVERTER_H

#include <Arduino.h>

// Date conversion
#include <stdio.h>
#include <time.h>

// #include <sstream>

class Converter {
  public:
    static String toString(float value);
    static String toString(uint8_t value);
    static String toString(uint32_t value);
    static String epochToISOString(unsigned long epoch);
    static struct tm epochToTmStruct(unsigned long epoch);
};

#endif
