#ifndef OA_BUZZER_H
#define OA_BUZZER_H

#include "Pitches.h"
#include "Melodies.h"
#include <Arduino.h>

class Buzzer {
  
  private:
    byte pin;
    
  public:
    // notes of the moledy followed by the duration.
    // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
    // !!negative numbers are used to represent dotted notes,
    // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
    int beepMelody[2] = { 2048, 16 };

    explicit Buzzer(byte pin);

    void play(int melody[], size_t melodySize, int tempo = 88);

    void playBeep();

    #ifdef OA_MELODIES_H
    void playPacman();

    void playImperialMarch();

    void playSuperMario();

    void playZelda();
    #endif
    
};

#endif
