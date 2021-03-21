#ifndef OA_EVENT_ROLL_CALL_SENSOR_H
#define OA_EVENT_ROLL_CALL_SENSOR_H
#include <Arduino.h>

typedef struct {
  int id;
  String sensor;
  String address;
  String category;
} RollCallSensor;

#endif
