/*
  Open Aquarium - Data logger
*/

/**
 * DEVICE
 **/
const int INBUILD_LED = 13;
const int STATUS_LED = 12;
const int DEVICE_STATUS_LED_RED = 12;
const int DEVICE_STATUS_LED_BLUE = 11;
const int DEVICE_STATUS_LED_GREEN = 10;
const int DEVICE_TEMPERATURE_SENSOR = A0;

/**
 * ROOM
 **/
const int ROOM_TEMPERATURE_SENSOR = A3;

/**
 * WATER
 **/
const int WATER_TEMPERATURE_SENSOR = A5;

const String deviceSerialNumber = "1234567890";
const String hardwareVersion = "0.1.0";
const String softwareVersion = "0.1.1";

int readyState = 0;
int batteryPowered = 0;
int halt = 0;

int discoveryMockCounter = 0;
int turnOffMockCounter = 0;
const int turnOffMockThreshold = 3;

void turnStatusLedOff() {
    digitalWrite(DEVICE_STATUS_LED_RED, LOW);
    digitalWrite(DEVICE_STATUS_LED_GREEN, LOW);
    digitalWrite(DEVICE_STATUS_LED_BLUE, LOW);
}

void turnStatusLedWhite() {
    digitalWrite(DEVICE_STATUS_LED_RED, HIGH);
    digitalWrite(DEVICE_STATUS_LED_GREEN, HIGH);
    digitalWrite(DEVICE_STATUS_LED_BLUE, HIGH);
}

void turnStatusLedRed() {
    //log("turn status led RED");
    digitalWrite(DEVICE_STATUS_LED_RED, HIGH);
    digitalWrite(DEVICE_STATUS_LED_GREEN, LOW);
    digitalWrite(DEVICE_STATUS_LED_BLUE, LOW);
}

void turnStatusLedGreen() {
    //log("turn status led GREEN");
    digitalWrite(DEVICE_STATUS_LED_RED, LOW);
    digitalWrite(DEVICE_STATUS_LED_GREEN, HIGH);
    digitalWrite(DEVICE_STATUS_LED_BLUE, LOW);
}

void turnStatusLedBlue() {
    //log("turn status led BLUE");
    digitalWrite(DEVICE_STATUS_LED_RED, LOW);
    digitalWrite(DEVICE_STATUS_LED_GREEN, LOW);
    digitalWrite(DEVICE_STATUS_LED_BLUE, HIGH);
}

void turnStatusLedYellow() {
    //log("turn status led YELLOW");
    digitalWrite(DEVICE_STATUS_LED_RED, HIGH);
    digitalWrite(DEVICE_STATUS_LED_GREEN, HIGH);
    digitalWrite(DEVICE_STATUS_LED_BLUE, LOW);
}

void calibrate() {
  log("calibrate");
  for (int i = 0; i <= 1000; i++) {
    delay(10);
  }
  turnStatusLedGreen();
}

bool triggerDiscovery() {
  discoveryMockCounter += 1;
  return discoveryMockCounter < 2;
}

void discoveryEvent() {
  logEventSimple("{ ");
  logEventSimple(createHeaderBlock("DISCOVERY") + ", ");
  logEventSimple(createDeviceBlock());
  logEvent(" }");
}

void turnOnEvent() {
  logEventSimple("{ ");
  logEventSimple(createHeaderBlock("TURN_ON") + ", ");
  logEventSimple(createDeviceBlock() + ", ");
  logEventSimple(createDeviceSampleBlock());
  logEvent(" }");
  readyState = 1;
}

void turnOffEvent() {
  logEventSimple("{ ");
  logEventSimple(createHeaderBlock("TURN_OFF") + ", ");
  logEventSimple(createDeviceBlock() + ", ");
  logEventSimple(createDeviceSampleBlock());
  logEvent(" }");
  halt = 1;
  digitalWrite(13, HIGH);
}

void periodicEvent() {
  //createWaterSampleBlock
  String waterTemperature = "temperature: ";
  waterTemperature.concat(readWaterTemperature());
  
  String waterPotentialOfHydrogen = "potentialOfHydrogen: ";
  waterPotentialOfHydrogen.concat(readWaterPotentialOfHydrogen());
  
  String waterAmmonia = "ammonia: ";
  waterAmmonia.concat(readWaterAmmonia());
  
  String waterNitrite = "nitrite: ";
  waterNitrite.concat(readWaterNitrite());
  
  String waterNitrate = "nitrate: ";
  waterNitrate.concat(readWaterNitrate());
  
  String waterTotalDissolvedSolids = "totalDissolvedSolids: ";
  waterTotalDissolvedSolids.concat(readWaterTotalDissolvedSolids());
  
  String waterDissolvedOxygen = "dissolvedOxygen: ";
  waterDissolvedOxygen.concat(readWaterDissolvedOxygen());
  
  String eventWaterSample = "waterSample: { ";
  eventWaterSample += waterTemperature + ", ";
  eventWaterSample += waterPotentialOfHydrogen + ", ";
  String eventWaterSample2 = waterAmmonia + ", ";
  eventWaterSample2 += waterNitrite + ", ";
  eventWaterSample2 += waterNitrate + ", ";
  String eventWaterSample3 = waterTotalDissolvedSolids + ", ";
  eventWaterSample3 += waterDissolvedOxygen;
  eventWaterSample3 += " }";
  
  logEventSimple("{ ");
  logEventSimple(createHeaderBlock("PERIODIC") + ", ");
  logEventSimple(createDeviceBlock() + ", ");
  logEventSimple(createDeviceSampleBlock() + ", ");
  logEventSimple(createRoomSampleBlock() + ", ");
  logEventSimple(eventWaterSample);
  logEventSimple(eventWaterSample2);
  logEventSimple(eventWaterSample3);
  logEvent(" }");

  /* DEBUG
  String deviceTemperature = "Device temperature: ";
  deviceTemperature.concat(readDeviceTemperature());
  logEvent(deviceTemperature);

  String roomTemperature = "Room temperature: ";
  roomTemperature.concat(readRoomTemperature());
  logEvent(roomTemperature);

  String waterTemperature123 = "Water temperature: ";
  waterTemperature123.concat(readWaterTemperature());
  logEvent(waterTemperature123);*/
}

void log(String message) {
  Serial.println(message);
}

void logEvent(String message) {
  Serial.println(message);
}

void logEventSimple(String message) {
  Serial.print(message);
}

String getDSN() {
  return deviceSerialNumber;
}

String getSwVersion() {
  return softwareVersion;
}

String getHwVersion() {
  return hardwareVersion;
}

String generateEventId() {
  long randomNumber = random(1, 100);
  String id = "ABC";
  id.concat(randomNumber);
  return id;
}

String getCurrentDate() {
  // https://www.circuitbasics.com/using-an-arduino-ethernet-shield-for-timekeeping/
  return "2020-01-21";
}

float readDeviceTemperature() {
  if(getHwVersion() == "0.1.0") {
    //return random(20, 70); // mocked value
    float temperatureVoltage = 0;
    temperatureVoltage = (analogRead(DEVICE_TEMPERATURE_SENSOR) * 0.004882814);
    return ((temperatureVoltage - 0.5) * 100);
  } else {
    // only a few of arduino boards have an inbuild sensor
    return readDeviceInbuiltTemperature();
  }
}

float readDeviceInbuiltTemperature() {
  // https://playground.arduino.cc/Main/InternalTemperatureSensor/
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;
  return t;
}

float readRoomTemperature() {
  //return random(20, 40); // mocked value
  float temperatureVoltage = 0;
  //float temperatureC = 0;
  temperatureVoltage = (analogRead(ROOM_TEMPERATURE_SENSOR) * 0.004882814);
  //temperatureC = ((temperatureVoltage - 0.5) * 100);
  return ((temperatureVoltage - 0.5) * 100);
}

float readRoomHumidity() {
  return random(20, 100); // mocked value
}

float readAtmosphericPressure() {
  return random(0, 100); // mocked value
}

float readWaterTemperature() {
  float temperatureVoltage = 0;
  temperatureVoltage = (analogRead(WATER_TEMPERATURE_SENSOR) * 0.004882814);
  return ((temperatureVoltage - 0.5) * 100);
}

float readWaterPotentialOfHydrogen() {
  return random(6, 8);
}

float readWaterAmmonia() {
  return random(0, 5);
}

float readWaterNitrite() {
  return random(0, 5);
}

float readWaterNitrate() {
  return random(0, 5);
}

float readWaterTotalDissolvedSolids() {
  return random(0, 100);
}

float readWaterDissolvedOxygen() {
  return random(0, 100);
}

String createHeaderBlock(String type) {
  String eventHeader = "header: { ";
  eventHeader += "type: " + type + ", ";
  eventHeader += "eventId: " + generateEventId() + ", ";
  eventHeader += "triggerTime: " + getCurrentDate();
  eventHeader += " }";
  return eventHeader;
}

String createDeviceBlock() {
  String eventDevice = "device: { ";
  eventDevice += " serialNumber: " + getDSN() + ", ";
  eventDevice += " hardwareVersion: " + getHwVersion() + ", ";
  eventDevice += " softwareVersion: " + getSwVersion();
  eventDevice += " }";
  return eventDevice;
}

String createDeviceSampleBlock() {
  String deviceTemperature = "temperature: ";
  deviceTemperature.concat(readDeviceTemperature());
  String eventDeviceSample = "deviceSample: { ";
  eventDeviceSample += deviceTemperature;
  eventDeviceSample += " }";
  return eventDeviceSample;
}

String createRoomSampleBlock() {
  String roomTemperature = "temperature: ";
  roomTemperature.concat(readRoomTemperature());
  String roomHumidity = "humidity: ";
  roomHumidity.concat(readRoomHumidity());
  String roomAtmosphericPressure = "atmosphericPressure: ";
  roomAtmosphericPressure.concat(readAtmosphericPressure());
  String eventRoomSample = "roomSample: { ";
  eventRoomSample += roomTemperature + ", ";
  eventRoomSample += roomHumidity + ", ";
  eventRoomSample += roomAtmosphericPressure;
  eventRoomSample += " }";
  return eventRoomSample;
}

String createWaterSampleBlock() {
  /*String eventRoomSample = "waterSample: { ";
  String waterTemperature = "temperature: ";
  waterTemperature.concat(readRoomTemperature());
  eventRoomSample += waterTemperature;
  eventRoomSample += " }";
  return eventRoomSample;*/
  //periodicEvent
  return "{}";
}

void flashStatusLed() {
  if(digitalRead(INBUILD_LED) == HIGH) {
    digitalWrite(INBUILD_LED, LOW);
  } else {
    digitalWrite(INBUILD_LED, HIGH);
  }
  if(digitalRead(DEVICE_STATUS_LED_RED) == HIGH) {
    turnStatusLedOff();
  } else {
    turnStatusLedRed();
  }
}

void statusLedSetError() {
  digitalWrite(INBUILD_LED, HIGH);
  //digitalWrite(STATUS_LED, HIGH);
  turnStatusLedRed();
}

void setup() {
  pinMode(INBUILD_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  pinMode(DEVICE_STATUS_LED_RED, OUTPUT);
  pinMode(DEVICE_STATUS_LED_GREEN, OUTPUT);
  pinMode(DEVICE_STATUS_LED_BLUE, OUTPUT);

  pinMode(DEVICE_TEMPERATURE_SENSOR, INPUT);
  
  pinMode(ROOM_TEMPERATURE_SENSOR, INPUT);

  pinMode(WATER_TEMPERATURE_SENSOR, INPUT);
  turnStatusLedYellow();
  Serial.begin(9600);
  
  calibrate();
}

void loop() {
  if(halt == 1) {
    statusLedSetError();
    return;
  }
  //flashStatusLed();
  turnOffMock();
  if(readyState == 0) {
    turnOnEvent();
  }
  if(triggerDiscovery()) {
    discoveryEvent();
  }
  periodicEvent();
  if(batteryPowered == 1) {
    turnOffEvent();
  }
  delay(1000); // Wait for 1s
}

void turnOffMock() {
  turnOffMockCounter += 1;
  if(turnOffMockCounter == turnOffMockThreshold) {
    batteryPowered = 1;
  }
}
