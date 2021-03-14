#ifndef OA_BUZZER_H
#define OA_BUZZER_H

#include <Arduino.h>

class Buzzer {
  
  private:
    int8_t pin;
    Buzzer();

  public:
    
    Buzzer(int8_t pin);

    void play(int melody[], size_t melodySize, int tempo = 88);

    void playBeep();

};

#endif
