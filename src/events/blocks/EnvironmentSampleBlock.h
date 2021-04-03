#ifndef OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H
#define OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H

typedef struct {
  float roomTemperature;
  float relativeHumidity;
  float heatIndex;
  float atmosphericPressure;
  float altitude;
} EnvironmentSampleBlock;

#endif
