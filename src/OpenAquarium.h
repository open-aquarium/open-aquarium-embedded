#ifndef OA_OPEN_AQUARIUM_H
#define OA_OPEN_AQUARIUM_H

#include "log/Log.h"

#include "buzzer/Buzzer.h"

#include "device/Device.h"

#include "sdCard/SDCard.h"

#include "DHT.h"

#include <Arduino.h>

class OpenAquarium {
  private:
    const String VERSION = "0.1.0";
    
    Log log;
  
    bool error = false;
  
    bool ready = false;
    
    const uint16_t DISCOVERY_INTERVAL = 10000; // 86400000 1 day
    uint64_t lastDiscoveryEvent = 0;
    
    const uint16_t PERIODIC_INTERVAL = 5000; // 300000 5 min
    uint64_t lastPeriodicEvent = 0;
    
    //uint64_t lastBlink = 0;
  
    void deviceReady();
  
    void reportError(String message);
  
    void discoveryEvent();
    void periodicEvent();

    uint64_t lastActivityLedBlink = 0;
    bool activityLedOn = false;
    void setupActivityLed();
    void blinkActivityLed();

    // BUZZER
    const uint8_t PASSIVE_BUZZER_PIN = 12;
    Buzzer buzzer;

    // DEVICE
    Device device;

    // SD Card
    const uint8_t SDCARD_CS_PIN = 53;
    SDCard sdcard; //(SDCARD_CS_PIN);

    // SENSORS
    const float ERROR_FLOAT_READING = -32768.99;

    // DHT
    const uint8_t DHT22_PIN = 2;
    DHT dht;
    void setupDHT();

    // LIGHT
    const uint8_t LDR_PIN = 3;
    void setupLDR();
    int readLDRSensor();
  
  public:
    OpenAquarium() :
      sdcard(SDCARD_CS_PIN),
      dht(DHT22_PIN, DHT22),
      buzzer(PASSIVE_BUZZER_PIN) {};
    void setup();
    void loop();

    String getVersion();
    String getBuildVersion();
};

#endif
