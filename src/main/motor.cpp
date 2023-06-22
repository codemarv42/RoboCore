#ifndef MOTOR_CPP
#define MOTOR_CPP

#include <Arduino.h>
#include "Pins.h"
#include "shiftregister.h"

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

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
  }
  void fwd(int motor,int v){
    if (v > 255){v = 255;}
    else if (v < -255){v = -255;}
    if (v == 0){stop(); return;}
    shift_register::write(SR_STBY, HIGH);
    if (v >= 0) {
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
}

#endif
