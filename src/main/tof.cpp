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
    shift_register::write(SR_XSHT1, LOW, true);
    shift_register::write(SR_XSHT2, HIGH);
    delay(10);
    if (!claw.init()){Serial.print("\tMISSING TOF 2 (Claw)");}
    shift_register::write(SR_XSHT3, HIGH, true);
    shift_register::write(SR_XSHT2, LOW);
    delay(10);
    if (!turnable_upper.init()){Serial.print("\tMISSING TOF 3 (Upper)");}
    shift_register::write(SR_XSHT4, HIGH, true);
    shift_register::write(SR_XSHT3, LOW);
    delay(10);
    if (!turnable_lower.init()){Serial.print("\tMISSING TOF 4 (Lower)");}
    shift_register::write(SR_XSHT4, LOW);

    Serial.println("...tof Sensors active!");
  }

  uint16_t readLeft(){
    shift_register::write(SR_XSHT1, HIGH);
    delay(1);
    uint16_t a = left.readSingle();
    shift_register::write(SR_XSHT1, LOW, true);
    return a;
  }
  uint16_t readUpper(){
    shift_register::write(SR_XSHT3, HIGH);
    //delay(1);
    uint16_t a = turnable_upper.readSingle();
    shift_register::write(SR_XSHT3, LOW, true);
    return a;
  }
  uint16_t readLower(){
    shift_register::write(SR_XSHT4, HIGH);
    //delay(1);
    uint16_t a = turnable_lower.readSingle();
    shift_register::write(SR_XSHT4, LOW, true);
    return a;
  }

  triangleData* readPos(){ // DONT FORGET TO DELETE THE TRIANGLEDATA!!!
    triangleData* t = new triangleData;
    rottof.write(0);
    for(uint8_t i = 0; i < 180; i++){
      rottof.write(i);
      t->upper[i] = readUpper();
      t->lower[i] = readLower();
    }

    return t;
  }
}



