#include "Arduino.h"
#include "drive_motor.h"
#include "robot.h"
#include "shift_register.h"

Motor::Motor(byte pinPwm, byte pin1, byte pin2, byte pinStby, bool pol) {
  this->pinPwm = pinPwm;
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pinStby = pinStby;
  this->pol = pol;
}

void Motor::init() {
  pinMode(pinPwm, OUTPUT);
  ShiftRegisterWrite(pinStby, HIGH);
}

void Motor::Fwd(int v) {
  if (v > 255) {
    v = 255;
  }
  
  if (v < -255) {
    v = -255;
  }
  
  if ((v >= 0)) {
    ShiftRegisterWrite(pin1, LOW,false);
    ShiftRegisterWrite(pin2, HIGH);
  }
  else{
    v = -v;
    ShiftRegisterWrite(pin1, HIGH,false);
    ShiftRegisterWrite(pin2, LOW);
  }
  
  analogWrite(pinPwm, v);
}

void Motor::Rev(int v) {
  Fwd(-v);
}

void Motor::Left(int v){
  Fwd(-v);
}

void Motor::Right(int v){
  Fwd(v);
}

void Motor::Off() {
  Fwd(0);
}

void Motor::StandBy() {
  ShiftRegisterWrite(pinStby, LOW);
}

void Motor::WakeUp() {
  ShiftRegisterWrite(pinStby, HIGH);
}
