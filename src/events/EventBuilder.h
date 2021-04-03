#ifndef OA_EVENT_BUILDER_H
#define OA_EVENT_BUILDER_H

#include <Arduino.h>
// #include <sstream>

#include "../util/limits.h"
#include "../util/converter.h"

#include "Discovery.h"
#include "Periodic.h"
#include "blocks/HeaderBlock.h"
#include "blocks/DeviceBlock.h"
#include "blocks/RollCallDataBlock.h"
#include "blocks/RollCallSensor.h"
#include "blocks/DeviceSampleBlock.h"
#include "blocks/EnvironmentSampleBlock.h"
#include "blocks/WaterSampleBlock.h"

class EventBuilder {
  private:
    /*void buildHeaderBlock();
    void buildDeviceBlock();
    void buildRollCallData();
    void buildDeviceSampleBlock();
    void buildEnvironmentSampleBlock();
    void buildWaterSampleBlock();*/
    static String headerBlockToJSON(HeaderBlock header);
    static String deviceBlockToJSON(DeviceBlock device);
    static String rollCallDataBlockToJSON(RollCallDataBlock rollCallData);
    static String deviceSampleBlockToJSON(DeviceSampleBlock deviceSample);
    static String environmentSampleBlockToJSON(EnvironmentSampleBlock environmentSample);
    static String waterSampleBlockToJSON(WaterSampleBlock waterSample);

    static bool checkNullFloat(float data);
    static bool checkNullUInt8(uint8_t data);
    static bool checkNullUInt16(uint16_t data);
    static bool checkNullUInt32(uint32_t data);
    static bool checkNullUInt64(uint64_t data);
    static bool checkNullString(String data);
  public:
    void buildDiscovery();
    void buildPeriodic();
    static String discoveryToJSON(DiscoveryEvent discoveryEvent);
    static String periodicToJSON(PeriodicEvent periodicEvent);
};

#endif
