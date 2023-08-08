#include "Pins.h"
#include <Arduino.h>
#include "hardware.h"
#include "Wire.h"
#include <time.h>
//#include "color.h"
/*
copyright (c) ImmernochKeinName, 2023

licensed under GPL v3. This applies for ALL files included in this Branch

view LICENSE.md for details
*/


// SPEED
#define V 100
#define DEBUG
//#define NOMOTORS


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
LightSensor red = LightSensor(SR_PT_RED);
LightSensor green = LightSensor(SR_PT_GREEN);
// PUT LIGHT SENSORS HERE
LightSensor* all_sensors[] = {&white,&green,&red,nullptr};

void setup(){

  Serial.begin(115200);
  Serial.println("HardwareInit...");
  HardwareInit();
  Serial.println("MPU-detection...");
  setupMPU();
  Serial.println("Calibration...");
  calibrate(all_sensors, 3000, 3);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
  delayMicroseconds(1000000);
}

int16_t diff_cache[10] = {0};
uint32_t timestamp = millis() + 100;
uint8_t cache_index = 0;

inline void clear_cache(){
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
  //color::update(&white, &green, &red);
  ////// LINE FOLLOWING //////
  #define diff_outer_factor 2 // Factor for the outer light 
  #define mul 1.5
  int16_t diff = white.left.value - white.right.value;
  int16_t diff_outer = white.left_outer.value - white.right_outer.value;
  //if (abs(diff_outer) < 25){diff_outer = 0;} // set diff to 0 when no difference is recognised
  int16_t mot_diff = (diff + diff_outer*diff_outer_factor) * mul; 
  cache(mot_diff);
  //if (color::on_black(RIGHT)){shift_register::write(SR_LED_L_RED, LOW);}
  #ifdef DEBUG
    Serial.print(white.left_outer.value);
    Serial.print(" ");
    Serial.print(white.left.value);
    Serial.print(" ");
    Serial.print(white.right.value);
    Serial.print(" ");
    Serial.print(white.right_outer.value);
    Serial.print(" ");
    Serial.println(mot_diff);
  #endif
  #ifndef NOMOTORS
    motor::fwd(A, ( V + mot_diff)); // TODO: change both sides to be equal, when hardware-problem is solved
    motor::fwd(B, ( V - mot_diff)*1.2);
    delayMicroseconds(100); // 
  #endif 
}
