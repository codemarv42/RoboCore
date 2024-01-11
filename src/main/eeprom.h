#pragma once
#include "Pins.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "lightsensor.h"

// see eeprom_layout.md for details on the layout


namespace eeprom{
  //extern void writeInRescueZone(bool); // NYI
  //extern bool loadInRescueZone(); // NYI

  extern void writeLSData(LightSensor* white, LightSensor* green, LightSensor* red, LightSensor* blue);
  extern void loadLSData(LightSensor* white, LightSensor* green, LightSensor* red, LightSensor* blue);
}




