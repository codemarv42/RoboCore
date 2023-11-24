
#include <Arduino.h>
#include <VL53L1X.h>
#include <Wire.h>
#include "Pins.h"
#include "shiftregister.h"

#include "tof.h"

namespace tof{
  VL53L1X left;
  VL53L1X claw;
  VL53L1X turnable_upper;
  VL53L1X turnable_lower;

  void init(){

    left.setAddress(TOF_ADRESS);
    left.setTimeout(500);
    claw.setAddress(TOF_ADRESS);
    claw.setTimeout(500);
    turnable_upper.setAddress(TOF_ADRESS);
    turnable_upper.setTimeout(500);
    turnable_lower.setAddress(TOF_ADRESS);
    turnable_lower.setTimeout(500);
    
    shift_register::write(SR_XSHT1, HIGH);
    delay(10);
    if (!left.init()){Serial.print("dumm");}
    
    Serial.println("All tof Sensors active!");

    left.startContinuous(50);
    claw.startContinuous(50);
    turnable_upper.startContinuous(50);
    turnable_lower.startContinuous(50);
  }
}



