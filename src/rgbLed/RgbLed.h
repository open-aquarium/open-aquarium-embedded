#ifndef OA_RGBLED_H
#define OA_RGBLED_H

#include <Arduino.h>

class RgbLed {
  
  private:
    byte redPin;
    byte greenPin;
    byte bluePin;
    
  public:
    // Setup pin LED and call init()
    RgbLed(byte redPin, byte greenPin, byte bluePin);

    // Setup the pin led as OUTPUT
    // and power off the LED - default state
    void init();
    
    void turnRed();

    void turnGreen();

    void turnBlue();

    void turnYellow();

    void turnMagenta();

    void turnCyan();

    void turnWhite();

    // Power off the LED
    void turnOff();
};

#endif