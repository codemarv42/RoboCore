#include <type_traits>
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
#include "shared.h"
#include <utility>
#include <vector>
#include <cmath>
#include <cstdlib>


#define VICTIM1 1 // dead victim
#define VICTIM2 2
#define VICTIM3 4
#define V 150
#define V2 80

#define VICTIM_DIFF 100
#define SKIP_SEARCH

TaskHandle_t loop0;

void evacuationZone(){
  uint8_t transporting = 0; // which victims are transported
  std::srand(millis());

  //motor::fwd(AB, V); // go into the room
  //delay(1500);
  //motor::stop();

  #ifndef SKIP_SEARCH
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

        if (upper < 1200 && upper-lower  > 60 /*&& upper-lower < 250*/){ // found -> init better search
          if (!(upper - lower > 60)){
            continue;
          }

          std::vector<int16_t> distances = {};
          std::vector<int> is = {};
          
          for (int j = i-9; j < i+8; j++){
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
            motor::gyro(V2, (correction - 90) * std::pow(2, abs(correction) / 90) * 0.3); // turn at victim
          }

          tof::rotate(0); // reset servo for security
          claw::openwide();
          motor::rev(AB, V2);
          delay(1000);
          motor::stop();
          claw::down();
          claw::open();
          motor::fwd(AB, V2);
          delay(1000);
          motor::stop();
          
          
          uint16_t clawdist = tof::readClaw();
          auto timestamp = millis() + 30 * (*std::max_element(distances.begin(), distances.end()).base());
          int16_t clawDist;
          bool isLiving = false;
          bool succes = false;
          shift_register::write(SR_LED_R_GREEN, LOW);
          while (clawdist > 69) {
            clawdist = tof::readClaw();
            Serial.println(clawdist);
            motor::fwd(AB, max(min(clawdist/2, V), 60));
            if (millis() > timestamp){
              motor::stop();
              goto after_pickup; // expanded later
            }
          }
          shift_register::write(SR_LED_R_GREEN, HIGH);

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
          if (!(ADCRead(M_S) + ADCRead(M_S) + ADCRead(M_S))){ // Read 3 times to confirm => 0 means living victim
            Serial.println("Succes (MS)!");
            shift_register::write(SR_LED_L_BLUE, LOW);
            isLiving = true;
            succes = true;
            if (transporting & VICTIM2){ // update transporting
              transporting |= VICTIM3;
            }
            else{
              transporting |= VICTIM2; 
            }
          }
          else if (clawDist <= 69){
            Serial.println("Succes (Distance)!");
            shift_register::write(SR_LED_L_GREEN, LOW);
            succes = true;
            transporting |= VICTIM1; // update transporting
          }

          if(succes){ // divide the victims living / dead
            claw::divide(int(isLiving) + 1);
          }

          after_pickup:
          claw::up();
          claw::open();

          shift_register::write(SR_LED_L_RED, HIGH, true);
          shift_register::write(SR_LED_L_GREEN, HIGH);
          break; // break for loop and restart searching
          }
      }
      if (transporting == VICTIM1 + VICTIM2 + VICTIM3){ // => all victims rescued
        break;
      }
      motor::gyro(V, 45); // turn a random amount
      motor::fwd(AB, V);
      delay(std::rand() % 20 * 100); 
      motor::stop();
      tof::rotate(0);
      while (tof::readUpper() < 200){ // => In front of a Wall
        motor::rev(AB, V);
        delay(500);
        motor::stop();
        motor::gyro(V, 90);
      }
    }
  #endif // SKIP_SEARCH

  // find nearest wall
  std::vector<int16_t> distances = {};
  std::vector<int> is = {};
  
  for (int j = 45; j < 135; j += 15){
    tof::rotate(j - 90);
    uint16_t upper_r = tof::readUpper();
    distances.push_back(upper_r);
    is.push_back(j);
  }
  
  /*
  Serial.print("Distances Evaluated: "); Serial.println(distances.size());
  int correction = is[std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()))]; // get best angle.
  
  Serial.print("Correction: "); Serial.println((correction - 90) * std::pow(2, abs(correction) / 90) * 0.5);
  if (abs(correction -90) > 5){
    motor::gyro(V2, (correction - 90));
    //motor::gyro(V2, (correction - 90) * std::pow(2, abs(correction) / 90) * 0.5); // turn at victim
  }
  */
  // go fwd until wall
  motor::fwd(AB, V);
  while (digitalRead(T_L) && digitalRead(T_R)){
    // do nothing
  }

  motor::stop(A);
  delay(2000);
  motor::stop(B);
  motor::fwd(A, V);
  delay(2000);
  motor::stop();

  // align and start navigating
  motor::rev(AB, V);
  delay(700);
  motor::gyro(V, -90);

  navRoom(true);


}

void navRoom(bool skipGap){
  shift_register::reset();
  int unloaded = 0;
  if (!skipGap){
    Serial.println("Halllo");
    motor::fwd(AB, V);
    delay(1500);
    motor::stop();
    motor::gyro(V, 110);
  
    motor::fwd(AB, V2);
    uint16_t dist;
    do { // go fwd until outside the gap
      dist = tof::readLeft();
    } while (dist > 250 && (digitalRead(T_L) && digitalRead(T_R)));
    shift_register::write(SR_LED_R_BLUE, LOW);
    unloaded = LEFT | RIGHT;
  }

  while(true){
    white.read();
    motor::fwd(AB, V2);
    if ((!digitalRead(T_L) || !digitalRead(T_R))){ // Corner/Wall
      Serial.println("Hewlojfiejdhefoie");
      // align with wall
      while (digitalRead(T_R)){
        motor::rev(AB, V2);
        delay(750);
        motor::rev(A, V2);
        motor::fwd(B, V);
        delay(100);
        motor::fwd(AB, V2);
        while(digitalRead(T_R)){

        }
      }
      motor::stop();

      // read RGB-sensor
      shift_register::write(SR_PT_RED, HIGH);
      delayMicroseconds(100);
      int16_t green = ADCRead(ADC_PT_RGB);
      shift_register::write(SR_PT_RED, LOW, true);
      shift_register::write(SR_PT_GREEN, HIGH);
      delayMicroseconds(100);
      int16_t red = ADCRead(ADC_PT_RGB);
      shift_register::write(SR_PT_GREEN, LOW);
      if (red - green >= 200 && unloaded == LEFT){
        motor::rev(AB, V2);
        delay(500);
        motor::gyro(V, 180);
        motor::rev(AB, V2);
        delay(1500);
        motor::stop();
        claw::unloadVictims(RIGHT);
        delay(2000);
        claw::closeFlap();
        unloaded |= RIGHT;
      }
      else if (green - red >= 60 && unloaded != 3){
        motor::rev(AB, V2);
        delay(500);
        motor::gyro(V, 180);
        motor::rev(AB, V2);
        delay(1500);
        motor::stop();
        claw::unloadVictims(LEFT);
        delay(2000);
        claw::closeFlap();
        unloaded |= LEFT;
      }
      else{
        motor::rev(AB, V2);
        delay(500);
        motor::gyro(V, -90);
        goto ignore_rgb;
      }
      motor::fwd(AB, V2);
      delay(1000);
      motor::gyro(V, 90);

      if (unloaded == 3){
        skipGap = false;
      }
    }
    
    ignore_rgb:;
    /*
    // find gap in the wall
    ///*if (!skipGap && tof::readLeft() > 500){
      motor::fwd(AB, V);
      delay(700);
      motor::gyro(V, 90);
      motor::fwd(AB, V);
      delay(2000);
      //motor::sensorFwd(V2, V2, 2000, a);
      break; 
    }///
    // Black line => go out
    if (white.left_outer.value < 35 || white.right_outer.value < 35){
      Serial.println("Black line");
      if (!skipGap){
        motor::fwd(AB, V);
        delay(400);
        motor::stop();
        // align with line
        while(!(white.left_outer.value < 35 || white.right_outer.value < 35)){
          motor::fwd(A, V2);
          motor::fwd(B, 40);
          //motor::rev(B, V2);
          white.read();
        }
        shift_register::reset();
        break;
      }
      else{
        motor::rev(AB, V2);
        delay(500);
        motor::gyro(V, -90);
      }
    }
    // Silver line => turn
    if (refL.data.value < 1000 || refR.data.value < 1000){
      Serial.println("jo");
      motor::rev(AB, V2);
      delay(500);
      motor::gyro(V, -90);
    }
    */
    
  }
}
