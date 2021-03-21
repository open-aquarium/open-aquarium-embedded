#include "Display.h"

void Display::setup() {
  this->display.begin(SSD1306_SWITCHCAPVCC);
  this->display.clearDisplay();
}

void Display::displayWelcomeMessage() {
  this->display.clearDisplay();
  this->display.setTextSize(1);
  this->display.setTextColor(WHITE);
  this->display.setCursor(0,0);
  this->display.println();
  this->display.println();
  this->display.setTextSize(2);
  this->display.println(F("   Open"));
  this->display.println(F(" Aquarium "));
  this->display.display();
}

void Display::displayFusRoDah() {
  this->display.clearDisplay();
  this->display.setTextSize(1);
  this->display.setTextColor(WHITE);
  this->display.setCursor(0,0);
  this->display.println();
  this->display.println();
  this->display.println();
  this->display.setTextSize(2);
  this->display.println(F("FUS RO DAH"));
  this->display.display();
}

void Display::sleepDisplay() {
  this->display.clearDisplay();
  this->display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void Display::wakeDisplay() {
  this->display.ssd1306_command(SSD1306_DISPLAYON);
}
