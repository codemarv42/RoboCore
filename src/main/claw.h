#ifndef CLAW
#define CLAW
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pins.h"

namespace claw{
  

  void setup();
  void up();
  void down();
  void half();

  void open();
  void openwide();
  void close();

  void closeFlap();
  void unloadVictims(int side);
  void divide(int side);
}

#endif
