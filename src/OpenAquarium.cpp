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

  // Environment Humidity and Temperature
  this->setupDHT();
  // Environment Light
  this->setupLDR();

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
