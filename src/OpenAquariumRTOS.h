#ifndef OA_OPEN_AQUARIUM_RTOS_H
#define OA_OPEN_AQUARIUM_RTOS_H

#include <Arduino.h>

#include "buzzer/Buzzer.h"

#include "rtc/RealTimeClock.h"

#include "WiFiEsp.h"

/**
 * RTOS implementation of the Open Aquarium application
 */
class OpenAquariumRTOS {
  private:
    // BOOT
    bool bootBasicSystem = false;
    bool bootSystemReady = false;
    // const uint16_t DISCOVERY_INTERVAL = 10000; // 86400000 1 day
    // const uint16_t PERIODIC_INTERVAL = 5000; // 300000 5 min
    const uint8_t ACTIVITY_LED_PIN = 11;
  public:
    const uint16_t DISCOVERY_INTERVAL = 10000; // 86400000 1 day
    const uint16_t PERIODIC_INTERVAL = 5000; // 300000 5 min
    const uint8_t ACTIVITY_LED_INTERVAL = 200;

    bool isSystemReady();

    // BUZZER
    const uint8_t PASSIVE_BUZZER_PIN = 12;
    Buzzer buzzer;

    // REAL TIME CLOCK
    RealTimeClock realTimeClock;

    // WIFI
    void setupWiFi();
    void connectWiFi();
    String getFirmwareVersion();
    String getWiFiStatus();
    String getWiFiSSID();
    String getMacAddress();
    String getIP();
    String getSubnetMask();
    String getGatewayIP();
    int32_t getRSSI();


    /**
     * Initializes all dependencies
     */
    OpenAquariumRTOS() :
      buzzer(PASSIVE_BUZZER_PIN) {};
    
    /**
     * Setup the activity led.
     * It blinks while the device is working correctly
     */
    void setupActivityLed();

    /**
     * Turn the activity led on/off
     */
    void blinkActivityLed(boolean on);

    /**
     * Enable/disable debug messages
     */
    bool debug = false;

    /**
     * Enable/disable production mode
     * When enable, all events will trigger faster
     */
    bool productionMode = true;

    /**
     * Setup Open Aquarium application from an Arduino perspective
     */
    void setup();
};

#endif
