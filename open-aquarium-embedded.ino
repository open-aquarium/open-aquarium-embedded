/*#include "src/OpenAquarium.h"

OpenAquarium app;

void setup() {
  app.setup();
}

void loop() {
  app.loop();
}*/

/*******************************************************************************
 * FreeRTOS - https://www.freertos.org/
 *******************************************************************************/
#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "semphr.h"
SemaphoreHandle_t   SerialMutex;

/*******************************************************************************
 * Open Aquarium - https://open-aquarium.github.io/
 *******************************************************************************/
//#define OA_ENVIRONMENT 1 // 1=prod
//#define OA_DEBUG       1 // 1=true

//#include "src/OpenAquarium.h"
//OpenAquarium app;
#include "src/OpenAquariumRTOS.h"
OpenAquariumRTOS app;

/*******************************************************************************
 * CUSTOM
 *******************************************************************************/
void print(const char *message) {
    if(xSemaphoreTake(SerialMutex, portMAX_DELAY) == pdTRUE) {
        //Serial.print("\t\t");
        Serial.print(message);
        xSemaphoreGive(SerialMutex);
    }
}
void println(const char *message) {
    if(xSemaphoreTake(SerialMutex, portMAX_DELAY) == pdTRUE) {
        //Serial.print("\t\t");
        Serial.println(message);
        xSemaphoreGive(SerialMutex);
    }
}

/*******************************************************************************
 * ARDUINO
 *******************************************************************************/
void setup() {
  app.productionMode = false;
  app.debug = true;
  app.setup();

  SerialMutex = xSemaphoreCreateMutex();
  println("Started: FreeRTOS Serial Semaphore.");

  xTaskCreate(
    activityLedTask,
    "activityLedTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: FreeRTOS Activity Led Task.");

  xTaskCreate(
    discoveryProducerTask,
    "discoveryProducerTask",
    128,
    NULL,
    10,
    NULL);
  println("Created: FreeRTOS Discovery Producer Task.");

  xTaskCreate(
    periodicProducerTask,
    "periodicProducerTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: FreeRTOS Periodic Producer Task.");

  println("Open Aquarium (RTOS) Ready :D");
}

void loop() {}

/*******************************************************************************
 * RTOS - EVENT TASKS
 *******************************************************************************/
void activityLedTask(void *arg) {
  if(!app.isSystemReady()) {
    app.blinkActivityLed(true);
    return;
  }
  boolean on = false;
  while(1) {
    on = !on;
    app.blinkActivityLed(on);
    vTaskDelay(pdMS_TO_TICKS(app.ACTIVITY_LED_INTERVAL));
  }
}

void discoveryProducerTask(void *arg) {
  if(!app.isSystemReady()) {
    app.blinkActivityLed(true);
    return;
  }
  unsigned int stime = millis();
  while(1) {
    stime = millis();
//      xQueueSend(discoveryQueue, discoveryCounter++, 0);
    unsigned int etime = millis();
    Serial.print(app.realTimeClock.nowAsISOString());
    println(" DISCOVERY producer");
    vTaskDelay(pdMS_TO_TICKS(app.DISCOVERY_INTERVAL));
  }
  vTaskDelete(NULL);
}

void periodicProducerTask(void *arg) {
  if(!app.isSystemReady()) {
    app.blinkActivityLed(true);
    return;
  }
  unsigned int stime = millis();
    while(1) {
      stime = millis();
//      xQueueSend(periodicQueue, &myString, 0);
//      xQueueOverwrite(periodicQueue, (void *)"PERIODIC");
      unsigned int etime = millis();
      Serial.print(app.realTimeClock.nowAsISOString());
      println(" PERIODIC producer");
      vTaskDelay(pdMS_TO_TICKS(app.PERIODIC_INTERVAL));
    }
    vTaskDelete(NULL);
}

/*******************************************************************************
 * NEXT
 *******************************************************************************/

/*******************************************************************************
 * END
 *******************************************************************************/
