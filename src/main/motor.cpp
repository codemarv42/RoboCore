#ifndef MOTOR_CPP
#define MOTOR_CPP

#include <Arduino.h>
#include "Pins.h"
#include "gyro.h"
#include "motor.h"
#include "shiftregister.h"
#include "lightsensor.h"
#include <MPU6050_light.h>
#include <Wire.h>

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3


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
  void gyro(int v, int16_t deg){
    //mpu.
    gyro::ResetZAngle();

    //deg += mpu.getAngleZ(); // 'reset' the Z axis
    if (deg < 0){
      fwd(A, v);
      fwd(B, -v);
    }
    else{
      fwd(A, -v);
      fwd(B, v);
    }
    while (mpu.getAngleZ() < deg){
      mpu.update();
      delay(1);
    }
  }
  bool sensorFwd(int motor, int v, LightSensor* s, LightSensor* s2, int diff, int time){
    fwd(motor, v);
    const int timestamp = millis() + time;
    while (s->left.value-s2->left.value >= diff && s->right.value-s2->right.value >= diff){
      s->read();
      s2->read();
      if (millis() > timestamp){motor::stop(); return false;}

    }
    stop();
    return true;
  }
  bool sensorFwd(int motor, int v, LightSensor* s, int diff, int time){
    fwd(motor, v);
    const int timestamp = millis() + time;
    while (s->left.value >= diff || s->right.value >= diff){
      s->read();
      if (millis() > timestamp){motor::stop(); return false;}

    }
    stop();
    return true;
  }
}

#endif
