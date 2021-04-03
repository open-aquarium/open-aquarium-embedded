#ifndef OA_EVENT_DEVICE_SAMPLE_BLOCK_H
#define OA_EVENT_DEVICE_SAMPLE_BLOCK_H

typedef struct {
  uint8_t freeMemory;
  String sdCardType;
  String sdCardVolumeType;
  uint32_t sdCardVolumeSize;
  uint32_t sdCardClusterCount;
  uint8_t sdCardBlocksPerCluster;
  uint32_t sdCardTotalBlocks;
} DeviceSampleBlock;

#endif
