#include "OpenAquariumRTOS2.h"

String OpenAquariumRTOS2::getVersion() {
  return this->VERSION;
}

void OpenAquariumRTOS2::setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println(F("<°)))><"));
  Serial.println(F("Open Aquarium"));

  this->buzzer.playBeep();

  // INFO MESSAGES
  Serial.println(F("------------------------------"));
  Serial.println(F("Device: Arduino Mega + ESP8266"));
  Serial.println(F("FreeRTOS kernel: 10.4.3-8"));
  Serial.println(F("Product: 0.2.0"));
  Serial.println(F("Hardware: 0.1.0"));
  Serial.println(F("------------------------------"));

  // ACTIVITY LED
  this->setupActivityLed();

  // DISPLAY
  this->display.setup();
  this->display.displayWelcomeMessage();
  delay(3000);
  this->display.displayFusRoDah();
  delay(1000);
  this->display.sleepDisplay();

  // REAL TIME CLOCK
  this->realTimeClock.setup();
  Serial.println(F("Started: Real Time Clock."));

  // WIFI
  this->wifi.setup();
  Serial.println(F("Started: Wi-Fi."));

  /*******************************************************************************
   * DEVICE SENSORS
   *******************************************************************************/
    
  /*******************************************************************************
   * ENVIRONMENT SENSORS
   *******************************************************************************/
  
  // DHT
  this->setupDHT();

  /*******************************************************************************
   * WATHER SENSORS
   *******************************************************************************/
  
  // FINALIZING
  Serial.println(F("System time: 2021-01-01H22:00:00.0Z"));
  Serial.println(F("Started: Basic System."));

  // TODO remove after DEBUG
  String now = this->wifi.getNTPDate();
  Serial.print("NTP: ");
  Serial.println(now);
}

void OpenAquariumRTOS2::setupActivityLed() {
  // Serial.println(F("OpenAquariumRTOS2::setupActivityLed()")); // TODO log
  pinMode(this->ACTIVITY_LED_PIN, OUTPUT);
  digitalWrite(this->ACTIVITY_LED_PIN, LOW);
  Serial.println(F("Started: Activity Led."));
}

void OpenAquariumRTOS2::blinkActivityLed() {
  if(this->activityLed) {
    this->activityLed = false;
    digitalWrite(this->ACTIVITY_LED_PIN, LOW);
  } else {
    this->activityLed = true;
    digitalWrite(this->ACTIVITY_LED_PIN, HIGH);
  }
}

void OpenAquariumRTOS2::produceDiscovery() {
  // Serial.println(F("OpenAquariumRTOS2::produceDiscovery"));
  String now = this->realTimeClock.nowAsISOString();
  now += " ";
  now += F("produceDiscovery()");
  Serial.println(now);
  // String ntp = this->wifi.getNTPDate();
  // Serial.print("NTP: ");
  // Serial.println(ntp);
}

void OpenAquariumRTOS2::producePeriodic() {
  // Serial.println(F("OpenAquariumRTOS2::producePeriodic()"));
  String now = this->realTimeClock.nowAsISOString();
  now += " ";
  now += F("producePeriodic()");
  Serial.println(now);
}

void OpenAquariumRTOS2::synchronizeRTC() {
  String now = this->realTimeClock.nowAsISOString();
  now += " ";
  now += F("synchronizeRTC()");
  Serial.println(now);
}

void OpenAquariumRTOS2::setupDHT() {
  this->dht.begin();
}
