#ifndef OA_CUSTOM_WIFI_H
#define OA_CUSTOM_WIFI_H

#include <Arduino.h>
#include "WiFiEsp.h"
#include "WiFiEspUdp.h"

class CustomWiFi {

  private:
    WiFiEspClient client;

  public:

    CustomWiFi();

    void setup();
    void connect();

    String getFirmwareVersion();
    String getWiFiStatus();
    String getWiFiSSID();
    String getMacAddress();
    String getIP();
    String getSubnetMask();
    String getGatewayIP();
    int32_t getRSSI();
    unsigned long getNTPDate();

    bool ping(String hostname);
};

#endif
