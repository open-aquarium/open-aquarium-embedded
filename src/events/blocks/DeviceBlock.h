#ifndef OA_EVENT_DEVICE_BLOCK_H
#define OA_EVENT_DEVICE_BLOCK_H

#include <Arduino.h>

typedef struct {
  String serialNumber;
  String softwareVersion;
  String hardwareVersion;

  String cpu;
  uint8_t cpuSpeed;

  uint16_t totalEEPROM;
  uint32_t totalFlash;
  uint32_t totalMemory;

  String sdCardType;
  String sdCardVolumeType;
  uint32_t sdCardVolumeSize;
  uint32_t sdCardClusterCount;
  uint8_t sdCardBlocksPerCluster;
  uint32_t sdCardTotalBlocks;
} DeviceBlock;

#endif
