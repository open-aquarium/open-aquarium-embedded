#include "RealTimeClock.h"

void RealTimeClock::setup() {
  Serial.println(F("RealTimeClock::setup()"));
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // CHANGE TO NTP sync
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

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
