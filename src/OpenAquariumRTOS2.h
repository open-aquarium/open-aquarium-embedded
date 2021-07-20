#ifndef OA_OPEN_AQUARIUM_RTOS2_H
#define OA_OPEN_AQUARIUM_RTOS2_H

#include <Arduino.h>
#include "buzzer/Buzzer.h"
#include "display/Display.h"
#include "rtc/RealTimeClock.h"
#include "wifi/CustomWiFi.h"

// ENVIRONMENT SENSORS
#include "DHT.h"

/**
 * RTOS implementation of the Open Aquarium application
 */
class OpenAquariumRTOS2 {
  private:
    const String VERSION = "0.3.0";
    bool activityLed = false;
  public:
    const uint16_t DISCOVERY_INTERVAL = 10000; // 86400000 1 day
    const uint16_t PERIODIC_INTERVAL = 5000; // 300000 5 min
    const uint8_t ACTIVITY_LED_INTERVAL = 200;
    const uint16_t NTP_INTERVAL = 60000;
    const uint16_t WIFI_RECONNECTION_INTERVAL = 60000;

    /**
     * Get the Software version.
     **/
    String getVersion();

    // ACTIVITY LED
    const uint8_t ACTIVITY_LED_PIN = 13;

    // BUZZER
    const uint8_t PASSIVE_BUZZER_PIN = 12;
    Buzzer buzzer;

    // Display OLED
    Display display;

    // REAL TIME CLOCK
    RealTimeClock realTimeClock;

    // WIFI
    CustomWiFi wifi;

    /*******************************************************************************
     * DEVICE SENSORS
     *******************************************************************************/
    
    /*******************************************************************************
     * ENVIRONMENT SENSORS
     *******************************************************************************/
    
    // DHT
    const uint8_t DHT22_PIN = 2;
    DHT dht;
    void setupDHT();

    /*******************************************************************************
     * WATHER SENSORS
     *******************************************************************************/

    /**
     * Initializes all dependencies
     */
    OpenAquariumRTOS2() :
      buzzer(PASSIVE_BUZZER_PIN),
      dht(DHT22_PIN, DHT22)
      {};

    /**
     * Setup the device
     **/
    void setup();

    /**
     * Setup the activity led.
     * It blinks while the device is working correctly
     */
    void setupActivityLed();

    /**
     * Turn the activity led on/off
     */
    void blinkActivityLed();

    /**
     * Produces a DISCOVERY event
     **/
    void produceDiscovery();

    /**
     * Produces a PERIODIC event
     **/
    void producePeriodic();

    /**
     * Synchronizes RTC with an NTP server
     **/
    void synchronizeRTC();

};

#endif
