#ifndef OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H
#define OA_EVENT_ENVIRONMENT_SAMPLE_BLOCK_H

typedef struct {
  float roomTemperature; // Celcius DHT2 sensor
  float roomTemperature2; // Celcius / BMP sensor
  float relativeHumidity; // %
  float heatIndex; // Celcius
  float atmosphericPressure; // Pascal
  float altitude; // Meters
  float lightIntensity; // ???
  float noiseLevel; // ???
} EnvironmentSampleBlock;

#endif
