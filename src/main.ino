/*
  Open Aquarium - Data logger
*/

const String deviceSerialNumber = "1234567890";
const String hardwareVersion = "0.1.0";
const String softwareVersion = "0.1.1";

int readyState = 0;
int batteryPowered = 0;
int halt = 0;

int discoveryMockCounter = 0;
int turnOffMockCounter = 0;
const int turnOffMockThreshold = 2;

void calibrate() {
  log("calibrate");
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
  return random(20, 70); // mocked value
}

float readRoomTemperature() {
  return random(20, 40); // mocked value
}

float readRoomHumidity() {
  return random(20, 100); // mocked value
}

float readAtmosphericPressure() {
  return random(0, 100); // mocked value
}

float readWaterTemperature() {
  return random(20, 30);
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

void flashStatusLed() {
  if(digitalRead(13) == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
}

void statusLedSetError() {
  digitalWrite(13, HIGH);
}

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  
  calibrate();
}

void loop() {
  if(halt == 1) {
    statusLedSetError();
    return;
  }
  flashStatusLed();
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