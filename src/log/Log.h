#ifndef OA_LOG_H
#define OA_LOG_H

#include <Arduino.h>

class Log {
  private:
    int mode = 2; // 0=disabled, 1=error, 2=info, 3=debug
  public:
    void info(String message);
    void debug(String message);
    void error(String message);
};

#endif
