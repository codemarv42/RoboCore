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
#define V 200
//#define DEBUG
//#define NOMOTORS
//#define LED_TEST
#define LF_ACTIVE

bool HardwareInit(){
  /// get the shift register's Pins ///
  claw::setup();
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(T_L, INPUT_PULLUP);
  pinMode(T_R, INPUT);
  pinMode(T_M, INPUT);

  shift_register::reset(); /// set all values to LOW
  Wire.begin();
  //StartBLE();
  return true;
}
LightSensor white = LightSensor(SR_PT_WHITE);
LightSensor red   = LightSensor(SR_PT_RED);
LightSensor green = LightSensor(SR_PT_GREEN);
// PUT LIGHT SENSORS HERE
LightSensor* all_sensors[] = {&white,&green,&red,nullptr};  // nullptr is placeholder for an (optional) blue LightSensor

void setup(){
  Serial.begin(115200);
  Serial.println("HardwareInit...");
  HardwareInit();
  DisplayInit();
  Serial.println("MPU-detection...");
  gyro::MPU6050Init();
  Serial.println("Resetting Claw...");
  claw::up(); // reset the claw
  claw::close();
  #ifdef LED_TEST
    shift_register::write(SR_PT_WHITE, HIGH);
    delay(2000);
    shift_register::write(SR_PT_WHITE, LOW);
    shift_register::write(SR_PT_GREEN, HIGH);
    delay(2000);
    shift_register::write(SR_PT_GREEN, LOW);
    shift_register::write(SR_PT_RED, HIGH);
    delay(2000);
    shift_register::write(SR_PT_RED, LOW);
  #endif
  Serial.println("Calibration...");
  calibrate(all_sensors, 3000, 3);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
  delayMicroseconds(1000000);
  //motor::gyro(AB, V/2, 90);
  motor::stop();
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
    //showDifference(value, "D", true);
  }
}

void loop() {
  for (auto sensor:all_sensors){ // read light values
    if (sensor != nullptr){sensor->read();}
  }
  //BLELoop(white.left.value);
  color::update(&white, &green, &red);  // update color checking
  if (color::on_red(RIGHT | LEFT)){
    motor::stop();
    shift_register::write(SR_LED_R_RED, LOW);
    shift_register::write(SR_LED_L_RED, LOW);
    delay(7000);
    shift_register::write(SR_LED_R_RED, HIGH);
    shift_register::write(SR_LED_L_RED, HIGH);
  }

  #ifndef NOMOTORS
    if (color::on_green(RIGHT | LEFT)){
      #ifdef DEBUG
        Serial.println("Green Detected!");
      #endif
      motor::fwd(AB, V); // go fwd a litle bit, to confirm green values
      delay(200);
      motor::stop();
      bool left = color::on_green(LEFT);
      bool right = color::on_green(RIGHT);
      shift_register::write(SR_LED_R_GREEN, !right); // show side on LED
      shift_register::write(SR_LED_L_GREEN, !left);
      motor::sensorFwd(AB, V/2, 1500, all_sensors); // go fwd, until there is no green
      if(color::on_black(LEFT | RIGHT)){ // check for black line
        delay(1000);
        int16_t turn = 0; // choose turn side W.I.P.
        if(left){ turn += 90;};
        if(right){ turn += 90;};
        if (right && (!left)){turn = -turn;}
        motor::gyro(V, turn);
        motor::fwd(AB, V);
        delay(1000);
        motor::stop();
      }
      shift_register::write(SR_LED_R_GREEN, HIGH); // LEDs off
      shift_register::write(SR_LED_L_GREEN, HIGH);
    }
  #endif
  ////// LINE FOLLOWING //////
  #ifdef LF_ACTIVE
    #define diff_outer_factor 2 // Factor for the outer light 
    #define mul 3
    int16_t diff = white.left.value - white.right.value;
    int16_t diff_outer = white.left_outer.value - white.right_outer.value;
    if (abs(diff_outer) < 25){diff_outer = 0;} // set diff to 0 when no difference is recognised
    int16_t mot_diff = (diff*1.5 + diff_outer*diff_outer_factor) * mul;  // calculate inner to outer mult 
    cache(mot_diff); // cache W.I.P.
    
    #ifdef DEBUG  // Debug light values
      Serial.print("White: ");
      Serial.print(white.left_outer.value);
      Serial.print(" ");
      Serial.print(white.left.value);
      Serial.print(" ");
      Serial.print(white.right.value);
      Serial.print(" ");
      Serial.print(white.right_outer.value);
      Serial.print(" ");
      Serial.print("Green: ");
      Serial.print(green.left_outer.value);
      Serial.print(" ");
      Serial.print(green.left.value);
      Serial.print(" ");
      Serial.print(green.right.value);
      Serial.print(" ");
      Serial.print(green.right_outer.value);
      Serial.print(" ");
      Serial.print("Red: ");
      Serial.print(red.left_outer.value);
      Serial.print(" ");
      Serial.print(red.left.value);
      Serial.print(" ");
      Serial.print(red.right.value);
      Serial.print(" ");
      Serial.print(red.right_outer.value);
      Serial.print(" ");
      Serial.println(mot_diff);
      //delay(200);
    #endif
    #ifndef NOMOTORS
      motor::fwd(A, ( V + mot_diff)); // TODO: change both sides to be equal, when hardware-problem is solved
      motor::fwd(B, ( V - mot_diff)*1.2);
      delayMicroseconds(100); // about 3000 measurements per second
    #endif
  #endif
  
  ////// RESCUE-KIT //////
  if(digitalRead(T_M) == LOW){ // rescue kit detected
    motor::rev(AB, V);
    delay(1000);
    motor::stop();
    claw::open();
    claw::down();
    motor::fwd(AB, V);
    delay(700);
    motor::stop();
    claw::close();
    claw::up();
  }

  //shift_register::write(SR_LED_L_RED,bool(digitalRead(T_L)));
  //shift_register::write(SR_LED_R_RED,bool(digitalRead(T_R)));
  ////// OBSTACLE HANDLING //////
  if (!(bool(digitalRead(T_L)) || bool(digitalRead(T_R)))){
    motor::rev(AB, V);
    delay(500);
    motor::stop();
    delay(500);
    motor::gyro(V, 90);
    // TODO MAKE THIS READY
  }
}
