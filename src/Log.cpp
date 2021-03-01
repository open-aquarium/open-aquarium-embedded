#include "Log.h"

void Log::info(String message) {
  Serial.println(message);
}

void Log::debug(String message) {
  Serial.println(message);
}

void Log::error(String message) {
  Serial.println(message);
}
