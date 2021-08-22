#include "OpenAquarium.h"

void OpenAquarium::setup() {
  Serial.begin(9600);
  while(!Serial);

  this->log.debug(F("OpenAquarium::setup"));
  this->sdcard.printDebug(F("OpenAquarium::setup"));

  this->buzzer.playBeep();

  this->setupActivityLed();

  // Real Time Clock
  this->realTimeClock.setup();

  // Display
  // this->display.setup();

  // Environment Humidity and Temperature
  // this->setupDHT();

  // Environment Light
  // this->setupLDR();

  // Environment Sound
  // this->setupSoundSensor();

  // Environment BMP
  // this->setupBMP();

  // WiFi
  setupWiFi();

  // this->display.displayWelcomeMessage();
  // delay(3000);
  // this->display.displayFusRoDah();
  // delay(1000);
  // this->display.sleepDisplay();

  // this->deviceReady();

  // String version = F("");
  // version += F("Date ");
  // version += this->realTimeClock.nowAsISOString();
  // version += F("  Version ");
  // version += this->getVersion();
  // version += F("  BUILD ");
  // version += this->getBuildVersion();
  // this->log.info(version);
  // this->sdcard.printDebug(version);
}

void OpenAquarium::loop() {
  if(!this->ready || this->error) {
    return;
  }
  
  uint64_t currentExecution = millis();

  // blink led
  if(this->lastActivityLedBlink == 0 || (currentExecution - this->lastActivityLedBlink) >= 250) {
    this->lastActivityLedBlink = currentExecution;
    if(this->lastActivityLedBlink == 0){ this->lastActivityLedBlink = 1; }
    blinkActivityLed();
  }

  if(lastDiscoveryEvent == 0 || (currentExecution - lastDiscoveryEvent) >= DISCOVERY_INTERVAL) {
    lastDiscoveryEvent = currentExecution;
    if(lastDiscoveryEvent == 0){ lastDiscoveryEvent = 1; }
    this->discoveryEvent();
  }
  
  if(lastPeriodicEvent == 0 || (currentExecution - lastPeriodicEvent) >= PERIODIC_INTERVAL) {
    lastPeriodicEvent = currentExecution;
    if(lastPeriodicEvent == 0){ lastPeriodicEvent = 1; }
    this->periodicEvent();
  }
}

void OpenAquarium::deviceReady() {
  this->ready = true;
  this->log.debug(F("deviceReady()"));
  this->sdcard.printDebug(F("OpenAquarium::deviceReady()"));
  // set status led to READY
}

void OpenAquarium::reportError(String message) {
  this->log.error(message);
  this->sdcard.printError(message);
  this->error = true;
}

void OpenAquarium::discoveryEvent() {
  this->log.debug(F("OpenAquarium::loop->discovery"));
  this->sdcard.printDebug(F("OpenAquarium::loop->discovery"));
  DiscoveryEvent discoveryEvent;
  discoveryEvent = this->buildDiscovery();

  String json = EventBuilder::discoveryToJSON(discoveryEvent); 
  this->log.info(json);
  this->sdcard.println(json);
}

void OpenAquarium::periodicEvent() {
  this->log.debug(F("OpenAquarium::loop->periodic"));
  this->sdcard.printDebug(F("OpenAquarium::loop->periodic"));
  PeriodicEvent periodicEvent;
  periodicEvent = this->buildPeriodic();
  
  String json = EventBuilder::periodicToJSON(periodicEvent); 
  this->log.info(json);
  this->sdcard.println(json);
}

void OpenAquarium::setupActivityLed() {
  pinMode(LED_BUILTIN, OUTPUT);
  this->activityLedOn = false;
  digitalWrite(LED_BUILTIN, LOW);
}

void OpenAquarium::blinkActivityLed() {
  if(this->activityLedOn) {
    this->activityLedOn = false;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    this->activityLedOn = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

String OpenAquarium::getVersion() {
  return this->VERSION;
}

String OpenAquarium::getBuildVersion() {
  String version = F("");
  version += __DATE__;
  version += F(" ");
  version += __TIME__;
  return version;
}

void OpenAquarium::setupDHT() {
  this->dht.begin();
}

void OpenAquarium::setupLDR() {
  pinMode(this->LDR_PIN, INPUT);
}

int OpenAquarium::readLDRSensor() {
  return digitalRead(this->LDR_PIN);
}

void OpenAquarium::setupSoundSensor() {
  // TODO replace with a better sensor
  pinMode(this->SOUND_SENSOR_PIN, INPUT);
}

int OpenAquarium::readSoundSensor() {
  // environment sound intensity > threshold = LOW
  // environment sound intensity < threshold = HIGH
  return digitalRead(this->SOUND_SENSOR_PIN);
}

void OpenAquarium::setupBMP() {
  if (!this->bmp.begin()) {
    this->reportError(F("BMP sensor not found"));
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

DiscoveryEvent OpenAquarium::buildDiscovery() {
  this->log.debug(F("OpenAquarium::buildDiscovery()"));
  DiscoveryEvent discovery1;
  discovery1.header = this->buildHeaderBlock(1);
  discovery1.device = this->buildDeviceBlock();
  discovery1.rollCallData = this->buildRollCallDataBlock();
  
  return discovery1;
}

PeriodicEvent OpenAquarium::buildPeriodic() {
  this->log.debug(F("OpenAquarium::buildPeriodic()"));
  PeriodicEvent periodic1;
  periodic1.header = this->buildHeaderBlock(2);
  periodic1.device = this->buildDeviceBlock();
  periodic1.deviceSample = this->buildDeviceSampleBlock();
  periodic1.environmentSample = this->buildEnvironmentSampleBlock();
  periodic1.waterSample = this->buildWaterSampleBlock();
  return periodic1;
}

HeaderBlock OpenAquarium::buildHeaderBlock(int eventType) {
  this->log.debug(F("OpenAquarium::buildHeaderBlock()"));
  HeaderBlock header1;
  if(eventType == 1) {
    header1.type = "DISCOVERY";
  } else if(eventType == 2) {
    header1.type = "PERIODIC";
  } else {
    header1.type = "";
  }
  header1.eventId = "383c44a4-adf7-4831-a4f5-f4019087e8db";
  // header1.triggerTime = "2020-11-29T03:48:31.218Z";
  header1.triggerTime = this->realTimeClock.nowAsISOString();
  return header1;
}

DeviceBlock OpenAquarium::buildDeviceBlock() {
  this->log.debug(F("OpenAquarium::buildDeviceBlock()"));
  DeviceBlock device1;
  device1.serialNumber = "A123";
  device1.softwareVersion = "1.0.0";
  device1.hardwareVersion = "1.1.0";
  return device1;
}

RollCallDataBlock OpenAquarium::buildRollCallDataBlock() {
  this->log.debug(F("OpenAquarium::buildRollCallData()"));
  RollCallSensor rollCallSensor1;
  rollCallSensor1.id = 1000;
  rollCallSensor1.sensor = "ESP";
  rollCallSensor1.address = "D1";
  rollCallSensor1.category = "environment";

  RollCallSensor rollCallSensor2;
  rollCallSensor2.id = 2000;
  rollCallSensor2.sensor = "DHT22";
  rollCallSensor2.address = "C1";
  rollCallSensor2.category = "device";

  RollCallDataBlock rollCallData1;
  rollCallData1.sensors[0] = rollCallSensor1;
  rollCallData1.sensors[1] = rollCallSensor2;

  return rollCallData1;
}

DeviceSampleBlock OpenAquarium::buildDeviceSampleBlock() {
  this->log.debug(F("OpenAquarium::buildDeviceSampleBlock();"));
  DeviceSampleBlock deviceSample1;
  deviceSample1.freeMemory = this->device.getFreeSRAM();

  deviceSample1.sdCardType = this->sdcard.cardType();
  deviceSample1.sdCardVolumeType = this->sdcard.volumeType();
  deviceSample1.sdCardClusterCount = this->sdcard.clusterCount();
  deviceSample1.sdCardBlocksPerCluster = this->sdcard.blocksPerCluster();
  deviceSample1.sdCardVolumeSize = this->sdcard.volumeSize();

  deviceSample1.temperature = this->device.getDeviceInbuiltTemperature();
  
  return deviceSample1;
}

EnvironmentSampleBlock OpenAquarium::buildEnvironmentSampleBlock() {
  this->log.debug(F("OpenAquarium::buildEnvironmentSampleBlock();"));
  EnvironmentSampleBlock environmentSample1;
  environmentSample1.roomTemperature = dht.readTemperature();
  environmentSample1.roomTemperature2 = bmp.readTemperature();
  environmentSample1.relativeHumidity = dht.readHumidity();
  environmentSample1.heatIndex = OA_MIN_FLOAT;
  environmentSample1.atmosphericPressure = this->bmp.readPressure();
  environmentSample1.altitude = this->bmp.readAltitude(1013.25);

  if (isnan(environmentSample1.relativeHumidity) || isnan(environmentSample1.roomTemperature)) {
    this->log.error(F("Failed to read from DHT sensor!"));
  } else {
    environmentSample1.heatIndex = dht.computeHeatIndex(
      environmentSample1.roomTemperature, 
      environmentSample1.relativeHumidity, 
      false
    );
  }

  // TODO change the sensors
  environmentSample1.lightIntensity = this->readLDRSensor();
  environmentSample1.noiseLevel = this->readSoundSensor();

  return environmentSample1;
}

WaterSampleBlock OpenAquarium::buildWaterSampleBlock() {
  this->log.debug(F("OpenAquarium::buildWaterSampleBlock()"));
  WaterSampleBlock waterSample1;
  waterSample1.temperature1 = OA_MIN_FLOAT;
  waterSample1.temperature2 = OA_MIN_FLOAT;
  waterSample1.totalDissolvedSolids = OA_MIN_FLOAT;
  waterSample1.waterLevelLow = true;
  waterSample1.waterLevelMedium = true;
  waterSample1.waterLevelHigh = true;
  return waterSample1;
}

void OpenAquarium::setupWiFi() {
  this->log.debug(F("setupWiFi()"));
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
  this->log.info(message);

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

  /*WiFiUDP udp;
  NTPClient ntp(udp, "0.br.pool.ntp.org", -3 * 3600, 60000);
  ntp.begin();
  //ntp.forceUpdate();
  while(!ntp.update())
    {
        Serial.print(".");
        ntp.forceUpdate();
        delay(500);
    }

  char* strDate = (char*)ntp.getFormattedDate().c_str();

  //Passa os dados da string para a struct
  Date date;
  sscanf(strDate, "%d-%d-%dT%d:%d:%dZ", 
                  &date.year, 
                  &date.month, 
                  &date.day, 
                  &date.hours, 
                  &date.minutes,
                  &date.seconds);

  //Dia da semana de 0 a 6, sendo 0 o domingo
  date.dayOfWeek = ntp.getDay();

  message = F("NTP ");
  message += ntp.getFormattedTime();
  message += date.day;
  message += "/";
  message += date.month;
  message += "/";
  message += date.year;
  message += " ";
  message += date.hours;
  message += ":";
  message += date.minutes;
  message += ":";
  message += date.seconds;
  this->log.info(message);*/
}

void OpenAquarium::sendNTPpacket(char *ntpSrv, byte packetBuffer[], int NTP_PACKET_SIZE, WiFiEspUDP Udp)
{
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

void OpenAquarium::connectWiFi() {
  char ssid[] = "";
  char pwd[] = "";
  WiFi.begin(ssid, pwd);
}

String OpenAquarium::getFirmwareVersion() {
  this->log.debug(F("getFirmwareVersion()"));
  return WiFi.firmwareVersion();
}

String OpenAquarium::getWiFiStatus() {
  this->log.debug(F("getWiFiStatus()"));
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

String OpenAquarium::getWiFiSSID() {
  this->log.debug(F("getWiFiSSID()"));
  return WiFi.SSID();
}

String OpenAquarium::getMacAddress() {
  this->log.debug(F("getMacAddress()"));
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

String OpenAquarium::getIP() {
  this->log.debug(F("getIP()"));
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

String OpenAquarium::getSubnetMask() {
  this->log.debug(F("getSubnetMask()"));
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

String OpenAquarium::getGatewayIP() {
  this->log.debug(F("getGatewayIP()"));
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

int32_t OpenAquarium::getRSSI() {
  this->log.debug(F("getRSSI()"));
  return WiFi.RSSI();
}

