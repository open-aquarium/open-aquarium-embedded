#ifndef OA_OPEN_AQUARIUM_RTOS_H
#define OA_OPEN_AQUARIUM_RTOS_H

#include <Arduino.h>

// DEVICE

#include "device/Device.h"

#include "buzzer/Buzzer.h"

#include "rtc/RealTimeClock.h"

#include "WiFiEsp.h"
#include "WiFiEspUdp.h"

#include "display/Display.h"

#include "sdCard/SDCard.h"

// ENVIRONMENT SENSORS

#include "DHT.h"

#include <Adafruit_BMP280.h>

// WATER SENSORS

/**
 * RTOS implementation of the Open Aquarium application
 */
class OpenAquariumRTOS {
  private:
    const String VERSION = "0.3.0";
    // BOOT
    bool bootBasicSystem = false;
  public:
    const uint16_t DISCOVERY_INTERVAL = 10000; // 86400000 1 day
    const uint16_t PERIODIC_INTERVAL = 5000; // 300000 5 min
    const uint8_t ACTIVITY_LED_INTERVAL = 200;

    String getVersion();
    bool isSystemReady();

    // DEVICE helper
    Device device;

    // SD CARD
    const uint8_t SDCARD_CS_PIN = 53;
    SDCard sdcard; //(SDCARD_CS_PIN);

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
    void sendNTPpacket(char *ntpSrv, byte packetBuffer[], int NTP_PACKET_SIZE, WiFiEspUDP Udp);
    void getNTPDate();

    // ACTIVITY LED
    const uint8_t ACTIVITY_LED_PIN = 11;

    // Display OLED
    Display display;

    // DHT
    const uint8_t DHT22_PIN = 2;
    DHT dht;
    void setupDHT();

    // LDR
    const uint8_t LDR_PIN = 3;
    void setupLDR();
    int readLDRSensor();

    // SOUND SENSOR
    const uint8_t SOUND_SENSOR_PIN = 4;
    void setupSoundSensor();
    int readSoundSensor();

    // BMP280
    Adafruit_BMP280 bmp;
    void setupBMP();

    /**
     * Initializes all dependencies
     */
    OpenAquariumRTOS() :
      sdcard(SDCARD_CS_PIN),
      buzzer(PASSIVE_BUZZER_PIN),
      dht(DHT22_PIN, DHT22) {};
    
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
