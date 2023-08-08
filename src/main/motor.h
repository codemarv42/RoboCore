#ifndef MOTOR_CPP
#define MOTOR_CPP
#include <Arduino.h>
#include "motor.cpp"

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

namespace motor {
extern void stop();
extern void fwd(int, int);
extern void rev(int, int);
}
extern void setupMPU();
#endif