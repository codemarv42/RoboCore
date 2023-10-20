#include "Arduino.h"
#include "robot.h"


Robot robot = Robot();


void setup() {
  robot.init();
  robot.run();
}

void loop() {
  Serial.println("This is a test!");
  // return;
  delay(1000);
}