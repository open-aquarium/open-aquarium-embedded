#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 3

unsigned long lastExecution = 0;
unsigned long lastBlink = 0;

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

unsigned long lastDisplay = 0;

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}

void displayWelcomeMessage() {
  display.clearDisplay();
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println();
  display.println();
  display.println();
  display.setTextSize(2);
  display.println(F("FUS RO DAH"));
  display.display();
}

void displayData(float humidity, float temperature, float heatIndex, int light) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.cp437(true);
  
  display.setTextSize(2);
  display.println(F("READINGS"));
  
  display.setTextSize(1);
  
  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F("%"));
  
  display.print(F("Temperature: "));
  display.print(temperature);
  display.write(167);
  display.println(F("C"));
  
  display.print(F("Heat index: "));
  display.println(heatIndex);
  display.write(167);
  display.println(F("C"));
  
  display.print(F("Light: "));
  if(light == 0) {
    display.println(F("ON"));
  } else {
    display.println(F("OFF"));
  }
  
  display.display();
}

void sleepDisplay() {
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void wakeDisplay() {
  display.ssd1306_command(SSD1306_DISPLAYON);
}
/**DISPLAY - END***************************************************************/

/**BUZZER - BEGIN**************************************************************/
#include "src/buzzer/Buzzer.h"
const byte PASSIVE_BUZZER = 12;
Buzzer buzz(PASSIVE_BUZZER);
/**BUZZER - END****************************************************************/

/**DEVICE - BEGIN**************************************************************/
#include "src/device/Device.h"
Device dev;
/**DEVICE - END****************************************************************/

/**SDCARD - BEGIN**************************************************************/
#include "src/sdCard/SDCard.h"
const byte SDCARD_CS_PIN = 53;
SDCard card(SDCARD_CS_PIN);
/**SDCARD - END****************************************************************/

/**RTC - BEGIN*****************************************************************/
#include "RTClib.h"
RTC_DS3231 rtc;
void setupRTC() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // CHANGE TO NTP sync
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

String nowAsISOString() {
  DateTime date = rtc.now();
  String result = F("");
  
  String month = F("");
  month += date.month();
  if(month.length() == 1) {
    month = "0" + month;
  }
  String day = F("");
  day += date.day();
  if(day.length() == 1) {
    day = "0" + day;
  }
  String hour = F("");
  hour += date.hour();
  if(hour.length() == 1) {
    hour = "0" + hour;
  }
  String minute = F("");
  minute += date.minute();
  if(minute.length() == 1) {
    minute = "0" + minute;
  }
  String second = F("");
  second += date.second();
  if(second.length() == 1) {
    second = "0" + second;
  }
  
  result += date.year();
  result += F("-");
  result += month;
  result += F("-");
  result += day;
  result += F("T");
  result += hour;
  result += F(":");
  result += minute;
  result += F(":");
  result += second;
  result += F("Z");
  return result;
}
/**RTC - END*******************************************************************/

int statusLedState = LOW;
float humidity = -32768.99;
float temperature = -32768.99;
float heatIndex = -32768.99;
int light = -32768;

void setup() {
  Serial.begin(9600);
//  while (!Serial);
//  Serial.println("setup()");
  buzz.playBeep();

  // setupNTP();
  setupRTC();

  card.printLog(F("Open Aquarium"));
  card.printLog(F("setup()"));
  card.printLog(nowAsISOString());
  
  setupDisplay();
  displayWelcomeMessage();
  delay(2000);
  displayFusRoDah();
  delay(5000);
  sleepDisplay();
  
  pinMode(LED_BUILTIN, OUTPUT);

  // DHT
  dht.begin();

  // LDR (photoresistor)
  pinMode(LDRPIN, INPUT);

  String cardType = F("SD Card type: ");
  card.printLog(cardType + card.cardType());

  String volumeType = F("Volume type: ");
  card.printLog(volumeType + card.volumeType());

  String clusterCount = F("Cluster count: ");
  card.printLog(clusterCount + card.clusterCount());

  String blocksPerCluster = F("Blocks: ");
  card.printLog(blocksPerCluster + card.blocksPerCluster());

  String volumeSize = F("Volume size: ");
  card.printLog(volumeSize + card.volumeSize());

  card.printDebug(F("example"));
  card.printError(F("example"));
}

void loop() {
  unsigned long currentExecution = millis();

  if(currentExecution - lastBlink >= 500) {
    lastBlink = currentExecution;
    if(statusLedState == LOW) {
      digitalWrite(LED_BUILTIN, HIGH);
      statusLedState = HIGH;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      statusLedState = LOW;
    }
  }
  
  if(currentExecution - lastExecution >= 5000) {
    lastExecution = currentExecution;
//    DateTime now = rtc.now();
    
    // DHT
    humidity = dht.readHumidity();
    temperature = dht.readTemperature(); // Celsius
  
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  
    heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  
    // LDR
    light = digitalRead(LDRPIN);

    
    String data = F("");
    data += F("Date: ");
    data += nowAsISOString();
    data += F(" RTC Temperature: ");
    data += rtc.getTemperature();
    data += F(" Humidity: ");
    data += humidity;
    data += F("%  Temperature: ");
    data += temperature;
    data += F("°C ");
    data += F("  Heat index: ");
    data += heatIndex;
    data += F("°C ");
    data += F("  Light: ");
    data += light;
    data += F("  SRAM: ");
    data += dev.getFreeSRAM();
    card.println(data);

    Serial.print(F("Date: "));
    Serial.print(nowAsISOString());

    Serial.print(" RTC Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.print("°C ");
    
    Serial.print(F(" Humidity: "));
    Serial.print(humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(temperature);
    Serial.print(F("°C "));
    Serial.print(F("  Heat index: "));
    Serial.print(heatIndex);
    Serial.print(F("°C "));
    Serial.print(F("  Light: "));
    Serial.print(light);
    Serial.print(F("  SRAM: "));
    Serial.println(dev.getFreeSRAM());
  }

  // Sleep?
  if(currentExecution - lastDisplay >= 5000) {
//    card.printDebug(F("sleep display"));
    sleepDisplay();
  }
  // Display it?
  if( (currentExecution - lastDisplay) >= 60000) {
//    card.printError(F("example display"));
    lastDisplay = currentExecution;
    wakeDisplay();
    displayData(humidity, temperature, heatIndex, light);
  }
}
