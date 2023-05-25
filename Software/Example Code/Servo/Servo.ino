#include "Pins.h"

#include <ESP32_Servo.h> //https://github.com/jkb-git/ESP32Servo

Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

void setup() {
  Servo1.attach(SERVO1);
  Servo2.attach(SERVO2);
  Servo3.attach(SERVO3);
  Servo4.attach(SERVO4);
}

void loop() {
  Servo1.write(0);
  Servo2.write(0);
  Servo3.write(0);
  Servo4.write(0);
  delay(1000);
  Servo1.write(90);
  Servo2.write(90);
  Servo3.write(90);
  Servo4.write(90);
  delay(1000);
  Servo1.write(180);
  Servo2.write(180);
  Servo3.write(180);
  Servo4.write(180);
  delay(1000);
}