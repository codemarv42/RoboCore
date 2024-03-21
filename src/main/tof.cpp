#include <sys/unistd.h>

#include <Arduino.h>
#include <VL53L1X.h>
#include <Wire.h>
#include "Pins.h"
#include "shiftregister.h"
#include <ESP32Servo.h>

#include "tof.h"

Servo rottof;

namespace tof{
  VL53L1X left;
  VL53L1X claw;
  VL53L1X turnable_upper;
  VL53L1X turnable_lower;

  void init(){

    rottof.attach(18);
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
    if (!left.init()){Serial.print("\tMISSING TOF 1 (Left)");}
    else{
      left.setDistanceMode(VL53L1X::Short);
      left.setMeasurementTimingBudget(33000);
    }
    shift_register::write(SR_XSHT1, LOW, true);
    shift_register::write(SR_XSHT2, HIGH);
    delay(10);
    if (!claw.init()){Serial.print("\tMISSING TOF 2 (Claw)");}
    else{
      claw.setDistanceMode(VL53L1X::Short);
      claw.setMeasurementTimingBudget(33000);
    }
    shift_register::write(SR_XSHT3, HIGH, true);
    shift_register::write(SR_XSHT2, LOW);
    delay(10);
    if (!turnable_upper.init()){Serial.print("\tMISSING TOF 3 (Upper)");}
    else{
      turnable_upper.setDistanceMode(VL53L1X::Short);
      turnable_upper.setMeasurementTimingBudget(33000);
    }
    shift_register::write(SR_XSHT4, HIGH, true);
    shift_register::write(SR_XSHT3, LOW);
    delay(10);
    if (!turnable_lower.init()){Serial.print("\tMISSING TOF 4 (Lower)");}
    else{
      turnable_lower.setDistanceMode(VL53L1X::Short);
      turnable_lower.setMeasurementTimingBudget(33000);
    }
    shift_register::write(SR_XSHT4, LOW);

    Serial.println("...tof Sensors active!");
  }

  #define MACRO_TOF_READ(pin, obj)               \
    shift_register::write(pin, HIGH);            \
    uint16_t a = obj.readSingle();               \
    shift_register::write(pin, LOW, true);       \
    return a;

  uint16_t readLeft(){
    MACRO_TOF_READ(SR_XSHT1, left)
  }
  uint16_t readClaw(){
    MACRO_TOF_READ(SR_XSHT2, claw)
  }
  uint16_t readUpper(){
    MACRO_TOF_READ(SR_XSHT3, turnable_upper)
  }
  uint16_t readLower(){
    MACRO_TOF_READ(SR_XSHT4, turnable_lower)
  }

  void rotate(int dir){
    dir = min(max(dir, -90), 90);
    rottof.write(int(float(dir)/270*180) + 90);
  }
}



