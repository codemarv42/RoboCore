#include "Pins.h"
#include <Arduino.h>
#include "hardware.h"
#include "Wire.h"

/*
copyright (c) ImmernochKeinName, 2023

licensed under GPL v3

view LICENSE.md for details
*/


// SPEED
#define V 100

bool HardwareInit(){
  /// get the shift register's Pins ///
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);

  shift_register::reset(); /// set all values to LOW
  Wire.begin();
  return true;
}

LightSensor white = LightSensor(SR_PT_WHITE);
// PUT LIGHT SENSORS HERE
LightSensor* all_sensors[4] = {&white,nullptr};
void setup(){
  Serial.begin(9600);
  Serial.println("HardwareInit...");
  HardwareInit();
  Serial.println("Calibration...");
  calibrate(all_sensors);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  delay(1000);
}



void loop() {

  for (auto sensor:all_sensors){ // read light values
    if (sensor != nullptr){sensor->read();}
  }
  int diff = white.left.value - white.right.value;
  motor::fwd(A, V + diff);
  motor::fwd(B, V - diff);
  
}
