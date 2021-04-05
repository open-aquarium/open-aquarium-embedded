#include "Log.h"

void Log::error(String message) {
  if(this->mode >= 1) {
    Serial.println(message);
  }
}

void Log::info(String message) {
  if(this->mode >= 2) {
    Serial.println(message);
  }
}

void Log::debug(String message) {
  if(this->mode >= 3) {
    Serial.println(message);
  }
}
