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
SemaphoreHandle_t SerialMutex;

/*******************************************************************************
 * Open Aquarium - https://open-aquarium.github.io/
 *******************************************************************************/
#include "src/OpenAquariumRTOS2.h"
OpenAquariumRTOS2 app;

/*******************************************************************************
 * CUSTOM
 *******************************************************************************/
void print(const char *message) {
    if(xSemaphoreTake(SerialMutex, portMAX_DELAY) == pdTRUE) {
        Serial.print(message);
        xSemaphoreGive(SerialMutex);
    }
}

void println(const char *message) {
    if(xSemaphoreTake(SerialMutex, portMAX_DELAY) == pdTRUE) {
        Serial.println(message);
        xSemaphoreGive(SerialMutex);
    }
}

/*******************************************************************************
 * ARDUINO
 *******************************************************************************/
void setup() {
  app.setup();
  
  SerialMutex = xSemaphoreCreateMutex();
  println("Started: FreeRTOS Serial Semaphore");

  xTaskCreate(
    activityLedTask,
    "activityLedTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: FreeRTOS Activity Led Task");

  xTaskCreate(
    discoveryProducerTask,
    "discoveryProducerTask",
    128,
    NULL,
    10,
    NULL);
  println("Created: FreeRTOS Discovery Producer Task");

  xTaskCreate(
    periodicProducerTask,
    "periodicProducerTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: FreeRTOS Periodic Producer Task");

  xTaskCreate(
    synchronizeRTCTask,
    "synchronizeRTCTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: FreeRTOS RTC/NTP synchronization Task");

  xTaskCreate(
    reconnectWiFiTask,
    "reconnectWiFiTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: FreeRTOS Wi-Fi reconnection Task");

  xTaskCreate(
    readDHTTask,
    "readDHTTask",
    128,
    NULL,
    1,
    NULL);
  println("Created: DHT reader Task");

  println("Open Aquarium (RTOS) Ready :D");
}

/**
 * Empty loop, not used with RTOS
 */
void loop() {}

/*******************************************************************************
 * RTOS - EVENT TASKS
 *******************************************************************************/
void activityLedTask(void *arg) {
  while(1) {
    app.blinkActivityLed();
    vTaskDelay(pdMS_TO_TICKS(app.ACTIVITY_LED_INTERVAL));
  }
  vTaskDelete(NULL);
}

void discoveryProducerTask(void *arg) {
  while(1) {
//      xQueueSend(discoveryQueue, discoveryCounter++, 0);
    app.produceDiscovery();
    vTaskDelay(pdMS_TO_TICKS(app.DISCOVERY_INTERVAL));
  }
  vTaskDelete(NULL);
}

void periodicProducerTask(void *arg) {
    while(1) {
//      xQueueSend(periodicQueue, &myString, 0);
//      xQueueOverwrite(periodicQueue, (void *)"PERIODIC");
      app.producePeriodic();
      vTaskDelay(pdMS_TO_TICKS(app.PERIODIC_INTERVAL));
    }
    vTaskDelete(NULL);
}

/*******************************************************************************
 * NEXT
 *******************************************************************************/
void synchronizeRTCTask(void *arg) {
    // TODO create a task to update RTC with an NTP server
    while(1) {
      app.synchronizeRTC();
      vTaskDelay(pdMS_TO_TICKS(app.NTP_INTERVAL));
    }
    vTaskDelete(NULL);
}

void reconnectWiFiTask(void *arg) {
    // TODO create a task to recconect to the Wi-Fi when necessary
    while(1) {
      println("TODO reconnectWiFiTask()");
//      app.wifi.connect();
      vTaskDelay(pdMS_TO_TICKS(app.WIFI_RECONNECTION_INTERVAL));
    }
    vTaskDelete(NULL);
}

void readDHTTask(void *arg) {
    while(1) {
      float temperature = app.dht.readTemperature();
//      float humidity = app.dht.readHumidity();
//      float heatIndex = app.dht.computeHeatIndex(
//        temperature,
//        humidity, 
//        false
//      );
      String sample = F("DHT ===> ");
//      sample+= F("Temperature: ");
//      sample += temperature;
//      sample += F(" , ");
//      sample += F("Relative Humidity: ");
//      sample += humidity;
//      sample += F(" , ");
//      sample += F("Heat Index: ");
//      sample += heatIndex;
      Serial.println(sample);
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelete(NULL);
}

/*******************************************************************************
 * END
 *******************************************************************************/
