#include "Periodic.h"

struct PeriodicStruct {
  HeaderBlock header;
  DeviceBlock device;
  DeviceSampleBlock deviceSample;
  EnvironmentSampleBlock environmentSample;
  WaterSampleBlock waterSample;
};
