#ifndef OA_REAL_TIME_CLOCK_H
#define OA_REAL_TIME_CLOCK_H

#include "RTClib.h"

#include <Arduino.h>

class RealTimeClock {
  private:
    RTC_DS3231 rtc;
  public:
    void setup();
    String nowAsISOString();
};

#endif
