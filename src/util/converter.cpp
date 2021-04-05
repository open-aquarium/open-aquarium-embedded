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
