#include "color.h"
#include <iterator>
#include <algorithm>
#include "esp32-hal-gpio.h"
#include <sys/_stdint.h>
#include "tof.h"

#include <Arduino.h>
#include "Pins.h"
#include "hardware.h"
#include "evac.h"
#include "menu.h"
#include <utility>
#include <vector>

#define VICTIM1 1 // dead victim
#define VICTIM2 2
#define VICTIM3 4
#define V 100
#define V2 80

#define VICTIM_DIFF 100

TaskHandle_t loop0;

std::pair<uint8_t, bool> getOptimalVictimAngle(triangleData* t){
  for(static uint8_t i = 0; i < 180; i++){
    if (t->upper[i] - t->lower[i] >= VICTIM_DIFF && i > 15 && i < 165){
      return std::make_pair<uint8_t, bool>(i+1, false);
    }
  }
  
  return std::make_pair<uint8_t, bool>(0, true);
}

void core0evac(void * pvParameters);
void evacuationZone(){
  uint8_t transporting = 0; // which victims are transported
  
  motor::fwd(AB, V); // go into the room
  delay(1500);
  motor::stop();

  while (true){
    // search victim

    rottof.write(90);
    //int16_t diff_av = tof::readUpper() - tof::readLower();
    for(uint i = 15; i < 180; i += 15){
      rottof.write(i);
      uint16_t upper = tof::readUpper();
      uint16_t lower = tof::readLower();
      
      Serial.print(upper);
      Serial.print(" ");
      Serial.print(lower);
      Serial.print(" ");
      Serial.println(upper-lower);

      if (upper < 1200 && upper-lower >= 110 && i != 30 && i != 165){ // found -> init better search
        
        std::vector<uint16_t> distances = {};
        std::vector<uint> is = {};
        for (uint j = i-7; j < i+6; j++){
          rottof.write(j);
          uint16_t upper = tof::readUpper();
          uint16_t lower = tof::readLower();
          distances.push_back(upper-lower);
          is.push_back(j);
        }

        int correction = is[std::distance(distances.begin(), std::max_element(distances.begin(), distances.end()))]; // get best angle.
        shift_register::write(SR_LED_L_RED, LOW);
        motor::gyro(V2, -90 + correction); // turn at victim
        motor::stop();

        rottof.write(90); // reset servo for security
        claw::down();
        claw::open();
        uint16_t clawdist = tof::readClaw();
        while (clawdist > 69) {
          clawdist = tof::readClaw();
          motor::fwd(AB, min(clawdist/2, V));
        }
      }
    }
  }
}
/*
while(true){
  for(uint i = 0; i < 180; i += 15){
    rottof.write(i);
    uint16_t upper = tof::readUpper();
    uint16_t lower = tof::readLower();
    Serial.print(upper);
    Serial.print(" ");
    Serial.print(lower);
    Serial.print(" ");
    Serial.println(upper-lower);
    if(upper - lower <= 100 && upper - lower > 10){
      motor::gyro(V2,90-i);
      Serial.println("found Victim!");
      shift_register::write(SR_LED_L_BLUE, LOW);
      claw::open();
      claw::down();
      int claw;
      do {
        claw = tof::readClaw();
        Serial.println(claw);
        motor::fwd(AB,min(70,claw));
      } while(claw > 69);
      motor::fwd(AB, 70);
      delay(100);
      motor::stop();
      motor::stop();
      claw::close();
      claw::up();
    }
    else{
      shift_register::write(SR_LED_L_BLUE, HIGH);
    }
  }
}
*/


void navRoom(LightSensorArray* a[4]){
  motor::fwd(AB, V);
  delay(1500);
  motor::stop();
  motor::gyro(V, 90);
  
  motor::fwd(AB, V2);
  uint16_t dist;
  do { // go fwd until outside the gap
    dist = tof::readLeft();
  } while (dist > 250);
  shift_register::write(SR_LED_R_BLUE, LOW);

  while(true){
    a[0]->read();
    color::update(a[0], a[1], a[2])
    motor::fwd(AB, V2);
    if (!(bool(digitalRead(T_L)) || bool(digitalRead(T_R)))){
      delay(500);
      motor::rev(AB, 70);
      delay(500);
      motor::gyro(V, -90);
    }
    if (tof::readLeft() > 250){
      motor::fwd(AB, V);
      delay(700);
      motor::gyro(V, 90);
      if (motor::sensorFwd(V2, V2, 1500, a)){
        shift_register::reset();
        break;
      }
      else{
        motor::rev(AB, V);
        delay(400);
        motor::gyro(V, -90);
      }
    }
    if (color::on_black(LEFT | RIGHT)){
      shift_register::reset();
      break;
    }
  }
}


/*
int32_t rdist, upper;
void navRoom(LightSensorArray* white){
  motor::fwd(AB, V);
  delay(1100);
  motor::stop();
  motor::gyro(V, 90);
  motor::fwd(AB, V);

  uint16_t dist;
  do { // go fwd until outside the gap
    dist = tof::readLeft();
  } while (dist > 250);
  shift_register::write(SR_LED_R_BLUE, LOW);

  // follow the wall
  rottof.write(180);
  LightSensorArray* a[4] = {white, nullptr, nullptr, nullptr};
  vTaskDelete(loop0);
  xTaskCreatePinnedToCore(core0evac, "Core0Evac", 10000, NULL, 0, &loop0, 0);
  while (true){
    motor::fwd(A, (V+(rdist-upper)));
    motor::fwd(B, (V-(rdist-upper)));
    white->read();
    if (white->left_outer.value < 30 || white->right_outer.value < 30){ //break on black detected
      break;
    }
    if (!(bool(digitalRead(T_L)) || bool(digitalRead(T_R)))){ // turn on wall
      vTaskDelete(loop0);
      delay(500);
      shift_register::write(SR_LED_R_GREEN, LOW);
      motor::rev(AB, V);
      delay(400);
      mpu.begin();
      motor::gyro(V, -90);
      xTaskCreatePinnedToCore(core0evac, "Core0Evac", 10000, NULL, 0, &loop0, 0);
      }
    if (upper >= 400){
      motor::fwd(AB, V);
      delay(1000);
      motor::gyro(V, 90);
      if (motor::sensorFwd(V2, V2, 4000, a)){
        shift_register::reset();
        break;
      }
      else{
        motor::rev(AB, V);
        delay(2000);
        motor::gyro(V, -90);
      }
    }
  }

  motor::stop();
  shift_register::write(SR_LED_R_BLUE, HIGH);
  motor::fwd(AB, V);
  delay(500);
  motor::stop();

  
}

void core0evac( void * pvParameters){
  while(true){
    rdist = tof::readLeft();
    upper = tof::readUpper();
  }
}
*/
