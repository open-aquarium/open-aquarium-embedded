#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 3

/**DISPLAY - BEGIN*************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED dimensions
#define OLED_HEIGHT  64
#define OLED_WIDTH  128
// OLED SPI interface
#define OLED_CLK   22 // D0
#define OLED_MOSI  23 // D1
#define OLED_RESET 24 // Reset
#define OLED_DC    25 // ?
#define OLED_CS    26 // ?

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}

void displayWelcomeMessage() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println();
  display.println();
  display.setTextSize(2);
  display.println(F("   Open"));
  display.println(F(" Aquarium "));
  display.display();
}

void displayFusRoDah() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println();
  display.println();
  display.println();
  display.setTextSize(2);
  display.println(F(" FUS RO DAH!"));
  display.display();
}
/**DISPLAY - END***************************************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Dovahkiin!"));

  setupDisplay();
  displayWelcomeMessage();

  pinMode(LED_BUILTIN, OUTPUT);

  // DHT
  dht.begin();

  // LDR (photoresistor)
  pinMode(LDRPIN, INPUT);
}

void loop() {
  Serial.println(F("FUS RO DAH!"));
  digitalWrite(LED_BUILTIN, LOW);
  displayFusRoDah();
  delay(5000);
  display.clearDisplay();

  digitalWrite(LED_BUILTIN, HIGH);

  // DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  // LDR
  int light = digitalRead(LDRPIN);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(F("  Light: "));
  Serial.println(light);
}
