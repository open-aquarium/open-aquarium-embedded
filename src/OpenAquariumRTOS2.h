#ifndef OA_OPEN_AQUARIUM_RTOS2_H
#define OA_OPEN_AQUARIUM_RTOS2_H

#include <Arduino.h>
#include "buzzer/Buzzer.h"
#include "display/Display.h"
#include "rtc/RealTimeClock.h"
#include "wifi/CustomWiFi.h"

// #include "events/blocks/HeaderBlock.h"
// #include "events/blocks/DeviceBlock.h"
// #include "events/blocks/RollCallDataBlock.h"
// #include "events/blocks/RollCallSensor.h"
// #include "events/blocks/DeviceSampleBlock.h"
#include "events/blocks/EnvironmentSampleBlock.h"
// #include "events/blocks/WaterSampleBlock.h"

// ENVIRONMENT SENSORS
#include "DHT.h"

// Samples
#include "events/blocks/DeviceSampleBlock.h"
#include "events/blocks/EnvironmentSampleBlock.h"
#include "events/blocks/WaterSampleBlock.h"

// Util
#include "util/converter.h"

/**
 * RTOS implementation of the Open Aquarium application
 */
class OpenAquariumRTOS2 {
  private:
    const String VERSION = "0.3.0";
    const bool DEBUG = true;
    const bool DEBUG_WIFI = false;
    
    bool activityLed = false;
    const long activityLedInterval = 300;
    unsigned long previousActivityLedMillis = 0;

    const long discoveryInterval = 600000; // 10000 (10s) // 600000 (10 min)
    unsigned long previousDiscoveryMillis = 0;
    const long periodicInterval = 120000; // 5000 (5s) // 300000 (5 min) // 120000 (2 min)
    unsigned long previousPeriodicMillis = 0;
    const long dht22Interval = 5000;
    unsigned long previousDht22Millis = 0;
    const long rtcSynchronizationInterval = 3600000; // 1h
    unsigned long previousRtcSynchronizationMillis = 0;
    const long testInterval = 2000;
    unsigned long previousTestMillis = 0;

    float roomTemperature1 = -1000;
    float relativeHumidity = -1000;

    // SAMPLES
    DeviceSampleBlock deviceSample;
    EnvironmentSampleBlock environmentSample;
    WaterSampleBlock waterSample;
    const float ABSOLUTE_ZERO = -273.15;
    const unsigned long INTERVAL_24H = 86400;
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

    void loop();

    void initializeSamples();
    void sensorsCalibration();

    void activityLedTask(unsigned long currentMillis);
    void discoveryTask(unsigned long currentMillis);
    void periodicTask(unsigned long currentMillis);
    void dht22Task(unsigned long currentMillis);
    void rtcSynchronizationTask(unsigned long currentMillis);
};

#endif