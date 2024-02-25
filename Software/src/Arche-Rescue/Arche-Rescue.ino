#include "Arduino.h"
#include "robot.h"

TaskHandle_t sensorLoopHandle;
// SemaphoreHandle_t xMutex_GYRO = NULL;


void setup() {

  robot.init();

  // xMutex_GYRO = xSemaphoreCreateMutex()
  // /*
  xTaskCreatePinnedToCore(   //Task is assigned to a processor core
    sensorLoop,             // Task function.
    "SensorLoop",           // name of task.
    10000,                  // Stack size of task
    NULL,                   // parameter of the task
    0,                      // priority of the task
    &sensorLoopHandle,      // Task handle to keep track of created task
    0);                     // pin task to core 0
  // */
  robot.actionLoop();
}

void loop() {}


