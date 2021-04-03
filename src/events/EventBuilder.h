#ifndef OA_EVENT_BUILDER_H
#define OA_EVENT_BUILDER_H

#include <Arduino.h>

class EventBuilder {
  private:
    void buildHeaderBlock();
    void buildDeviceBlock();
    void buildRollCallData();
    void buildDeviceSampleBlock();
    void buildEnvironmentSampleBlock();
    void buildWaterSampleBlock();
  public:
    void buildDiscovery();
    void buildPeriodic();
};

#endif
