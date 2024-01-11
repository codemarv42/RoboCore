
#pragma once
#include <cstdint>
#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>
#include <ESP32Servo.h>

extern Servo rottof;

struct triangleData{
  uint16_t upper[180] = {0};
  uint16_t lower[180] = {0};
};

namespace tof{
  extern void init();
  extern VL53L1X left, claw, turnable_upper, turnable_lower;
  extern uint16_t readLeft();
  extern uint16_t readUpper();
  extern uint16_t readLower();
  extern triangleData* readPos();
}
