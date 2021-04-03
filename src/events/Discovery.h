#ifndef OA_EVENT_DISCOVERY_H
#define OA_EVENT_DISCOVERY_H

#include "blocks/HeaderBlock.h"
#include "blocks/DeviceBlock.h"
#include "blocks/RollCallDataBlock.h"

typedef struct DiscoveryStruct {
  HeaderBlock header;
  DeviceBlock device;
  RollCallDataBlock rollCallData;
} DiscoveryEvent;

#endif
