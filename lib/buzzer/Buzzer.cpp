#include "Buzzer.h"

explicit Buzzer::Buzzer(byte pin) {
  this->pin = pin;
}

void Buzzer::play(int melody[], size_t melodySize, int tempo) {
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = melodySize / 2;
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(this->pin, melody[thisNote], noteDuration*0.9);
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    // stop the waveform generation before the next note.
    noTone(this->pin);
  }

}

void Buzzer::playBeep() {
  int size = sizeof(this->beepMelody) / sizeof(this->beepMelody[0]);
  this->play(this->beepMelody, size);
}

#ifdef OA_MELODIES_H
void Buzzer::playPacman() {
  int size = sizeof(pacmanMelody) / sizeof(pacmanMelody[0]);
  this->play(pacmanMelody, size, 105);
}

void Buzzer::playImperialMarch() {
  int size = sizeof(imperialMarchMelody) / sizeof(imperialMarchMelody[0]);
  this->play(imperialMarchMelody, size, 120);
}

void Buzzer::playSuperMario() {
  int size = sizeof(superMarioMelody) / sizeof(superMarioMelody[0]);
  this->play(superMarioMelody, size, 200);
}

void Buzzer::playZelda() {
  int size = sizeof(zeldaMelody) / sizeof(zeldaMelody[0]);
  this->play(zeldaMelody, size, 88);
}
#endif
