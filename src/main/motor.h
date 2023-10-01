#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "gyro.h"
#include "lightsensor.h"

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

namespace motor {
extern void stop();
extern void fwd(int motor, int v);
extern void rev(int motor, int v);
extern void gyro(int v, int16_t deg);
extern bool sensorFwd(int motor, int v, LightSensor* s, LightSensor* s2, int diff, int time); 
extern bool sensorFwd(int motor, int v, LightSensor* s, int diff, int time); 
}
//extern void setupMPU();
#endif