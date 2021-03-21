#ifndef OA_EVENT_WATER_SAMPLE_BLOCK_H
#define OA_EVENT_WATER_SAMPLE_BLOCK_H

typedef struct {
  float temperature1;
  float temperature2;
  float totalDissolvedSolids;
  bool waterLevelLow;
  bool waterLevelMedium;
  bool waterLevelHigh;
} WaterSampleBlock;

#endif
