#include "RealTimeClock.h"

/**
 * Setup the RTC. Call this method before using RTC.
 */
void RealTimeClock::setup() {
  // Serial.println(F("RealTimeClock::setup()"));
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // Set default date in case the clock lost power
  if (rtc.lostPower()) {
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Compilation date/time
    rtc.adjust(DateTime(2000, 1, 1, 0, 0, 0)); // arbitrary time
  }
}

/**
 * Adjust the clock to a given date.
 */
void RealTimeClock::adjust(DateTime now) {
  this->rtc.adjust(now);
}

/**
 * Get the current time in ISO format.
 * TODO refactor
 */
String RealTimeClock::nowAsISOString() {
  DateTime date = rtc.now();
  String result = F("");
  
  String month = F("");
  month += date.month();
  if(month.length() == 1) {
    month = "0" + month;
  }
  String day = F("");
  day += date.day();
  if(day.length() == 1) {
    day = "0" + day;
  }
  String hour = F("");
  hour += date.hour();
  if(hour.length() == 1) {
    hour = "0" + hour;
  }
  String minute = F("");
  minute += date.minute();
  if(minute.length() == 1) {
    minute = "0" + minute;
  }
  String second = F("");
  second += date.second();
  if(second.length() == 1) {
    second = "0" + second;
  }
  
  result += date.year();
  result += F("-");
  result += month;
  result += F("-");
  result += day;
  result += F("T");
  result += hour;
  result += F(":");
  result += minute;
  result += F(":");
  result += second;
  result += F("Z");
  return result;
}
