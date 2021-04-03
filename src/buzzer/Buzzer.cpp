#include "Buzzer.h"

Buzzer::Buzzer() {
  this->pin = 12;
}

Buzzer::Buzzer(int8_t pin) {
  this->pin = pin;
}

void Buzzer::play(int melody[], size_t melodySize, int tempo) {
  // Serial.println(F("Buzzer::play()"));
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
  // Serial.println(F("Buzzer::playBeep()"));
  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  int beepMelody[2] = { 2048, 16 };
  int size = sizeof(beepMelody) / sizeof(beepMelody[0]);
  this->play(beepMelody, size);
  // Stop the soft sound that the module keeps playing
  digitalWrite(this->pin, HIGH);
}
