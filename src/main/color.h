#ifndef COLOR_H
#define COLOR_H
#include <Arduino.h>
#include "Pins.h"
//#include "color.cpp"
#include "lightsensor.h"

namespace color{
  extern void update(LightSensorArray* s_white, LightSensorArray* s_green, LightSensorArray* s_red);
  extern bool on_green(int side);
  extern bool on_black(int side);
  extern bool on_red(int side);
}

#endif