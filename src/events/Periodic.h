#ifndef OA_EVENT_PERIODIC_H
#define OA_EVENT_PERIODIC_H

#include "blocks/HeaderBlock.h"
#include "blocks/DeviceBlock.h"
#include "blocks/DeviceSampleBlock.h"
#include "blocks/EnvironmentSampleBlock.h"
#include "blocks/WaterSampleBlock.h"

typedef struct PeriodicStruct {
  HeaderBlock header;
  DeviceBlock device;
  DeviceSampleBlock deviceSample;
  EnvironmentSampleBlock environmentSample;
  WaterSampleBlock waterSample;
} PeriodicEvent;

#endif
