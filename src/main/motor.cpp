#include "esp32-hal.h"
#include "HardwareSerial.h"
#ifndef MOTOR_CPP
#define MOTOR_CPP

#include <Arduino.h>
#include "Pins.h"
#include "gyro.h"
#include "motor.h"
#include "color.h"
#include "shared.h"
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
    shift_register::write(SR_AIN1, LOW, true);
    shift_register::write(SR_AIN2, LOW, true);
    shift_register::write(SR_BIN1, LOW, true);
    shift_register::write(SR_BIN2, LOW);
  }
  void stop(int motor){
    if (motor & B){
      digitalWrite(PWMB, LOW);
      shift_register::write(SR_BIN1, LOW, true);
      shift_register::write(SR_BIN2, LOW);
    }
    if (motor & A){
      digitalWrite(PWMA, LOW);
      shift_register::write(SR_AIN1, LOW, true);
      shift_register::write(SR_AIN2, LOW);
    }
  }
  void hardstop(){ // works only when going fwd!!!
    rev(AB, 40);
    delay(25);
    stop();
  }

  void fwd(int motor,int v){
    if (v > 255){v = 255;}
    else if (v < -255){v = -255;}
    if (v == 0){stop(); return;}
    shift_register::write(SR_STBY, HIGH, true);
    if (v > 0) {
      if (motor & A) {
        shift_register::write(SR_AIN1, LOW, true);
        shift_register::write(SR_AIN2, HIGH);
        analogWrite(PWMA, v);
      }
      if (motor & B) {
        shift_register::write(SR_BIN1, LOW, true);
        shift_register::write(SR_BIN2, HIGH);
        analogWrite(PWMB, v);
      }
    }
    else{
      v = -v;
      if (motor & A) {
        shift_register::write(SR_AIN1, HIGH, true);
        shift_register::write(SR_AIN2, LOW);
        analogWrite(PWMA, v);
      }
      if (motor & B) {
        shift_register::write(SR_BIN1, HIGH, true);
        shift_register::write(SR_BIN2, LOW);
        analogWrite(PWMB, v);
      }
    }
  }
  void rev(int motor, int v){
    fwd(motor, -v);
  }
  void gyro(int v, int16_t deg){ //negative angle -> right
    motor::stop();
    gyro::ResetZAngle();
    Serial.println("motor::gyro");
    gyro::UpdateMPU6050();
    delay(10);
    if (deg < 0){
      fwd(A, v);
      fwd(B, -v);
      
      while (gyro::ZAngle > deg){
        gyro::UpdateMPU6050();
      }
    }
    else{
      fwd(A, -v);
      fwd(B, v);
    
      while (gyro::ZAngle < deg){
        gyro::UpdateMPU6050();
      }
    }
    stop();
  }
  void readFwd(int motor, int v, int time, LightSensorArray* all[4]){
    const int timestamp = millis() + time;
    while (true){
      for (int i = 0; i < 4; i++){
        if (all[i] != nullptr){all[i]->read();}
      }
      color::update(all[0], all[1], all[2]);
      if (millis() > timestamp){motor::stop(); return;}
    }
  }

  bool sensorFwd(int va, int vb, int time, LightSensorArray* all[4]){
    fwd(A, va);
    fwd(B, vb);
    const int timestamp = millis() + time;
    while (millis() < timestamp){
      read();
      Serial.println(white.left_outer.value);
      color::update(&white, &green, &red);
      if (color::on_black(LEFT | RIGHT)){motor::stop(); return true;}

    }
    stop();
    return false;
  }

}

#endif
