#include "OpenAquariumRTOS.h"

bool OpenAquariumRTOS::isSystemReady() {
  return this->bootBasicSystem;
}

// void OpenAquariumRTOS::setDebugMode(bool debug) {
//   this->debugMode = debug;
// }

// void OpenAquariumRTOS::setProductionMode(bool prod) {
//   this->productionMode = prod;
// }  

void OpenAquariumRTOS::setup() {
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

  // REAL TIME CLOCK
  this->realTimeClock.setup();
  Serial.println(F("Started: Real Time Clock."));

  // WIFI
  this->setupWiFi();

  // SYNC RTC/NTP
  // this->

  // DISPLAY
  // this->display.setup();

  // DHT - HUMIDITY, TEMPERATURE
  // this->setupDHT();

  // LDR - ENVIRONMENT LIGHT
  // this->setupLDR();

  // ENVIRONMENT NOISE
  // this->setupSoundSensor();

  // BMP - BAROMETRIC PRESSURE
  // this->setupBMP();

  // DISPLAY
  // this->display.setup();

  // DIAGNOSTICS (check all sensors)
  // this->

  // FINALIZING
  Serial.println(F("System time: 2021-01-01H22:00:00.0Z"));
  Serial.println(F("Started: Basic System."));
  this->bootBasicSystem = true;
}

void OpenAquariumRTOS::setupActivityLed() {
  // Serial.println("OpenAquariumRTOS::setupActivityLed()");
  pinMode(this->ACTIVITY_LED_PIN, OUTPUT);
  digitalWrite(this->ACTIVITY_LED_PIN, LOW);
  Serial.println(F("Started: Activity Led."));
}

void OpenAquariumRTOS::blinkActivityLed(bool on) {
  if(on) {
    digitalWrite(this->ACTIVITY_LED_PIN, HIGH);
  } else {
    digitalWrite(this->ACTIVITY_LED_PIN, LOW);
  }
}

void OpenAquariumRTOS::connectWiFi() {
  char ssid[] = "estado-da-arte-2.4";
  char pwd[] = "guilherme5543988036135";
  WiFi.begin(ssid, pwd);
}

void OpenAquariumRTOS::setupWiFi() {
  // Serial.println("OpenAquariumRTOS::setupWiFi()");
  Serial3.begin(115200);
  WiFi.init(&Serial3);
  this->connectWiFi();

  String message = F("");
  message += F("Firmware ");
  message += getFirmwareVersion();
  message += F("  SSID ");
  message += this->getWiFiSSID();
  message += F("  Status ");
  message += this->getWiFiStatus();
  message += F("  MAC ");
  message += this->getMacAddress();
  message += F("  IP ");
  message += this->getIP();
  message += F("  Subnet Mask ");
  message += this->getSubnetMask();
  message += F("  Gateway ");
  message += this->getGatewayIP();
  message += F("  RSSI ");
  message += this->getRSSI();
  Serial.println(message);
  Serial.println(F("Started: WiFi."));
}

String OpenAquariumRTOS::getFirmwareVersion() {
  Serial.println(F("getFirmwareVersion()"));
  return WiFi.firmwareVersion();
}

String OpenAquariumRTOS::getWiFiStatus() {
  Serial.println(F("getWiFiStatus()"));
  uint8_t status = WiFi.status();
  if(status == WL_CONNECTED) {
    return F("Connected");
  } else if(status == WL_NO_SHIELD) {
    return F("No WiFi");
  } else if(status == WL_IDLE_STATUS) {
    return F("Idle");
  } /*else if(status == WL_NO_SSID_AVAIL) {
    return F("No SSID Available");
  } else if(status == WL_SCAN_COMPLETED) {
    return F("Scan Completed");
  } */else if(status == WL_CONNECT_FAILED) {
    return F("Connect Failed");
  } /*else if(status == WL_CONNECTION_LOST) {
    return F("Connection Lost");
  } */else if(status == WL_DISCONNECTED) {
    return F("Disconnected");
  } else {
    return F("Error");
  }
}

String OpenAquariumRTOS::getWiFiSSID() {
  Serial.println(F("getWiFiSSID()"));
  return WiFi.SSID();
}

String OpenAquariumRTOS::getMacAddress() {
  Serial.println(F("getMacAddress()"));
  byte mac[6]={0,0,0,0,0,0};
  WiFi.macAddress(mac);
  String result = F("");
  result += String(mac[5], HEX);
  result += F(":");
  result += String(mac[4], HEX);
  result += F(":");
  result += String(mac[3], HEX);
  result += F(":");
  result += String(mac[2], HEX);
  result += F(":");
  result += String(mac[1], HEX);
  result += F(":");
  result += String(mac[0], HEX);
  return result;
}

String OpenAquariumRTOS::getIP() {
  Serial.println(F("getIP()"));
  IPAddress ip = WiFi.localIP();
  String result = F("");
  result += ip[0];
  result += F(".");
  result += ip[1];
  result += F(".");
  result += ip[2];
  result += F(".");
  result += ip[3];
  return result;
}

String OpenAquariumRTOS::getSubnetMask() {
  Serial.println(F("getSubnetMask()"));
  //return WiFi.subnetMask();
  IPAddress subnet = WiFi.subnetMask();
  String result = F("");
  result += subnet[0];
  result += F(".");
  result += subnet[1];
  result += F(".");
  result += subnet[2];
  result += F(".");
  result += subnet[3];
  return result;
}

String OpenAquariumRTOS::getGatewayIP() {
  Serial.println(F("getGatewayIP()"));
  //return WiFi.gatewayIP();
  IPAddress gateway = WiFi.gatewayIP();
  String result = F("");
  result += gateway[0];
  result += F(".");
  result += gateway[1];
  result += F(".");
  result += gateway[2];
  result += F(".");
  result += gateway[3];
  return result;
}

int32_t OpenAquariumRTOS::getRSSI() {
  Serial.println(F("getRSSI()"));
  return WiFi.RSSI();
}
