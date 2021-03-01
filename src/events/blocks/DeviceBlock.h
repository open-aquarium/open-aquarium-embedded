#ifndef OA_EVENT_DEVICE_BLOCK_H
#define OA_EVENT_DEVICE_BLOCK_H
#include <Arduino.h>

//struct DeviceBlock;
typedef struct {
  String serialNumber;
  String softwareVersion;
  String hardwareVersion;
} DeviceBlock;

#endif
