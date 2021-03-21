#ifndef OA_DISPLAY_H
#define OA_DISPLAY_H

#include <SPI.h>

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#include <Arduino.h>

class Display {
  private:
    // OLED dimensions
    const uint8_t OLED_HEIGHT = 64;
    const uint8_t OLED_WIDTH = 128;
    // OLED SPI interface
    const uint8_t OLED_CLK_PIN = 22; // D0
    const uint8_t OLED_MOSI_PIN = 23; // D1
    const uint8_t OLED_RESET_PIN = 24; // Reset
    const uint8_t OLED_DC_PIN = 25; // ?
    const uint8_t OLED_CS_PIN = 26; // ?

    Adafruit_SSD1306 display;
  public:
    Display() :
      display(OLED_WIDTH, OLED_HEIGHT, OLED_MOSI_PIN, OLED_CLK_PIN, OLED_DC_PIN, OLED_RESET_PIN, OLED_CS_PIN) {};
    void setup();
    void displayWelcomeMessage();
    void displayFusRoDah();
    void sleepDisplay();
    void wakeDisplay();
};

#endif
