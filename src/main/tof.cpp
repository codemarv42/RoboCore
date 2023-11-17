
#include <Arduino.h>
#include <VL53L1X.h>
#include <Wire.h>
#include "Pins.h"

#include "tof.h"

namespace tof{
  VL53L1X left;
  VL53L1X claw;
  VL53L1X turnable_upper;
  VL53L1X turnable_lower;

  void init(){
    left.startContinuous(300);
    claw.startContinuous(300);
    turnable_upper.startContinuous(300);
    turnable_lower.startContinuous(300);
  }
}



