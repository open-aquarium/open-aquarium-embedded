#ifndef OA_EVENT_DEVICE_SAMPLE_BLOCK_H
#define OA_EVENT_DEVICE_SAMPLE_BLOCK_H

typedef struct {
  String cpu;
  uint8_t cpuSpeed;

  uint16_t totalEEPROM;
  uint32_t totalFlash;

  uint32_t freeMemory;
  uint32_t usedMemory;
  uint32_t totalMemory;

  float temperature;

  String sdCardType;
  String sdCardVolumeType;
  uint32_t sdCardVolumeSize;
  uint32_t sdCardClusterCount;
  uint8_t sdCardBlocksPerCluster;
  uint32_t sdCardTotalBlocks;
  uint32_t sdCardFreeSpace;
} DeviceSampleBlock;

#endif
