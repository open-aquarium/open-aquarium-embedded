#ifndef OA_EVENT_DEVICE_SAMPLE_BLOCK_H
#define OA_EVENT_DEVICE_SAMPLE_BLOCK_H

typedef struct {
  uint32_t freeMemory;
  uint32_t usedMemory;
  
  float temperature;

  uint32_t sdCardFreeSpace;
  uint32_t sdCardUsedSpace;
} DeviceSampleBlock;

#endif
