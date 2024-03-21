#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "gyro.h"
#include "lightsensor.h"
#include "color.h"

#define A 1 // Motors A = Left; B = Right
#define B 2
#define AB 3

namespace motor {
/*
namespace that handles all motor operations
*/

// stops both motors
extern void stop(); 
// stops the motor given
extern void stop(int motor);
// only when going fwd!!!
extern void hardstop(); 

// start a motor (or both) with velocity 'v'
extern void fwd(int motor, int v); 
// start a motor (or both) with velocity 'v' reversed
extern void rev(int motor, int v); 

// do a gyro turn by 'deg' degrees
extern void gyro(int v, int16_t deg);
extern void readFwd(int motor, int v, int time, LightSensorArray* all[4]);
extern bool sensorFwd(int va, int vb, int time, LightSensorArray* all[4]); 
extern bool sensorFwd(int motor, int v, LightSensorArray* s, int diff, int time);
}
#endif