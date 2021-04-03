#include "EventBuilder.h"

void EventBuilder::buildDiscovery() {
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
}

static String EventBuilder::discoveryToJSON(DiscoveryEvent discoveryEvent) {
  // Serial.println("EventBuilder::discoveryToJSON()");
  // return "{ discovery: 123}";
  String json = F("");
  json += F("{");
  json += EventBuilder::headerBlockToJSON(discoveryEvent.header);
  json += EventBuilder::deviceBlockToJSON(discoveryEvent.device);
  json += EventBuilder::rollCallDataBlockToJSON(discoveryEvent.rollCallData);
  json += F("}");
  return json;
}

static String EventBuilder::periodicToJSON(PeriodicEvent periodicEvent) {
  // Serial.println("EventBuilder::periodicToJSON()");
  // return "{ periodic: 456}";
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
  String json = F("");
  json += F("\"deviceSample\": {");
  json += F("\"freeMemory\": ");
  json += deviceSample.freeMemory;
  json += F(",");

  json += F("\"sdCardType\": \"");
  json += deviceSample.sdCardType;
  json += F("\",");
  json += F("\"sdCardVolumeType\": \"");
  json += deviceSample.sdCardVolumeType;
  json += F("\",");
  json += F("\"sdCardClusterCount\": ");
  json += deviceSample.sdCardClusterCount;
  json += F(",");
  json += F("\"sdCardBlocksPerCluster\": ");
  json += deviceSample.sdCardBlocksPerCluster;
  json += F(",");
  json += F("\"sdCardVolumeSize\": ");
  json += deviceSample.sdCardVolumeSize;
  // json += F(",");
  
  json += F("},");
  return json;
}

static String EventBuilder::environmentSampleBlockToJSON(EnvironmentSampleBlock environmentSample) {
  String json = F("");
  json += F("\"environmentSample\": {");
  json += F("\"roomTemperature\": ");
  json += environmentSample.roomTemperature;
  json += F(",");
  json += F("\"relativeHumidity\": ");
  json += environmentSample.relativeHumidity;
  json += F(",");
  json += F("\"heatIndex\": ");
  json += environmentSample.heatIndex;
  json += F(",");
  json += F("\"atmosphericPressure\": ");
  json += environmentSample.atmosphericPressure;
  json += F(",");
  json += F("\"altitude\": ");
  json += environmentSample.altitude;
  // json += F(",");
  json += F("},");
  return json;
}

static String EventBuilder::waterSampleBlockToJSON(WaterSampleBlock waterSample) {
  String json = F("");
  json += F("\"waterSample\": {");
  json += F("\"temperature1\": ");
  json += waterSample.temperature1;
  json += F(",");
  json += F("\"temperature2\": ");
  json += waterSample.temperature2;
  json += F(",");
  json += F("\"totalDissolvedSolids\": ");
  json += waterSample.totalDissolvedSolids;
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
