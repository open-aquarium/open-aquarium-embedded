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
  this->display.setup();

  // Environment Humidity and Temperature
  this->setupDHT();
  // Environment Light
  this->setupLDR();
  // Environment Sound
  this->setupSoundSensor();
  // Environment BMP
  this->setupBMP();

  this->display.displayWelcomeMessage();
  delay(3000);
  this->display.displayFusRoDah();
  delay(1000);
  this->display.sleepDisplay();

  this->deviceReady();

  String version = F("");
  version += F("Date ");
  version += this->realTimeClock.nowAsISOString();
  version += F("  Version ");
  version += this->getVersion();
  version += F("  BUILD ");
  version += this->getBuildVersion();
  this->log.info(version);
  this->sdcard.printDebug(version);
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
  // Add to debug log
  this->sdcard.printDebug(F("OpenAquarium::loop->periodic"));
  this->log.debug(F("OpenAquarium::loop->periodic"));
  PeriodicEvent periodicEvent;
  periodicEvent = this->buildPeriodic();
  
  String json = EventBuilder::periodicToJSON(periodicEvent); 
  this->log.debug(F("OpenAquarium::this->periodicToJSON(periodicEvent)"));
  this->log.info(json);
  this->sdcard.println(json);

  String data = F("PERIODIC  ");

  data += this->realTimeClock.nowAsISOString();

  float humidity = OA_MIN_FLOAT;
  float temperature = OA_MIN_FLOAT;
  float heatIndex = OA_MIN_FLOAT;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); // Celsius
  /*if (isnan(humidity) || isnan(temperature)) {
    this->reportError(F("Failed to read from DHT sensor!"));
    return;
  }  */
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  data += F("  Humidity: ");
  data += humidity;
  data += F("%");
  data += F("  Temperature: ");
  data += temperature;
  data += F("°C");
  data += F("  Heat Index: ");
  data += heatIndex;
  data += F("°C");
  data += F("  Light: ");
  data += this->readLDRSensor();
  data += F(" Free SRAM: ");
  data += this->device.getFreeSRAM();
  data += F("  Sound: "); // TODO
  data += readSoundSensor();

  // BMP
  data += F("  BMP-Temperature: ");
  data += this->bmp.readTemperature();
  data += F("  Pressure: ");
  data += this->bmp.readPressure();
  data += F(" Pa");
  data += F("  Altitude: ");
  data += this->bmp.readAltitude(1013.25);
  data += F(" m");

  this->log.info(data);
  this->sdcard.println(data);
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
  environmentSample1.relativeHumidity = dht.readHumidity();
  environmentSample1.heatIndex = OA_MIN_FLOAT;
  environmentSample1.atmosphericPressure = OA_MIN_FLOAT;
  environmentSample1.altitude = OA_MIN_FLOAT;

  if (isnan(environmentSample1.relativeHumidity) || isnan(environmentSample1.roomTemperature)) {
    this->log.error(F("Failed to read from DHT sensor!"));
  } else {
    environmentSample1.heatIndex = dht.computeHeatIndex(
      environmentSample1.roomTemperature, 
      environmentSample1.relativeHumidity, 
      false
    );
  }  
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
