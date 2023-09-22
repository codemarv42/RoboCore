#include <Arduino.h>
#include "robot.h"


Robot robot = Robot();


void setup() {
  Serial.begin(115200);
  robot.init();
}

void loop() {
  Serial.println("This is a test!");
  // return;
  delay(1000);
}