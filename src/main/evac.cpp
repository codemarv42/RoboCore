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

#define VICTIM_DIFF 100

std::pair<uint8_t, bool> getOptimalVictimAngle(triangleData* t){
  for(static uint8_t i = 0; i < 180; i++){
    if (t->upper[i] - t->lower[i] >= VICTIM_DIFF && i > 15 && i < 165){
      return std::make_pair<uint8_t, bool>(i+1, false);
    }
  }
  
  return std::make_pair<uint8_t, bool>(0, true);
}

void evacuationZone(){
  uint8_t transporting = 0; // which victims are transported
  
  motor::fwd(AB, V); // go into the room
  delay(1500);
  motor::stop();

  while (true){
    // search victim

    rottof.write(90);
    int16_t diff_av = tof::readUpper() - tof::readLower();
    for(uint i = 0; i < 180; i += 15){
      rottof.write(i);
      uint16_t upper = tof::readUpper();
      uint16_t lower = tof::readLower();
      
      Serial.print(upper);
      Serial.print(" ");
      Serial.print(lower);
      Serial.print(" ");
      Serial.println(upper-lower);

      /*if (upper < 700 && diff_av + (upper - lower)){ // found -> init better search
        
        std::vector<uint16_t> distances = {};
        std::vector<uint> is = {};
        for (uint j = i; j < i+15; j++){
          rottof.write(j);
          uint16_t upper = tof::readUpper();
          uint16_t lower = tof::readLower();
          distances.push_back(upper - lower);
          is.push_back(j);
        }

        int correction = is[std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()))]; // get best angle.
        shift_register::write(SR_LED_L_RED, LOW);
        motor::gyro(V, -90 + correction); // turn at victim
        motor::stop();

        rottof.write(90); // reset servo for security
        claw::down();
        claw::open();
      }*/
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

void navRoom(LightSensorArray* white){
  motor::fwd(AB, V);
  delay(1500);
  motor::stop();
  motor::gyro(V, 90);
  motor::fwd(AB, V);

  uint16_t dist;
  do { // go fwd until outside the gap
    dist = tof::readLeft();
  } while (dist > 250);
  shift_register::write(SR_LED_R_BLUE, LOW);

  // follow the wall
  int32_t rdist;
  while (true){
    rdist = 110-tof::readLeft();
    motor::fwd(A, (V+20+rdist));
    motor::fwd(B, (V+20-rdist));
    white->read();
    if (white->left_outer.value < 35 || white->right_outer.value < 35){ //break on black detected
      break;
    }
    if (!(bool(digitalRead(T_L)) || bool(digitalRead(T_R)))){ // turn on wall
      motor::rev(AB, V);
      delay(500);
      motor::gyro(V, -90);
    }
    if(digitalRead(ENC_SW) == HIGH){ // abort with LOP switch
      motor::stop();
      shift_register::write(SR_LED_R_BLUE, HIGH);
      return;
    }
  }

  motor::stop();
  shift_register::write(SR_LED_R_BLUE, HIGH);
  motor::fwd(AB, V);
  delay(500);
  motor::stop();

}

