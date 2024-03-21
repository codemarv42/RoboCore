#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "lightsensor.h"

#define check_length 7  // How often a color must be checked to allow a true call 
#define GREEN_THRESHOLD 15 // Minimum diff for a recognised green
#define BLACK_THRESHOLD 20// Max value for black
#define CHECKRED

namespace color{
  struct ColorCount{
    uint8_t left = 0;
    uint8_t right = 0;
  };

  extern void update(LightSensorArray* s_white, LightSensorArray* s_green, LightSensorArray* s_red);
  extern bool on_green(int side);
  extern bool on_black(int side);
  extern bool on_red(int side);
}


