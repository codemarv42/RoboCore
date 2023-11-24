#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>

//extern VL53L1X left, claw, turnable_upper, turnable_lower;
namespace tof{
  extern void init();
  extern VL53L1X left, claw, turnable_upper, turnable_lower;
}
