#include "OpenAquariumRTOS.h"

String OpenAquariumRTOS::getVersion() {
  return this->VERSION;
}

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
  this->setupWiFi();
  this->getNTPDate(); // TODO refactor

  // SYNC RTC/NTP
  // this->

  // DHT - HUMIDITY, TEMPERATURE
  this->setupDHT();

  // LDR - ENVIRONMENT LIGHT
  this->setupLDR();

  // ENVIRONMENT NOISE
  this->setupSoundSensor();

  // BMP - BAROMETRIC PRESSURE
  this->setupBMP();

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
  char ssid[] = "";
  char pwd[] = "";
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

void OpenAquariumRTOS::setupDHT() {
  this->dht.begin();
}

void OpenAquariumRTOS::setupLDR() {
  pinMode(this->LDR_PIN, INPUT);
}

int OpenAquariumRTOS::readLDRSensor() {
  return digitalRead(this->LDR_PIN);
}

void OpenAquariumRTOS::setupSoundSensor() {
  // TODO replace with a better sensor
  pinMode(this->SOUND_SENSOR_PIN, INPUT);
}

int OpenAquariumRTOS::readSoundSensor() {
  // environment sound intensity > threshold = LOW
  // environment sound intensity < threshold = HIGH
  return digitalRead(this->SOUND_SENSOR_PIN);
}

void OpenAquariumRTOS::setupBMP() {
  if (!this->bmp.begin()) {
    // this->reportError(F("BMP sensor not found"));
    Serial.println("BMP sensor not found");
    Serial.flush();
    abort();
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void OpenAquariumRTOS::sendNTPpacket(char *ntpSrv, byte packetBuffer[], int NTP_PACKET_SIZE, WiFiEspUDP Udp) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)

  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(ntpSrv, 123); //NTP requests are to port 123

  Udp.write(packetBuffer, NTP_PACKET_SIZE);

  Udp.endPacket();
}

void OpenAquariumRTOS::getNTPDate() {
  // NTP
  char timeServer[] = "time.nist.gov";
  unsigned int localPort = 2390;
  const int NTP_PACKET_SIZE = 48;
  const int UDP_TIMEOUT = 2000;
  byte packetBuffer[NTP_PACKET_SIZE];
  WiFiEspUDP Udp;
  Udp.begin(localPort);
  sendNTPpacket(timeServer, packetBuffer, NTP_PACKET_SIZE, Udp);
  unsigned long startMs = millis();
  while (!Udp.available() && (millis() - startMs) < UDP_TIMEOUT) {}
  Serial.println(Udp.parsePacket());
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    // We've received a packet, read the data from it into the buffer
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
}