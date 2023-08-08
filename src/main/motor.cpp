#ifndef MOTOR_CPP
#define MOTOR_CPP

#include <Arduino.h>
#include "Pins.h"
#include "shiftregister.h"
#include <MPU6050_light.h>
#include <Wire.h>

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

MPU6050 mpu(Wire);

bool setupMPU(){
  byte status = mpu.begin();
  if(status != 0){
    Serial.println(F("Fail to detect MPU6050!"));
    return false;
  }else{
    Serial.println(F("Found MPU6050"));
    Serial.println(F("While calculating the offsets value, do not move the MPU6050 sensor!"));
    delay(500);
    mpu.calcOffsets();
    Serial.println("Done!\n");
    return true;
  }
}

inline int limit(int value, int max){
  if (abs(value) > max){return max - 2*max*int(value < 0);}
  return value;
}

inline float limit(float value, float max){
  if (abs(value) > max){return max - 2*max*int(value < 0);}
  return value;
}

inline float limit(float value, int max){
  if (abs(value) > max){return float(max - 2*max*int(value < 0));}
  return value;
}

namespace motor{
  void stop(){
    shift_register::write(SR_STBY, LOW);
    digitalWrite(PWMA, LOW);
    digitalWrite(PWMB, LOW);
    shift_register::write(SR_AIN1, LOW);
    shift_register::write(SR_AIN2, LOW);
    shift_register::write(SR_BIN1, LOW);
    shift_register::write(SR_BIN2, LOW);
  }
  void fwd(int motor,int v){
    if (v > 255){v = 255;}
    else if (v < -255){v = -255;}
    if (v == 0){stop(); return;}
    shift_register::write(SR_STBY, HIGH);
    if (v > 0) {
      if (motor & A) {
        shift_register::write(SR_AIN1, LOW);
        shift_register::write(SR_AIN2, HIGH);
        analogWrite(PWMA, v);
      }
      if (motor & B) {
        shift_register::write(SR_BIN1, LOW);
        shift_register::write(SR_BIN2, HIGH);
        analogWrite(PWMB, v);
      }
    }
    else{
      v = -v;
      if (motor & A) {
        shift_register::write(SR_AIN1, HIGH);
        shift_register::write(SR_AIN2, LOW);
        analogWrite(PWMA, v);
      }
      if (motor & B) {
        shift_register::write(SR_BIN1, HIGH);
        shift_register::write(SR_BIN2, LOW);
        analogWrite(PWMB, v);
      }
    }
  }
  void rev(int motor, int v){
    fwd(motor, -v);
  }
  void gyro(int motor, int v, uint16_t deg){
    mpu.calcGyroOffsets();
    fwd(A, v*(deg/abs(deg)));
    fwd(B, -v*(deg/abs(deg)));
    while (mpu.getAngleZ() < deg){
      mpu.update();
    }
  }
}

#endif
