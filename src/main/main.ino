#include "Pins.h"
#include <Arduino.h>
#include "hardware.h"
#include "Wire.h"
#include <time.h>

/*
copyright (c) ImmernochKeinName, 2023

licensed under GPL v3. This applies for ALL files included in this Branch

view LICENSE.md for details
*/


// SPEED
#define V 100
#define DEBUG
#define NOMOTORS

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
LightSensor* all_sensors[] = {&white,nullptr,nullptr,nullptr};

void setup(){

  Serial.begin(115200);
  Serial.println("HardwareInit...");
  HardwareInit();
  Serial.println("Calibration...");
  calibrate(all_sensors, 1000, 3);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
  delayMicroseconds(1000000);
}

int16_t diff_cache[10] = {0};
uint32_t timestamp = millis() + 100;
uint8_t cache_index = 0;

void clear_cache(){
  std::fill(std::begin(diff_cache), std::end(diff_cache), 0);
}
void cache(int16_t value){
  if (millis() > timestamp){
    timestamp = millis() + 100;
    diff_cache[cache_index] = value;
    cache_index = (cache_index + 1) % 10;
    #ifdef DEBUG
      Serial.println("Cached");
    #endif
  }
}

void loop() {
  for (auto sensor:all_sensors){ // read light values
    if (sensor != nullptr){sensor->read();}
  }
 //Serial.print(white.right.value);
  #define diff_outer_factor 2.5 // Factor for the outer light 
  #define center_factor 0.1
  #define mul 2
  int16_t diff = white.left.value - white.right.value;
  int16_t diff_outer = white.left_outer.value - white.right_outer.value;
  int16_t mot_diff = (diff + diff_outer*diff_outer_factor)*mul; //* center_factor * white.center.value;
  cache(mot_diff);
  /*Serial.print(white.left.value);
  Serial.print(" ");
  Serial.print(white.right.value);
  Serial.print(" ");
  Serial.println(diff);*/
  #ifndef NOMOTORS
    motor::fwd(A, V + mot_diff);
    motor::fwd(B, V - mot_diff);
  #endif
  // white.read();
  //Serial.println(diff);
  #ifndef NOMOTORS
    delayMicroseconds(1000);
  #endif 
}
