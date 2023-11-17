#include <Arduino.h>
#include "drive_motor.h"
#include "robot.h"

Motor::Motor(byte pinPwm, byte pin1, byte pin2, byte pinStby) {
  this->pinPwm = pinPwm;
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pinStby = pinStby;
}

void Motor::init() {
  pinMode(pinPwm, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pinStby, OUTPUT);
  digitalWrite(pinStby, HIGH);
}

void Motor::Fwd(int v) {
  
  if (v > 255) {
    v = 255;
  }
  
  if (v < -255) {
    v = -255;
  }
  
  if (v >= 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  
  }
  if (v < 0) {
    v = -v;
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
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
  digitalWrite(pinStby, LOW);
}

void Motor::WakeUp() {
  digitalWrite(pinStby, HIGH);
}
