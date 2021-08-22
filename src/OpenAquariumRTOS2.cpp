#include "OpenAquariumRTOS2.h"

String OpenAquariumRTOS2::getVersion() {
  return this->VERSION;
}

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

  // INITIALIZE SAMPLES
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

void OpenAquariumRTOS2::loop() {
  unsigned long currentMillis = millis();
  this->activityLedTask(currentMillis);
  this->dht22Task(currentMillis);
  this->discoveryTask(currentMillis);
  this->periodicTask(currentMillis);
  this->rtcSynchronizationTask(currentMillis);
  
  /*
  if (currentMillis - this->previousTestMillis >= this->testInterval) {
    this->previousTestMillis = currentMillis;
    Serial.println(" BEGIN TEST ----------");
    Serial.print(this->realTimeClock.nowAsISOString());
    Serial.println(F(" TEST"));
    Serial.print("Temperature ");
    Serial.println(this->environmentSample.roomTemperature);
    Serial.print("Humidity ");
    Serial.println(this->environmentSample.relativeHumidity);
    Serial.print("Heat Index ");
    Serial.println(this->environmentSample.heatIndex);
    Serial.println(" END TEST ----------");
  }
  */
  
}

void OpenAquariumRTOS2::initializeSamples() {
  // DEVICE
  this->deviceSample.freeMemory = 0;
  this->deviceSample.sdCardType = "UNKNOWN";
  this->deviceSample.sdCardVolumeType = "UNKNOWN";
  this->deviceSample.sdCardVolumeSize = 0;
  this->deviceSample.sdCardClusterCount = 0;
  this->deviceSample.sdCardBlocksPerCluster = 0;
  this->deviceSample.sdCardTotalBlocks = 0;
  this->deviceSample.temperature = this->ABSOLUTE_ZERO;

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
  Serial.print(this->realTimeClock.nowAsISOString());
  Serial.println("OpenAquariumRTOS2::sensorsCalibration()");
  this->dht22Task(this->INTERVAL_24H);
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
    Serial.print(this->realTimeClock.nowAsISOString());
    Serial.println(F(" DISCOVERY"));
  }
}

void OpenAquariumRTOS2::periodicTask(unsigned long currentMillis) {
  if (currentMillis - this->previousPeriodicMillis >= this->periodicInterval) {
    this->previousPeriodicMillis = currentMillis;
    Serial.print(this->realTimeClock.nowAsISOString());
    Serial.println(F(" PERIODIC"));
  }
}

void OpenAquariumRTOS2::dht22Task(unsigned long currentMillis) {
  if (currentMillis - this->previousDht22Millis >= this->dht22Interval) {
    Serial.print(this->realTimeClock.nowAsISOString());
    Serial.println(" OpenAquariumRTOS2::dht22Task()");
    this->previousDht22Millis = currentMillis;
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

void OpenAquariumRTOS2::rtcSynchronizationTask(unsigned long currentMillis) {
  if (currentMillis - this->previousRtcSynchronizationMillis >= this->rtcSynchronizationInterval) {
    Serial.print(this->realTimeClock.nowAsISOString());
    Serial.println(" OpenAquariumRTOS2::rtcSynchronizationTask()");
    this->previousRtcSynchronizationMillis = currentMillis;
    unsigned long epoch = this->wifi.getNTPDate();
    // TODO is NTP time correct?
    if(epoch > 1629665697 && epoch < 2576350497) {
      // TODO only adjust RTC when the difference is greater then or equal to 1 min
      struct tm  ts = Converter::epochToTmStruct(epoch);
      this->realTimeClock.adjust(DateTime(1900+ts.tm_year, ts.tm_mon+1, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec));
    }
  }
}
