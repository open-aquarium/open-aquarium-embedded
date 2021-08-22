#ifndef OA_OPEN_AQUARIUM_H
#define OA_OPEN_AQUARIUM_H

#include "util/limits.h"

#include "log/Log.h"

#include "buzzer/Buzzer.h"

#include "device/Device.h"

#include "sdCard/SDCard.h"

#include "rtc/RealTimeClock.h"

#include "display/Display.h"

#include "DHT.h"

#include <Adafruit_BMP280.h>

//#include "events/EventBuilder.h"

#include "events/EventBuilder.h"
#include "events/Discovery.h"
#include "events/Periodic.h"
#include "events/blocks/HeaderBlock.h"
#include "events/blocks/DeviceBlock.h"
#include "events/blocks/RollCallDataBlock.h"
#include "events/blocks/RollCallSensor.h"
#include "events/blocks/DeviceSampleBlock.h"
#include "events/blocks/EnvironmentSampleBlock.h"
#include "events/blocks/WaterSampleBlock.h"

#include "WiFiEsp.h"
#include "WiFiEspUdp.h"
//#include <NTPClient.h>

#include <Arduino.h>

class OpenAquarium {
  private:
    const String VERSION = "0.2.0";

    // BOOT
    bool bootBasicSystem = false;
    bool bootOS = false;
    
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

    // RTC
    RealTimeClock realTimeClock;

    // Display OLED
    Display display;

    // SENSORS
    
    // DHT
    const uint8_t DHT22_PIN = 2;
    DHT dht;
    void setupDHT();

    // LIGHT
    const uint8_t LDR_PIN = 3;
    void setupLDR();
    int readLDRSensor();

    // SOUND
    const uint8_t SOUND_SENSOR_PIN = 4;
    void setupSoundSensor();
    int readSoundSensor();

    // BMP280
    Adafruit_BMP280 bmp;
    void setupBMP();

    // BUILDER - TODO move to a new builder class
    HeaderBlock buildHeaderBlock(int eventType);
    DeviceBlock buildDeviceBlock();
    RollCallDataBlock buildRollCallDataBlock();
    DeviceSampleBlock buildDeviceSampleBlock();
    EnvironmentSampleBlock buildEnvironmentSampleBlock();
    WaterSampleBlock buildWaterSampleBlock();
    DiscoveryEvent buildDiscovery();
    PeriodicEvent buildPeriodic();

    // WiFi
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
