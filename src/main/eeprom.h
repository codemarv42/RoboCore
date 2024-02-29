#pragma once
#include "Pins.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "lightsensor.h"

// see eeprom_layout.md for details on the layout


namespace eeprom{
  //extern void writeInRescueZone(bool); // NYI
  //extern bool loadInRescueZone(); // NYI
  extern void setup();

  extern void writeLSData(LightSensorArray* white, LightSensorArray* green, LightSensorArray* red, LightSensorArray* blue, LightSensorPair* back);
  extern void loadLSData(LightSensorArray* white, LightSensorArray* green, LightSensorArray* red, LightSensorArray* blue, LightSensorPair* back);
}




