#include "EventBuilder.h"

/*void EventBuilder::buildDiscovery() {
  Serial.println("EventBuilder::buildDiscovery()");
}

void EventBuilder::buildPeriodic() {
  Serial.println("EventBuilder::buildPeriodic()");
}

void EventBuilder::buildHeaderBlock() {
  Serial.println("EventBuilder::buildHeaderBlock()");
}

void EventBuilder::buildDeviceBlock() {
  Serial.println("EventBuilder::buildDeviceBlock()");
}

void EventBuilder::buildRollCallData() {
  Serial.println("EventBuilder::buildRollCallData()");
}

void EventBuilder::buildDeviceSampleBlock() {
  Serial.println("EventBuilder::buildDeviceSampleBlock();");
}

void EventBuilder::buildEnvironmentSampleBlock() {
  Serial.println("EventBuilder::buildEnvironmentSampleBlock();");
}

void EventBuilder::buildWaterSampleBlock() {
  Serial.println("EventBuilder::buildWaterSampleBlock()");
}*/

static String EventBuilder::discoveryToJSON(DiscoveryEvent discoveryEvent) {
  // Serial.println(F("EventBuilder::discoveryToJSON()"));
  String json = F("");
  json += F("{");
  json += EventBuilder::headerBlockToJSON(discoveryEvent.header);
  json += EventBuilder::deviceBlockToJSON(discoveryEvent.device);
  json += EventBuilder::rollCallDataBlockToJSON(discoveryEvent.rollCallData);
  json += F("}");
  return json;
}

static String EventBuilder::periodicToJSON(PeriodicEvent periodicEvent) {
  // Serial.println(F("EventBuilder::periodicToJSON()"));
  String json = F("");
  json += F("{");
  json += EventBuilder::headerBlockToJSON(periodicEvent.header);
  json += EventBuilder::deviceBlockToJSON(periodicEvent.device);
  json += EventBuilder::deviceSampleBlockToJSON(periodicEvent.deviceSample);
  json += EventBuilder::environmentSampleBlockToJSON(periodicEvent.environmentSample);
  json += EventBuilder::waterSampleBlockToJSON(periodicEvent.waterSample);
  json += F("}");
  return json;
}

static String EventBuilder::headerBlockToJSON(HeaderBlock header) {
  // Serial.println(F("EventBuilder::headerBlockToJSON()"));
  String json = F("");
  json += F("\"header\": {");
  json += F("\"type\": \"");
  json += header.type;
  json += F("\",");
  json += F("\"eventId\": \"");
  json += header.eventId;
  json += F("\",");
  json += F("\"triggerTime\": \"");
  json += header.triggerTime;
  json += F("\"");
  json += F("},");
  return json;
}

static String EventBuilder::deviceBlockToJSON(DeviceBlock device) {
  // Serial.println(F("EventBuilder::deviceBlockToJSON()"));
  String json = F("");
  json += F("\"device\": {");
  json += F("\"serialNumber\": \"");
  json += device.serialNumber;
  json += F("\",");
  json += F("\"softwareVersion\": \"");
  json += device.softwareVersion;
  json += F("\",");
  json += F("\"hardwareVersion\": \"");
  json += device.hardwareVersion;
  json += F("\"");
  json += F("},");
  return json;
}

static String EventBuilder::rollCallDataBlockToJSON(RollCallDataBlock rollCallData) {
  // Serial.println(F("EventBuilder::rollCallDataBlockToJSON()"));
  String json = F("");
  json += F("\"rollCallData\": {");
  json += F("\"sensors\": [");
  
  String sensors = F("");
  for (int i = 0; i < 10; i++) {
    if(rollCallData.sensors[i].sensor == "") {
      continue;
    }
    if(sensors.length() > 0) {
      sensors += F(",");
    }
    sensors += F("{");
    sensors += F("\"id\":\"");
    sensors += rollCallData.sensors[i].id;
    sensors += F("\",");
    sensors += F("\"sensor\":\"");
    sensors += rollCallData.sensors[i].sensor;
    sensors += F("\",");
    sensors += F("\"address\":\"");
    sensors += rollCallData.sensors[i].address;
    sensors += F("\",");
    sensors += F("\"category\":\"");
    sensors += rollCallData.sensors[i].category;
    sensors += F("\"");
    sensors += F("}");
  }

  if(sensors.length() > 0) {
    json += sensors;
  }

  json += F("]}");

  return json;
}

static String EventBuilder::deviceSampleBlockToJSON(DeviceSampleBlock deviceSample) {
  // Serial.println(F("EventBuilder::deviceSampleBlockToJSON()"));
  String json = F("");
  json += F("\"deviceSample\": {");
  json += F("\"freeMemory\": ");
  // json += deviceSample.freeMemory;
  json += deviceSample.freeMemory == OA_MIN_FLOAT ? F("null") : Converter::toString(deviceSample.freeMemory);
  json += F(",");

  json += F("\"sdCardType\": \"");
  json += deviceSample.sdCardType;
  json += F("\",");
  json += F("\"sdCardVolumeType\": \"");
  json += deviceSample.sdCardVolumeType;
  json += F("\",");
  json += F("\"sdCardClusterCount\": ");
  // json += deviceSample.sdCardClusterCount;
  json += deviceSample.sdCardClusterCount == OA_MIN_FLOAT ? F("null") : Converter::toString(deviceSample.sdCardClusterCount);
  json += F(",");
  json += F("\"sdCardBlocksPerCluster\": ");
  // json += deviceSample.sdCardBlocksPerCluster;
  json += deviceSample.sdCardBlocksPerCluster == OA_MIN_FLOAT ? F("null") : Converter::toString(deviceSample.sdCardBlocksPerCluster);
  json += F(",");
  json += F("\"sdCardVolumeSize\": ");
  // json += deviceSample.sdCardVolumeSize;
  json += deviceSample.sdCardVolumeSize == OA_MIN_FLOAT ? F("null") : Converter::toString(deviceSample.sdCardVolumeSize);
  json += F(",");
  json += F("\"temperature\": ");
  // json += deviceSample.temperature;
  json += deviceSample.temperature == OA_MIN_FLOAT ? F("null") : Converter::toString(deviceSample.temperature);
  // json += F(",");
  
  json += F("},");
  return json;
}

static String EventBuilder::environmentSampleBlockToJSON(EnvironmentSampleBlock environmentSample) {
  // Serial.println(F("EventBuilder::environmentSampleBlockToJSON()"));
  String json = F("");
  json += F("\"environmentSample\": {");
  json += F("\"roomTemperature\": ");
  json += environmentSample.roomTemperature == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.roomTemperature);
  json += F(",");
  json += F("\"roomTemperature2\": ");
  json += environmentSample.roomTemperature2 == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.roomTemperature2);
  json += F(",");
  json += F("\"relativeHumidity\": ");
  json += environmentSample.relativeHumidity == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.relativeHumidity);
  json += F(",");
  json += F("\"heatIndex\": ");
  json += environmentSample.heatIndex == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.heatIndex);
  json += F(",");
  json += F("\"atmosphericPressure\": ");
  json += environmentSample.atmosphericPressure == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.atmosphericPressure);
  json += F(",");
  json += F("\"altitude\": ");
  json += environmentSample.altitude == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.altitude);
  json += F(",");
  json += F("\"lightIntensity\": ");
  json += environmentSample.lightIntensity == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.lightIntensity);
  json += F(",");
  json += F("\"noiseLevel\": ");
  json += environmentSample.noiseLevel == OA_MIN_FLOAT ? F("null") : Converter::toString(environmentSample.noiseLevel);
  // json += F(",");
  json += F("},");
  return json;
}

static String EventBuilder::waterSampleBlockToJSON(WaterSampleBlock waterSample) {
  // Serial.println(F("EventBuilder::waterSampleBlockToJSON()"));
  String json = F("");
  json += F("\"waterSample\": {");
  json += F("\"temperature1\": ");
  json += waterSample.temperature1 == OA_MIN_FLOAT ? F("null") : Converter::toString(waterSample.temperature1);
  json += F(",");
  json += F("\"temperature2\": ");
  json += waterSample.temperature2 == OA_MIN_FLOAT ? F("null") : Converter::toString(waterSample.temperature2);
  json += F(",");
  json += F("\"totalDissolvedSolids\": ");
  json += waterSample.totalDissolvedSolids == OA_MIN_FLOAT ? F("null") : Converter::toString(waterSample.totalDissolvedSolids);
  json += F(",");
  json += F("\"waterLevelLow\": ");
  json += waterSample.waterLevelLow == true ? F("true") : F("false");
  json += F(",");
  json += F("\"waterLevelMedium\": ");
  json += waterSample.waterLevelMedium == true ? F("true") : F("false");
  json += F(",");
  json += F("\"waterLevelHigh\": ");
  json += waterSample.waterLevelHigh == true ? F("true") : F("false");
  // json += F(",");
  json += F("}");
  return json;
}

static bool EventBuilder::checkNullFloat(float data) {
  return data == OA_MIN_FLOAT;
}

static bool EventBuilder::checkNullUInt8(uint8_t data) {
  return data == data == UINT8_MAX;
}

static bool EventBuilder::checkNullUInt16(uint16_t data) {
  return data == UINT16_MAX;
}

static bool EventBuilder::checkNullUInt32(uint32_t data) {
  return data == UINT32_MAX;
}

static bool EventBuilder::checkNullUInt64(uint64_t data) {
  return data == UINT64_MAX;
}

static bool EventBuilder::checkNullString(String data) {
  return data.length() == 0;
}
