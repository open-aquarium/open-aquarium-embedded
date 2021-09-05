#include "OpenAquariumRTOS2.h"

void OpenAquariumRTOS2::setup() {
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

  // INITIALIZE BLOCKS
  initializeDeviceBlock();
  initializeSamples();

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
  this->wifi.setup();
  Serial.println(F("Started: Wi-Fi."));

  /*******************************************************************************
   * DEVICE SENSORS
   *******************************************************************************/
    
  /*******************************************************************************
   * ENVIRONMENT SENSORS
   *******************************************************************************/
  
  // DHT
  this->setupDHT();

  // BMP
  this->setupBMP();

  /*******************************************************************************
   * WATER SENSORS
   *******************************************************************************/
  
  /*******************************************************************************
   * SENSORS CALIBRATION
   *******************************************************************************/
  this->sensorsCalibration();
  
  /*******************************************************************************
   * FINALIZING
   *******************************************************************************/
  // FIRST DISCOVERY/PERIODIC RIGHT AWAY
  this->previousDiscoveryMillis = this->INTERVAL_24H;
  this->previousPeriodicMillis = this->INTERVAL_24H;

  // FINALIZING
  String now = F("System time: ");
  now += this->realTimeClock.nowAsISOString();
  Serial.println(now);
  Serial.println(F("Started: Basic System."));
}

void OpenAquariumRTOS2::setupActivityLed() {
  // Serial.println(F("OpenAquariumRTOS2::setupActivityLed()")); // TODO log
  pinMode(this->ACTIVITY_LED_PIN, OUTPUT);
  digitalWrite(this->ACTIVITY_LED_PIN, LOW);
  Serial.println(F("Started: Activity Led."));
}

void OpenAquariumRTOS2::blinkActivityLed() {
  if(this->activityLed) {
    this->activityLed = false;
    digitalWrite(this->ACTIVITY_LED_PIN, LOW);
  } else {
    this->activityLed = true;
    digitalWrite(this->ACTIVITY_LED_PIN, HIGH);
  }
}

void OpenAquariumRTOS2::produceDiscovery() {
  // Serial.println(F("OpenAquariumRTOS2::produceDiscovery"));
  String now = this->realTimeClock.nowAsISOString();
  now += " ";
  now += F("produceDiscovery()");
  Serial.println(now);
  // String ntp = this->wifi.getNTPDate();
  // Serial.print("NTP: ");
  // Serial.println(ntp);
}

void OpenAquariumRTOS2::producePeriodic() {
  // Serial.println(F("OpenAquariumRTOS2::producePeriodic()"));
  String now = this->realTimeClock.nowAsISOString();
  now += " ";
  now += F("producePeriodic()");
  Serial.println(now);
}

void OpenAquariumRTOS2::synchronizeRTC() {
  String now = this->realTimeClock.nowAsISOString();
  now += " ";
  now += F("synchronizeRTC()");
  Serial.println(now);
}

void OpenAquariumRTOS2::setupDHT() {
  this->dht.begin();
}

void OpenAquariumRTOS2::setupBMP() {
  if (!this->bmp.begin()) {
    this->sdcard.printError(F("BMP sensor not found"));
    Serial.println(F("BMP sensor not found"));
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void OpenAquariumRTOS2::loop() {
  unsigned long currentMillis = millis();
  this->activityLedTask(currentMillis);
  this->dht22Task(currentMillis);
  this->bmp280Task(currentMillis);
  this->discoveryTask(currentMillis);
  this->periodicTask(currentMillis);
  this->rtcSynchronizationTask(currentMillis);
  this->wifiReconnectionTask(currentMillis);
  this->deviceTask(currentMillis);
  
  if (currentMillis - this->previousTestMillis >= this->testInterval) {
    this->previousTestMillis = currentMillis;
    if(true) { return; }
    Serial.println("BEGIN TEST ------------------------------");
    Serial.print(this->realTimeClock.nowAsISOString());
    Serial.println(F(" TEST"));
    
    Serial.println(F("DEVICE BLOCK"));
    Serial.print("CPU ");
    Serial.println(this->deviceBlock.cpu);
    Serial.print("CPU Speed ");
    Serial.println(this->deviceBlock.cpuSpeed);
    Serial.print("EEPROM ");
    Serial.println(this->deviceBlock.totalEEPROM);
    Serial.print("Flash Memory ");
    Serial.println(this->deviceBlock.totalFlash);
    Serial.print("Total Memory ");
    Serial.println(this->deviceBlock.totalMemory);
    Serial.print("SD CARD -> cardType ");
    Serial.println(this->deviceBlock.sdCardType);
    Serial.print("SD CARD -> Volume Type ");
    Serial.println(this->deviceBlock.sdCardVolumeType);
    Serial.print("SD CARD -> Volume Size ");
    Serial.println(this->deviceBlock.sdCardVolumeSize);
    Serial.print("SD CARD -> Volume Cluster Count ");
    Serial.println(this->deviceBlock.sdCardClusterCount);
    Serial.print("SD CARD -> Volume Blocks per Cluster ");
    Serial.println(this->deviceBlock.sdCardBlocksPerCluster);
    Serial.print("SD CARD -> Total Blocks ");
    Serial.println(this->deviceBlock.sdCardTotalBlocks);

    Serial.println(F("DEVICE SAMPLES"));
    
    Serial.print("Free Memory ");
    Serial.println(this->deviceSample.freeMemory);
    Serial.print("Used Memory ");
    Serial.println(this->deviceSample.usedMemory);
    Serial.print("temperature ");
    Serial.println(this->deviceSample.temperature);
    Serial.print("SD CARD -> Free Space ");
    Serial.println(this->deviceSample.sdCardFreeSpace);

    Serial.println(F("ENVIRONMENT SAMPLES"));
    Serial.print("Temperature ");
    Serial.println(this->environmentSample.roomTemperature);
    Serial.print("Humidity ");
    Serial.println(this->environmentSample.relativeHumidity);
    Serial.print("Heat Index ");
    Serial.println(this->environmentSample.heatIndex);
    Serial.print("Temperature 2 ");
    Serial.println(this->environmentSample.roomTemperature2);
    Serial.print("Atmosferic Pressure ");
    Serial.println(this->environmentSample.atmosphericPressure);
    Serial.print("Altitude ");
    Serial.println(this->environmentSample.altitude);
    Serial.print("Light Intensity ");
    Serial.println(this->environmentSample.lightIntensity);
    Serial.print("Noise Level ");
    Serial.println(this->environmentSample.noiseLevel);
    
    Serial.println("END TEST ------------------------------");
  }
  
}

void OpenAquariumRTOS2::initializeDeviceBlock() {
  this->deviceBlock.serialNumber = this->SERIAL_NUMBER;
  this->deviceBlock.softwareVersion = this->SOFTWARE_VERSION;
  this->deviceBlock.hardwareVersion = this->HARDWARE_VERSION;
  /*this->deviceBlock.cpu = F("UNKNOWN");
  this->deviceBlock.cpuSpeed = 0;
  this->deviceBlock.totalEEPROM = 0;
  this->deviceBlock.totalFlash = 0;
  this->deviceBlock.totalMemory = 0;
  this->deviceBlock.sdCardType = F("UNKNOWN");
  this->deviceBlock.sdCardVolumeType = F("UNKNOWN");
  this->deviceBlock.sdCardVolumeSize = 0;
  this->deviceBlock.sdCardClusterCount = 0;
  this->deviceBlock.sdCardBlocksPerCluster = 0;
  this->deviceBlock.sdCardTotalBlocks = 0;*/

  this->deviceBlock.cpu = this->device.getCPU();
  this->deviceBlock.cpuSpeed = this->device.getCPUSpeed();
  this->deviceBlock.totalEEPROM = this->device.getTotalEEPROM();
  this->deviceBlock.totalFlash = this->device.getTotalFlash();
  this->deviceBlock.totalMemory = this->device.getTotalSRAM();
  this->deviceBlock.sdCardType = this->sdcard.cardType();
  this->deviceBlock.sdCardVolumeType = this->sdcard.volumeType();
  this->deviceBlock.sdCardVolumeSize = this->sdcard.volumeSize();
  this->deviceBlock.sdCardClusterCount = this->sdcard.clusterCount();
  this->deviceBlock.sdCardBlocksPerCluster = this->sdcard.blocksPerCluster();
  this->deviceBlock.sdCardTotalBlocks = this->deviceBlock.sdCardBlocksPerCluster * this->deviceBlock.sdCardClusterCount;
}

void OpenAquariumRTOS2::initializeSamples() {
  // DEVICE
  this->deviceSample.freeMemory = 0;
  this->deviceSample.usedMemory = 0;
  this->deviceSample.temperature = this->ABSOLUTE_ZERO;
  this->deviceSample.sdCardFreeSpace = 0;

  // ENVIRONMENT
  this->environmentSample.roomTemperature = this->ABSOLUTE_ZERO;
  this->environmentSample.roomTemperature2 = this->ABSOLUTE_ZERO;
  this->environmentSample.relativeHumidity = this->ABSOLUTE_ZERO;
  this->environmentSample.heatIndex = this->ABSOLUTE_ZERO;
  this->environmentSample.atmosphericPressure = this->ABSOLUTE_ZERO;
  this->environmentSample.altitude = this->ABSOLUTE_ZERO;
  this->environmentSample.lightIntensity = this->ABSOLUTE_ZERO;
  this->environmentSample.noiseLevel = this->ABSOLUTE_ZERO;

  // WATER
  this->waterSample.temperature1 = this->ABSOLUTE_ZERO;
  this->waterSample.temperature2 = this->ABSOLUTE_ZERO;
  this->waterSample.totalDissolvedSolids = this->ABSOLUTE_ZERO;
  this->waterSample.waterLevelLow = false;
  this->waterSample.waterLevelMedium = false;
  this->waterSample.waterLevelHigh = false;
}

void OpenAquariumRTOS2::sensorsCalibration() {
  String info = this->realTimeClock.nowAsISOString();
  info += " OpenAquariumRTOS2::sensorsCalibration()";
  this->sdcard.printLog(info);
  Serial.println(info);
  this->dht22Task(this->INTERVAL_24H);
  this->bmp280Task(this->INTERVAL_24H);
}

void OpenAquariumRTOS2::activityLedTask(unsigned long currentMillis) {
  if (currentMillis - this->previousActivityLedMillis >= this->activityLedInterval) {
    this->previousActivityLedMillis = currentMillis;
    this->blinkActivityLed();
  }
}

void OpenAquariumRTOS2::discoveryTask(unsigned long currentMillis) {
  if (currentMillis - this->previousDiscoveryMillis >= this->discoveryInterval) {
    this->previousDiscoveryMillis = currentMillis;
    String info = this->realTimeClock.nowAsISOString();
    info += " DISCOVERY";
    this->sdcard.printLog(info);
    Serial.println(info);

    StaticJsonDocument<300> doc;
    doc["version"] = "1.0.0";
    doc["header"]["type"] = "discovery";
    doc["header"]["eventId"] = "487fbb27-3c4a-44c9-9ee6-4dc9c3961cbf";
    doc["header"]["triggerTime"] = this->realTimeClock.nowAsISOString();
    doc["device"]["serialNumber"] = this->SERIAL_NUMBER;
    doc["device"]["softwareVersion"] = this->SOFTWARE_VERSION;
    doc["device"]["hardwareVersion"] = this->HARDWARE_VERSION;
    doc["device"]["cpu"] = this->deviceBlock.cpu;
    doc["device"]["cpuSpeed"] = this->deviceBlock.cpuSpeed;
    doc["device"]["totalEEPROM"] = this->deviceBlock.totalEEPROM;
    doc["device"]["totalFlash"] = this->deviceBlock.totalFlash;
    doc["device"]["totalMemory"] = this->deviceBlock.totalMemory;
    doc["device"]["sdCardType"] = this->deviceBlock.sdCardType;
    doc["device"]["sdCardVolumeType"] = this->deviceBlock.sdCardVolumeType;
    doc["device"]["sdCardVolumeSize"] = this->deviceBlock.sdCardVolumeSize;
    doc["device"]["sdCardClusterCount"] = this->deviceBlock.sdCardClusterCount;
    doc["device"]["sdCardBlocksPerCluster"] = this->deviceBlock.sdCardBlocksPerCluster;
    doc["device"]["sdCardTotalBlocks"] = this->deviceBlock.sdCardTotalBlocks;
    JsonArray data = doc.createNestedArray("rollCallData");
    data.add("DHT22");
    data.add("BMP280");
    serializeJson(doc, Serial);
    Serial.println();
    String output;
    serializeJson(doc, output);
    this->sdcard.printLog(output);
  }
}

void OpenAquariumRTOS2::periodicTask(unsigned long currentMillis) {
  if (currentMillis - this->previousPeriodicMillis >= this->periodicInterval) {
    this->previousPeriodicMillis = currentMillis;
    String info = this->realTimeClock.nowAsISOString();
    info += " PERIODIC";
    this->sdcard.printLog(info);
    Serial.println(info);

    StaticJsonDocument<600> doc;
    doc["version"] = "1.0.0";

    doc["header"]["type"] = "periodic";
    doc["header"]["eventId"] = "e0b67db4-99ba-41b0-b854-4cc721bf524e";
    doc["header"]["triggerTime"] = this->realTimeClock.nowAsISOString();

    doc["device"]["serialNumber"] = this->SERIAL_NUMBER;
    doc["device"]["softwareVersion"] = this->SOFTWARE_VERSION;
    doc["device"]["hardwareVersion"] = this->HARDWARE_VERSION;
    doc["device"]["cpu"] = this->deviceBlock.cpu;
    doc["device"]["cpuSpeed"] = this->deviceBlock.cpuSpeed;
    doc["device"]["totalEEPROM"] = this->deviceBlock.totalEEPROM;
    doc["device"]["totalFlash"] = this->deviceBlock.totalFlash;
    doc["device"]["totalMemory"] = this->deviceBlock.totalMemory;
    doc["device"]["sdCardType"] = this->deviceBlock.sdCardType;
    doc["device"]["sdCardVolumeType"] = this->deviceBlock.sdCardVolumeType;
    doc["device"]["sdCardVolumeSize"] = this->deviceBlock.sdCardVolumeSize;
    doc["device"]["sdCardClusterCount"] = this->deviceBlock.sdCardClusterCount;
    doc["device"]["sdCardBlocksPerCluster"] = this->deviceBlock.sdCardBlocksPerCluster;
    doc["device"]["sdCardTotalBlocks"] = this->deviceBlock.sdCardTotalBlocks;

    doc["deviceSample"]["batteryPowered"] = false;
    doc["deviceSample"]["freeMemory"] = this->deviceSample.freeMemory;
    doc["deviceSample"]["usedMemory"] = this->deviceSample.usedMemory;
    doc["deviceSample"]["temperature"] = this->deviceSample.temperature;
    doc["deviceSample"]["sdCardFreeSpace"] = this->deviceSample.sdCardFreeSpace;
    doc["deviceSample"]["sdCardUsedSpace"] = this->deviceSample.sdCardUsedSpace;

    doc["environmentSample"]["roomTemperature"] = this->environmentSample.roomTemperature;
    doc["environmentSample"]["roomTemperature2"] = this->environmentSample.roomTemperature2;
    doc["environmentSample"]["relativeHumidity"] = this->environmentSample.relativeHumidity;
    doc["environmentSample"]["heatIndex"] = this->environmentSample.heatIndex;
    doc["environmentSample"]["atmosphericPressure"] = this->environmentSample.atmosphericPressure;
    doc["environmentSample"]["altitude"] = this->environmentSample.altitude;
    doc["environmentSample"]["lightIntensity"] = -273.15;
    doc["environmentSample"]["noiseLevel"] = -273.15;

    doc["waterSample"]["temperature1"] = -273.15;
    doc["waterSample"]["temperature2"] = -273.15;
    doc["waterSample"]["totalDissolvedSolids"] = -273.15;
    doc["waterSample"]["ph"] = -273.15;
    doc["waterSample"]["waterLevelHigh"] = false;
    doc["waterSample"]["waterLevelMedium"] = false;
    doc["waterSample"]["waterLevelLow"] = true;
    
    serializeJson(doc, Serial);
    Serial.println();
    String output;
    serializeJson(doc, output);
    this->sdcard.printLog(output);
  }
}

void OpenAquariumRTOS2::dht22Task(unsigned long currentMillis) {
  if (currentMillis - this->previousDht22Millis >= this->dht22Interval) {
    // TODO loglevel info
    // Serial.print(this->realTimeClock.nowAsISOString());
    // Serial.println(" OpenAquariumRTOS2::dht22Task()");
    this->previousDht22Millis = currentMillis;
    String info = this->realTimeClock.nowAsISOString();
    info += " OpenAquariumRTOS2::dht22Task";
    this->sdcard.printLog(info);
    this->environmentSample.roomTemperature = this->dht.readTemperature();
    this->environmentSample.relativeHumidity = this->dht.readHumidity();
    this->environmentSample.heatIndex = this->ABSOLUTE_ZERO;
    if (
      isnan(this->environmentSample.relativeHumidity) || 
      isnan(this->environmentSample.roomTemperature)
    ) {
      Serial.println(F("ERROR: Failed to read from DHT sensor!"));
    } else {
      this->environmentSample.heatIndex = dht.computeHeatIndex(
        this->environmentSample.roomTemperature,
        this->environmentSample.relativeHumidity,
        false
      );
    }
  }
}

void OpenAquariumRTOS2::bmp280Task(unsigned long currentMillis) {
  if (currentMillis - this->previousBmp280Millis >= this->bmp280Interval) {
    // TODO loglevel info
    // Serial.print(this->realTimeClock.nowAsISOString());
    // Serial.println(" OpenAquariumRTOS2::dht22Task()");
    this->previousBmp280Millis = currentMillis;
    String info = this->realTimeClock.nowAsISOString();
    info += " OpenAquariumRTOS2::bmp280Task";
    this->sdcard.printLog(info);
    this->environmentSample.roomTemperature2 = this->bmp.readTemperature();
    this->environmentSample.atmosphericPressure = this->bmp.readPressure(); // Pascal
    this->environmentSample.altitude = this->bmp.readAltitude(1016.5); // Regional adjustment
  }
}

void OpenAquariumRTOS2::rtcSynchronizationTask(unsigned long currentMillis) {
  if (currentMillis - this->previousRtcSynchronizationMillis >= this->rtcSynchronizationInterval) {
    // TODO loglevel info
    // Serial.print(this->realTimeClock.nowAsISOString());
    // Serial.println(" OpenAquariumRTOS2::rtcSynchronizationTask()");
    this->previousRtcSynchronizationMillis = currentMillis;
    String info = this->realTimeClock.nowAsISOString();
    info += " OpenAquariumRTOS2::rtcSynchronizationTask()";
    this->sdcard.printLog(info);
    unsigned long epoch = this->wifi.getNTPDate();
    // TODO is NTP time correct?
    if(epoch > 1629665697 && epoch < 2576350497) {
      // TODO only adjust RTC when the difference is greater then or equal to 1 min
      struct tm  ts = Converter::epochToTmStruct(epoch);
      this->realTimeClock.adjust(DateTime(1900+ts.tm_year, ts.tm_mon+1, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec));
    }
  }
}

void OpenAquariumRTOS2::wifiReconnectionTask(unsigned long currentMillis) {
  if (currentMillis - this->previousWifiReconnectionMillis >= this->wifiReconnectionInterval) {
    // TODO loglevel info
    // Serial.print(this->realTimeClock.nowAsISOString());
    // Serial.println(" OpenAquariumRTOS2::wifiReconnectionTask()");
    this->previousWifiReconnectionMillis = currentMillis;
    String info = this->realTimeClock.nowAsISOString();
    info += " OpenAquariumRTOS2::wifiReconnectionTask";
    this->sdcard.printLog(info);
    if(!this->wifi.isConnected()) {
      Serial.println("Reconnect");
      this->sdcard.printLog("Reconnect");
      this->wifi.connect();
    }
  }
}

void OpenAquariumRTOS2::deviceTask(unsigned long currentMillis) {
  if (currentMillis - this->previousDeviceMillis >= this->deviceInterval) {
    this->previousDeviceMillis = currentMillis;
    // TODO loglevel info
    String info = this->realTimeClock.nowAsISOString();
    info += " OpenAquariumRTOS2::deviceTask";
    this->sdcard.printLog(info);
    Serial.println(info);

    this->deviceSample.freeMemory = this->device.getFreeSRAM();
    this->deviceSample.usedMemory = this->device.getSRAMUsage();
    this->deviceSample.temperature = this->device.getDeviceInbuiltTemperature();
    this->deviceSample.sdCardFreeSpace = this->sdcard.freeSpace();
  }
}
