/*DEVICE*SAMPLE*****************************************************************************/
#ifndef OA_EVENT_DEVICE_SAMPLE_BLOCK_H
#define OA_EVENT_DEVICE_SAMPLE_BLOCK_H
//#include <string>

typedef struct DeviceSampleBlockStruct DeviceSampleBlock;

#endif

struct DeviceSampleBlockStruct {
  int freeMemory;
};
/*ENVIRONMENT*SAMPLE************************************************************************/
#ifndef OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H
#define OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H
//#include <string>

typedef struct EnvironmentSampleBlockStruct EnvironmentSampleBlock;

#endif
struct EnvironmentSampleBlockStruct {
  float roomTemperature;
  float relativeHumidity;
  float atmosphericPressure;
  float altitude;
};
/*WATER*SAMPLE******************************************************************************/
#ifndef OA_EVENT_WATER_SAMPLE_BLOCK_H
#define OA_EVENT_WATER_SAMPLE_BLOCK_H
//#include <string>

typedef struct WaterSampleBlockStruct WaterSampleBlock;

#endif

struct WaterSampleBlockStruct {
  float temperature1;
  float temperature2;
  float totalDissolvedSolids;
  boolean waterLevelLow;
  boolean waterLevelMedium;
  boolean waterLevelHigh;
};
/*HEADER**********************************************************************************/
#ifndef OA_EVENT_HEADER_BLOCK_H
#define OA_EVENT_HEADER_BLOCK_H
//#include <string>

typedef struct HeaderBlockStruct HeaderBlock;

#endif

struct HeaderBlockStruct {
  String type;
  String eventId;
  String triggerTime;
};
/*DEVICE**********************************************************************************/
#ifndef OA_EVENT_DEVICE_BLOCK_H
#define OA_EVENT_DEVICE_BLOCK_H
//#include <string>

typedef struct DeviceBlockStruct DeviceBlock;

#endif
struct DeviceBlockStruct {
  String serialNumber;
  String softwareVersion;
  String hardwareVersion;
};
/*PERIODIC********************************************************************************/
#ifndef OA_EVENT_PERIODIC_H
#define OA_EVENT_PERIODIC_H
typedef struct PeriodicStruct Periodic;

#endif

struct PeriodicStruct {
  HeaderBlock header;
  DeviceBlock device;
  DeviceSampleBlock deviceSample;
  EnvironmentSampleBlock environmentSample;
  WaterSampleBlock waterSample;
};

/*ROLL*CALL*SENSOR*************************************************************************/
#ifndef OA_EVENT_ROLL_CALL_SENSOR_H
#define OA_EVENT_ROLL_CALL_SENSOR_H
//#include <string>

typedef struct RollCallSensorStruct RollCallSensor;

#endif
struct RollCallSensorStruct {
  int id;
  String sensor;
  String address;
  String category;
};
/*ROLL*CALL*SENSOR*************************************************************************/
#ifndef OA_EVENT_ROLL_CALL_DATA_BLOCK_H
#define OA_EVENT_ROLL_CALL_DATA_BLOCK_H
typedef struct RollCallDataBlockStruct RollCallDataBlock;

#endif
struct RollCallDataBlockStruct {
  RollCallSensor sensors[10];
};
/*****************************************************************************************/


/*****************************************************************************************/
DeviceSampleBlock deviceSample1;
void deviceSampleTest()
{
  Serial.println("TEST - Device Sample");

  deviceSample1.freeMemory = 127;
  
  Serial.println(deviceSample1.freeMemory);
  
  Serial.println("TEST - Device Sample - PASS");
}
/*****************************************************************************************/
EnvironmentSampleBlock environmentSample1;
void environmentSampleTest()
{
  Serial.println("TEST - Environment Sample");

  environmentSample1.roomTemperature = 28;
  environmentSample1.relativeHumidity = 80;
  environmentSample1.atmosphericPressure = 67;
  environmentSample1.altitude = 140;
  
  Serial.println(environmentSample1.roomTemperature);
  Serial.println(environmentSample1.relativeHumidity);
  Serial.println(environmentSample1.atmosphericPressure);
  Serial.println(environmentSample1.altitude);
  
  Serial.println("TEST - Environment Sample - PASS");
}
/*****************************************************************************************/
WaterSampleBlock waterSample1;
void waterSampleTest()
{
  Serial.println("TEST - Water Sample");

  waterSample1.temperature1 = 24.5;
  waterSample1.temperature2 = 25;
  waterSample1.totalDissolvedSolids = 250.1;
  waterSample1.waterLevelLow = false;
  waterSample1.waterLevelMedium = false;
  waterSample1.waterLevelHigh = true;
  
  Serial.println(waterSample1.temperature1);
  Serial.println(waterSample1.temperature2);
  Serial.println(waterSample1.totalDissolvedSolids);
  Serial.println(waterSample1.waterLevelLow);
  Serial.println(waterSample1.waterLevelMedium);
  Serial.println(waterSample1.waterLevelHigh);
  
  Serial.println("TEST - Water Sample - PASS");
}
/*****************************************************************************************/
HeaderBlock header1;
void headerTest()
{
  Serial.println("TEST - Header");

  header1.type = "PERIODIC/DISCOVERY";
  header1.eventId = "383c44a4-adf7-4831-a4f5-f4019087e8db";
  header1.triggerTime = "2020-11-29T03:48:31.218Z";
  
  Serial.println(header1.type);
  Serial.println(header1.eventId);
  Serial.println(header1.triggerTime);
  
  Serial.println("TEST - Header - PASS");
}
/*****************************************************************************************/
DeviceBlock device1;
void deviceTest()
{
  Serial.println("TEST - Device");

  device1.serialNumber = "A123";
  device1.softwareVersion = "1.0.0";
  device1.hardwareVersion = "1.1.0";
  
  Serial.println(device1.serialNumber);
  Serial.println(device1.softwareVersion);
  Serial.println(device1.hardwareVersion);
  
  Serial.println("TEST - Device - PASS");
}
/*****************************************************************************************/
Periodic periodic1;
void periodicTest()
{
  Serial.println("TEST - Periodic");

  periodic1.header = header1;
  periodic1.device = device1;
  periodic1.deviceSample = deviceSample1;
  periodic1.environmentSample = environmentSample1;
  periodic1.waterSample = waterSample1;
  
  Serial.println(periodic1.header.triggerTime);
  Serial.println(periodic1.device.softwareVersion);
  Serial.println(periodic1.deviceSample.freeMemory);
  Serial.println(periodic1.environmentSample.altitude);
  Serial.println(periodic1.waterSample.waterLevelHigh);
  
  Serial.println("TEST - Periodic - PASS");
}
/*****************************************************************************************/
RollCallSensor rollCallSensor1;
void rollCallSensorTest()
{
  Serial.println("TEST - Roll Call Sensor");

  rollCallSensor1.id = 20000014;
  rollCallSensor1.sensor = "ESP";
  rollCallSensor1.address = "D1";
  rollCallSensor1.category = "environment";
  
  Serial.println(rollCallSensor1.id);
  Serial.println(rollCallSensor1.sensor);
  Serial.println(rollCallSensor1.address);
  Serial.println(rollCallSensor1.category);
  
  Serial.println("TEST - Roll Call Sensor - PASS");
}
/*****************************************************************************************/
RollCallDataBlock rollCallData1;
void rollCallDataTest()
{
  Serial.println("TEST - Roll Call Data");

  rollCallData1.sensors[0] = rollCallSensor1;
  
  Serial.println(rollCallData1.sensors[0].sensor);
  
  Serial.println("TEST - Roll Call Data - PASS");
}
/*****************************************************************************************/
void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
/*****************************************************************************************/
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
/*****************************************************************************************/
  waterSampleTest();
  deviceSampleTest();
  environmentSampleTest();
  headerTest();
  deviceTest();
  periodicTest();
  rollCallSensorTest();
  rollCallDataTest();
}
