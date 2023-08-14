#ifndef MOTOR_CPP
#define MOTOR_CPP
#include <Arduino.h>
#include "motor.cpp"
#include "gyro.h"

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

namespace motor {
extern void stop();
extern void fwd(int motor, int v);
extern void rev(int motor, int v);
extern void gyro(int motor, int v, uint16_t deg);
}
//extern void setupMPU();
#endif