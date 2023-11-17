#ifndef DRIVE_MOTOR_H
#define DRIVE_MOTOR_H
#include "Arduino.h"

class Motor {
  private:
    byte pinPwm;
    byte pin1;
    byte pin2;
    byte pinStby;
    int v;  // optional, muss nicht gespeichert werden

  public:
    Motor(byte pinPwm, byte pin1, byte pin2, byte pinStby);

    void init();
    void Fwd(int v);
    void Rev(int v);
    void Left(int v);
    void Right(int v);
    void Off();
    void StandBy();
    void WakeUp();
};


#endif