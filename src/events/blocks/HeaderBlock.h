#ifndef OA_EVENT_HEADER_BLOCK_H
#define OA_EVENT_HEADER_BLOCK_H
#include <Arduino.h>

typedef struct {
  String type;
  String eventId;
  String triggerTime;
} HeaderBlock;

#endif
