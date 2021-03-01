#ifndef OA_EVENT_ROLL_CALL_DATA_BLOCK_H
#define OA_EVENT_ROLL_CALL_DATA_BLOCK_H
#include "RollCallSensor.h"

typedef struct {
  RollCallSensor sensors[10];
} RollCallDataBlock;

#endif
