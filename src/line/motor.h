#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "gyro.h"
#include "lightsensor.h"
//#include "color.h"

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

namespace motor {
extern void stop();
extern void stop(int motor);
extern void hardstop(); // only when going fwd!!!
extern void fwd(int motor, int v);
extern void rev(int motor, int v);
extern void gyro(int v, int16_t deg);
//extern void readFwd(int motor, int v, int time, LightSensorArray* all[4]);
//extern bool sensorFwd(int va, int vb, int time, LightSensorArray* all[4]); 
//extern bool sensorFwd(int motor, int v, LightSensorArray* s, int diff, int time);
}
#endif