#ifndef OA_LOG_H
#define OA_LOG_H
#include <Arduino.h>

class Log {
  public:
    void info(String message);
    void debug(String message);
    void error(String message);
};

#endif
