#ifndef OA_OPEN_AQUARIUM_H
#define OA_OPEN_AQUARIUM_H
#include "Log.h"
#include <Arduino.h>

class OpenAquarium {
  private:
    Log log;
  public:
    void setup();
    void loop();
};

#endif
