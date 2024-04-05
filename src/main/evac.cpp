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
#include <cmath>
#include <cstdlib>


#define VICTIM1 1 // dead victim
#define VICTIM2 2
#define VICTIM3 4
#define V 100
#define V2 80

#define VICTIM_DIFF 100

TaskHandle_t loop0;

void evacuationZone(){
  uint8_t transporting = 0; // which victims are transported
  
  //motor::fwd(AB, V); // go into the room
  //delay(1500);
  //motor::stop();

  while (true){
    // search victim

    tof::rotate(0);
    for(int i = 45; i < 135; i++){
      tof::rotate(i - 90);
      int16_t upper = tof::readUpper();
      int16_t lower = tof::readLower();
      
      Serial.print(upper);
      Serial.print(" ");
      Serial.print(lower);
      Serial.print(" ");
      Serial.println(upper-lower);

      if (upper < 1200 && upper-lower  > 99 /*&& upper-lower < 250*/){ // found -> init better search
        
        std::vector<int16_t> distances = {};
        std::vector<int> is = {};
        
        for (int j = i-7; j < i+6; j++){
          tof::rotate(j - 90);
          uint16_t upper_r = tof::readUpper();
          uint16_t lower_r = tof::readLower();
          distances.push_back(upper_r-lower_r);
          is.push_back(j);
        }
        
        Serial.print("Distances Evaluated: "); Serial.println(distances.size());
        int correction = is[std::distance(distances.begin(), std::max_element(distances.begin(), distances.end()))]; // get best angle.
        
        Serial.print("Correction: "); Serial.println((correction - 90) * std::pow(2, abs(correction) / 90) * 0.5);
        shift_register::write(SR_LED_L_RED, LOW);
        if (abs(correction -90) > 5){
          motor::gyro(V2, (correction - 90) * std::pow(2, abs(correction) / 90) * 0.5); // turn at victim
        }
        delay(1000);
        motor::stop();

        tof::rotate(0); // reset servo for security
        claw::openwide();
        claw::down();
        claw::open();
        
        uint16_t clawdist = tof::readClaw();
        auto timestamp = millis() + 4000;
        int16_t clawDist;
        bool isLiving = false;
        bool succes = false;
        while (clawdist > 69) {
          clawdist = tof::readClaw();
          Serial.println(clawdist);
          motor::fwd(AB, max(min(clawdist/2, V), 60));
          if (millis() > timestamp){
            motor::stop();
            goto after_pickup; // expanded later
          }
        }

        motor::fwd(AB, 60);
        delay(700);
        motor::stop();
        claw::close();
        motor::rev(AB, 60);
        delay(500);
        motor::stop();

        // check if victim has been rescued
        claw::half();
        Serial.print("Dist: ");
        clawDist = tof::readClaw();
        Serial.println(clawDist);

        // check if metal sensor is being activated
        if (!digitalRead(M_S)){
          Serial.println("Succes (MS)!");
          shift_register::write(SR_LED_L_BLUE, LOW);
          isLiving = true;
          succes = true;
        }
        else if (clawDist <= 69){
          Serial.println("Succes (Distance)!");
          shift_register::write(SR_LED_L_GREEN, LOW);
          succes = true;
        }

        if(succes){
          claw::divide(int(isLiving) + 1);
        }

        after_pickup:
        claw::up();
        claw::open();

        shift_register::write(SR_LED_L_RED, HIGH, true);
        shift_register::write(SR_LED_L_GREEN, HIGH);
        }
    }
  }
}

void navRoom(LightSensorArray* a[4]){
  shift_register::reset();
  motor::fwd(AB, V);
  delay(1500);
  motor::stop();
  motor::gyro(V, 110);
  
  motor::fwd(AB, V2);
  uint16_t dist;
  do { // go fwd until outside the gap
    dist = tof::readLeft();
  } while (dist > 250);
  shift_register::write(SR_LED_R_BLUE, LOW);

  while(true){
    a[0]->read();
    //color::update(a[0], a[1], a[2]);
    motor::fwd(AB, V2);
    if (!(bool(digitalRead(T_L))))/* || bool(digitalRead(T_R))))*/{
      //motor::stop();
      delay(500);
      motor::stop();
      motor::fwd(B, 200);
      delay(5000);
      motor::stop();
      motor::rev(AB, 70);
      delay(500);
      motor::gyro(V, -90);
    }
    if (tof::readLeft() > 500){
      motor::fwd(AB, V);
      delay(700);
      motor::gyro(V, 90);
      motor::sensorFwd(V2, V2, 2000, a);
      break;

      
      /*if (motor::sensorFwd(V2, V2, 3000, a)){
        shift_register::reset();
        break;*/
      }
      /*else{
        motor::rev(AB, V);
        delay(400);
        motor::gyro(V, -90);
      }*/
    //}
    if (a[0]->left_outer.value < 35 || a[0]->right_outer.value < 35){
      motor::fwd(AB, V);
      delay(400);
      motor::stop();
      while(!(a[0]->left_outer.value < 35 || a[0]->right_outer.value < 35)){
        motor::fwd(A, V2);
        motor::fwd(B, 40);
        //motor::rev(B, V2);
        a[0]->read();
      }
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
