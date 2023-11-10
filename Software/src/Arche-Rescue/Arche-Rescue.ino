#include "Arduino.h"
#include "robot.h"

TaskHandle_t sensorLoopHandle;
Robot robot = Robot();


void setup() {

 xTaskCreatePinnedToCore(   //Task is assigned to a processor core
    sensorLoop,             /* Task function. */
    "SensorLoop",           /* name of task. */
    10000,                  /* Stack size of task */
    NULL,                   /* parameter of the task */
    0,                      /* priority of the task */
    &sensorLoopHandle,      /* Task handle to keep track of created task */
    0);                     /* pin task to core 0 */

  robot.init();
  robot.run();
}

void loop() {
   
}

void sensorLoop(void* pvParameters) {}