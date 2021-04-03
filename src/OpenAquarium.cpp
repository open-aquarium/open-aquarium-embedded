#include "OpenAquarium.h"

void OpenAquarium::setup() {
  Serial.begin(9600);
  while(!Serial);

  this->log.info(F("OpenAquarium::setup"));
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

  // this->reportError(F("ERROR: debug"));
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
  this->log.info(F("deviceReady()"));
  this->sdcard.printDebug(F("OpenAquarium::deviceReady()"));
  // set status led to READY
}

void OpenAquarium::reportError(String message) {
  this->log.error(message);
  this->sdcard.printError(message);
  this->error = true;
  // set error led
}

void OpenAquarium::discoveryEvent() {
  // this->log.info(F("OpenAquarium::loop->discovery"));
  this->sdcard.printDebug(F("OpenAquarium::loop->discovery"));
  DiscoveryEvent discoveryEvent;
  discoveryEvent = this->buildDiscovery();

  String data = F("DISCOVERY ");

  data += this->realTimeClock.nowAsISOString();
  
  data += F("  SD Card type: ");
  data += this->sdcard.cardType();

  data += F("  SD Volume type: ");
  data += this->sdcard.volumeType();

  data += F("  SD Cluster count: ");
  data += this->sdcard.clusterCount();

  data += F("  SD Blocks: ");
  data += this->sdcard.blocksPerCluster();

  data += F("  SD Volume size: ");
  data += this->sdcard.volumeSize();

  this->log.info(data);
  this->sdcard.println(data);
}

void OpenAquarium::periodicEvent() {
  // Add to debug log
  // this->log.info(F("OpenAquarium::loop->periodic"));
  this->sdcard.printDebug(F("OpenAquarium::loop->periodic"));
  PeriodicEvent periodicEvent;
  periodicEvent = this->buildPeriodic();

  String data = F("PERIODIC  ");

  data += this->realTimeClock.nowAsISOString();

  float humidity = this->ERROR_FLOAT_READING;
  float temperature = this->ERROR_FLOAT_READING;
  float heatIndex = this->ERROR_FLOAT_READING;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); // Celsius
  if (isnan(humidity) || isnan(temperature)) {
    this->reportError(F("Failed to read from DHT sensor!"));
    return;
  }  
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
  Serial.println("OpenAquarium::buildDiscovery()");
  DiscoveryEvent discovery1;
  discovery1.header = this->buildHeaderBlock();
  discovery1.device = this->buildDeviceBlock();
  discovery1.rollCallData = this->buildRollCallDataBlock();
  return discovery1;
}

PeriodicEvent OpenAquarium::buildPeriodic() {
  Serial.println("OpenAquarium::buildPeriodic()");
  PeriodicEvent periodic1;
  periodic1.header = this->buildHeaderBlock();
  periodic1.device = this->buildDeviceBlock();
  periodic1.deviceSample = this->buildDeviceSampleBlock();
  periodic1.environmentSample = this->buildEnvironmentSampleBlock();
  periodic1.waterSample = this->buildWaterSampleBlock();
  return periodic1;
}

HeaderBlock OpenAquarium::buildHeaderBlock() {
  Serial.println("OpenAquarium::buildHeaderBlock()");
  HeaderBlock header1;
  header1.type = "PERIODIC/DISCOVERY";
  header1.eventId = "383c44a4-adf7-4831-a4f5-f4019087e8db";
  header1.triggerTime = "2020-11-29T03:48:31.218Z";
  return header1;
}

DeviceBlock OpenAquarium::buildDeviceBlock() {
  Serial.println("OpenAquarium::buildDeviceBlock()");
  DeviceBlock device1;
  device1.serialNumber = "A123";
  device1.softwareVersion = "1.0.0";
  device1.hardwareVersion = "1.1.0";
  return device1;
}

RollCallDataBlock OpenAquarium::buildRollCallDataBlock() {
  Serial.println("OpenAquarium::buildRollCallData()");

  RollCallSensor rollCallSensor1;
  rollCallSensor1.id = 20000014;
  rollCallSensor1.sensor = "ESP";
  rollCallSensor1.address = "D1";
  rollCallSensor1.category = "environment";

  RollCallDataBlock rollCallData1;
  rollCallData1.sensors[0] = rollCallSensor1;

  return rollCallData1;
}

DeviceSampleBlock OpenAquarium::buildDeviceSampleBlock() {
  Serial.println("OpenAquarium::buildDeviceSampleBlock();");
  DeviceSampleBlock deviceSample1;
  deviceSample1.freeMemory = 127;
  return deviceSample1;
}

EnvironmentSampleBlock OpenAquarium::buildEnvironmentSampleBlock() {
  Serial.println("OpenAquarium::buildEnvironmentSampleBlock();");
  EnvironmentSampleBlock environmentSample1;
  environmentSample1.roomTemperature = 28;
  environmentSample1.relativeHumidity = 80;
  environmentSample1.atmosphericPressure = 67;
  environmentSample1.altitude = 140;
  return environmentSample1;
}

WaterSampleBlock OpenAquarium::buildWaterSampleBlock() {
  Serial.println("OpenAquarium::buildWaterSampleBlock()");
  WaterSampleBlock waterSample1;
  waterSample1.temperature1 = 24.5;
  waterSample1.temperature2 = 25;
  waterSample1.totalDissolvedSolids = 250.1;
  waterSample1.waterLevelLow = false;
  waterSample1.waterLevelMedium = false;
  waterSample1.waterLevelHigh = true;
  return waterSample1;
}
