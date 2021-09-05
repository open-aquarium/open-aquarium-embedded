#ifndef OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H
#define OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H

typedef struct {
  float roomTemperature; // Celcius DHT2 sensor
  float roomTemperature2; // Celcius / BMP sensor
  float relativeHumidity; // %
  float heatIndex; // Celcius (C)
  float atmosphericPressure; // Pascal (Pa)
  float altitude; // Meters (m)
  float lightIntensity; // ???
  float noiseLevel; // ???
} EnvironmentSampleBlock;

#endif
