
#include "lightsensor.h"
#pragma once

// SPEED
#define VZ 150
#define V 100
#define V2 80
#define V3 40

// FLAGS
//#define BLE
//#define DEBUG
//#define NOMOTORS
//#define LED_TEST
#define LF_ACTIVE
#define LF_USE_BACK
//#define LF_USE_BLACK_INTRO






// Init Light Sesnsors
extern LightSensorArray white, red, green;
extern DirectSensor refL, refR;
extern LightSensorPair back;
extern LSBase* all_sensors[7];
extern LightSensorArray* ls[4];

extern int16_t diff_interchange;




