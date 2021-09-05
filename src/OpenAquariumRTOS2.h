#ifndef OA_OPEN_AQUARIUM_RTOS2_H
#define OA_OPEN_AQUARIUM_RTOS2_H

#include <Arduino.h>
#include "buzzer/Buzzer.h"
#include "display/Display.h"
#include "rtc/RealTimeClock.h"
#include "wifi/CustomWiFi.h"
#include "sdCard/SDCard.h"
#include "device/Device.h"

// #include "events/blocks/HeaderBlock.h"
#include "events/blocks/DeviceBlock.h"
// #include "events/blocks/RollCallDataBlock.h"
// #include "events/blocks/RollCallSensor.h"

// ENVIRONMENT SENSORS
#include "DHT.h"
#include <Adafruit_BMP280.h>

// Samples
#include "events/blocks/DeviceSampleBlock.h"
#include "events/blocks/EnvironmentSampleBlock.h"
#include "events/blocks/WaterSampleBlock.h"

// Util
#include "util/converter.h"
#include <ArduinoJson.h>

/**
 * RTOS implementation of the Open Aquarium application
 */
class OpenAquariumRTOS2 {
  private:
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
    const long bmp280Interval = 5000;
    unsigned long previousBmp280Millis = 0;
    const long rtcSynchronizationInterval = 3600000; // 1h
    unsigned long previousRtcSynchronizationMillis = 0;
    const long wifiReconnectionInterval = 10000; // 5s
    unsigned long previousWifiReconnectionMillis = 0;
    const long deviceInterval = 10000;
    unsigned long previousDeviceMillis = 0;
    const long testInterval = 2000;
    unsigned long previousTestMillis = 0;

    // float roomTemperature1 = -1000;
    // float relativeHumidity = -1000;

    DeviceBlock deviceBlock;

    // SAMPLES
    DeviceSampleBlock deviceSample;
    EnvironmentSampleBlock environmentSample;
    WaterSampleBlock waterSample;
    const float ABSOLUTE_ZERO = -273.15;
    const unsigned long INTERVAL_24H = 86400;
  public:
    const String SOFTWARE_VERSION = "0.3.0";
    const String HARDWARE_VERSION = "0.1.0";
    const String SERIAL_NUMBER = "9b1d53c5-bb7f-4b5e-a5e7-6490de1ceaa6";

    const uint16_t DISCOVERY_INTERVAL = 10000; // 86400000 1 day
    const uint16_t PERIODIC_INTERVAL = 5000; // 300000 5 min
    const uint8_t ACTIVITY_LED_INTERVAL = 200;
    const uint16_t NTP_INTERVAL = 60000;
    const uint16_t WIFI_RECONNECTION_INTERVAL = 60000;

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

    // SD Card
    const uint8_t SDCARD_CS_PIN = 53;
    SDCard sdcard; //(SDCARD_CS_PIN);

    // DEVICE
    Device device;

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

    // BMP280
    Adafruit_BMP280 bmp;
    void setupBMP();

    /*******************************************************************************
     * WATHER SENSORS
     *******************************************************************************/

    /**
     * Initializes all dependencies
     */
    OpenAquariumRTOS2() :
      buzzer(PASSIVE_BUZZER_PIN),
      sdcard(SDCARD_CS_PIN),
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

    void initializeDeviceBlock();
    void initializeSamples();
    void sensorsCalibration();

    void activityLedTask(unsigned long currentMillis);
    void discoveryTask(unsigned long currentMillis);
    void periodicTask(unsigned long currentMillis);
    void dht22Task(unsigned long currentMillis);
    void bmp280Task(unsigned long currentMillis);
    void rtcSynchronizationTask(unsigned long currentMillis);
    void wifiReconnectionTask(unsigned long currentMillis);
    void deviceTask(unsigned long currentMillis);
};

#endif
