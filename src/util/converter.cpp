#include "converter.h"

static String Converter::toString(float value) {
  // std::ostringstream ss;
  // ss << myFloat;
  // std::string s(ss.str());
  String result = F("");
  result += value;
  return result;
}

static String Converter::toString(uint8_t value) {
  String result = F("");
  result += value;
  return result;
}

static String Converter::toString(uint32_t value) {
  String result = F("");
  result += value;
  return result;
}

static String Converter::epochToISOString(unsigned long epoch) {
  // Source: https://www.epochconverter.com
  time_t rawtime = epoch;
  struct tm  ts;
  // char       buf[80];
  ts = *localtime(&rawtime);
  // strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
  // printf("%s\n", buf);
  String now = "";
  now += 1900 + ts.tm_year - 30; // TODO need to check this one
  now += "-";
  if(String(ts.tm_mon +1).length() == 1) {
    now += "0";
  }
  now += ts.tm_mon +1;
  now += "-";
  if(String(ts.tm_mday).length() == 1) {
    now += "0";
  }
  now += ts.tm_mday;
  now += "T";
  if(String(ts.tm_hour).length() == 1) {
    now += "0";
  }
  now += ts.tm_hour;
  now += ":";
  if(String(ts.tm_min).length() == 1) {
    now += "0";
  }
  now += ts.tm_min;
  now += ":";
  if(String(ts.tm_sec).length() == 1) {
    now += "0";
  }
  now += ts.tm_sec;
  now += "Z";
  return now;
}

static struct tm Converter::epochToTmStruct(unsigned long epoch) {
  time_t rawtime = epoch;
  struct tm  ts;
  // char       buf[80];
  ts = *localtime(&rawtime);
  ts.tm_year = ts.tm_year - 30; // TODO review
  return ts;
}
