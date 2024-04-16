#include "Pins.h"
#include <Arduino.h>

#include "hardware.h"
#include "Wire.h"
#include "menu.h"
#include "eeprom.h"
#include "evac.h"
#include "shared.h"
#include "linefollower.h"

/*
copyright (c) ImmernochKeinName, 2023

licensed under GPL v3. This applies for ALL files included in this Branch

view LICENSE.md for details
*/

#define GREEN_FREEZE 700
uint16_t freeze = 0;
uint16_t obstacle_freeze = 0;
unsigned long timestamp = 0;


// Function to restart the ESP32 in case of a L.o.P.
void IRAM_ATTR isr() {
	ESP.restart();
}

void HardwareInit(){
  /// get the shift register's Pins ///
  Serial.println("Fetching shiftregister and ADC pins...");
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
  // Set Encoder pin
  pinMode(ENC_SW, INPUT);
  Serial.println("Seting up button pins..");
  pinMode(T_L, INPUT_PULLUP);
  pinMode(T_R, INPUT);
  //pinMode(M_S, INPUT);
  Serial.println("Fetching Tof servo pin...");

  Serial.println("Resetting shiftregister pins...");
  shift_register::reset(); /// set all values to LOW
}


void setup(){
  Serial.begin(115200);

  ///// start core 0 //////
  Serial.println("Serial INIT");
  Serial.print("Loop running on core:");
  Serial.println(xPortGetCoreID());

  Wire.begin();
  Wire.setClock(400000); // Fast mode
  Serial.print("WIRE on core:");
  Serial.println(xPortGetCoreID());
  Serial.println("HardwareInit...");
  HardwareInit();

  Serial.print("Display Init...");
  menu::DisplayInit();
  menu::showWaiting("Initializing...");
  
  Serial.print("MPU-detection...");
  if (!gyro::MPU6050Init()){
    Serial.println("\tMISSING MPU5060!!!");
  }
  else{
    Serial.println("\tfound");
  }

  Serial.println("Setting up tof sensors...");
  tof::init();

  Serial.println("Resetting tof servo...");
  tof::rotate(0);
  delay(2000);

  Serial.println("Claw Setup...");
  claw::setup();

  menu::showWaiting("Resetting claw...");
  Serial.println("Resetting claw...");
  claw::up();
  claw::open();
  claw::closeFlap();

  // setup SPIFFS file system for light values
  eeprom::setup();

  #ifdef LED_TEST
    Serial.println("LED test (WGRB)...");
    shift_register::write(SR_PT_WHITE, HIGH);
    delay(2000);
    shift_register::write(SR_PT_WHITE, LOW, true);
    shift_register::write(SR_PT_GREEN, HIGH);
    delay(2000);
    shift_register::write(SR_PT_GREEN, LOW, true);
    shift_register::write(SR_PT_RED, HIGH);
    delay(2000);
    shift_register::write(SR_PT_RED, LOW, true);
    shift_register::write(SR_DE1, LOW);
    delay(2000);
    shift_register::write(SR_DE1, HIGH, true);
  #endif

  #ifdef BLE
    StartBLE();
    BLELoop(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Send placeholder values to avoid timeout
  #endif

  // Menu
  while (true){
    shift_register::write(SR_LED_R_RED, LOW, true);
    shift_register::write(SR_LED_R_BLUE, LOW, true);
    shift_register::write(SR_LED_L_RED, LOW, true);
    shift_register::write(SR_LED_L_BLUE, LOW, true);
    shift_register::write(SR_PT_GREEN, HIGH); // turn on cool green LED's
    post("In Menu...");
    delay(500);
    int option = menu::menu(); // run displayMenu
    if (option == MENU_CALIBRATE){ // handle results

      // calibrate
      menu::showWaiting("Calibration...");
      Serial.println("Calibration...");
      delay(1000);
      calibrate(all_sensors, 5000, 0);
      // Print min/max of WHITE
      Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
      Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
      Serial.print("Back Left max: "); Serial.print(back.left.max); Serial.print(" - Back Right max: "); Serial.println(back.right.max);
      Serial.print("Back Left min: "); Serial.print(back.left.min); Serial.print(" - Back Right min: "); Serial.println(back.right.min);
      eeprom::writeLSData(&white,&green,&red, nullptr, &back); // save light values
    }
    else if (option == MENU_RUN){
      break;
    }
    else if (option == 2){ // Test in Menu - put test code here
      shift_register::reset();
      
      /*while(true){
        shift_register::write(SR_PT_RED, HIGH);
        delayMicroseconds(40);
        int16_t green = ADCRead(ADC_PT_RGB);
        shift_register::write(SR_PT_RED, LOW, true);
        shift_register::write(SR_PT_GREEN, HIGH);
        delayMicroseconds(40);
        int16_t red = ADCRead(ADC_PT_RGB);
        shift_register::write(SR_PT_GREEN, LOW);

        Serial.print("green: "); Serial.print(green); Serial.print("\tred: "); Serial.println(red);
      }*/
      delay(1000);
      attachInterrupt(ENC_SW, isr, RISING);
      evacuationZone();
      detachInterrupt(ENC_SW);
      //motor::sensorFwd(V,V, 7000, ls);
    }
  }

  // Done before Loop
  eeprom::loadLSData(&white,&green,&red, nullptr, &back);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
  Serial.print("Back Left max: "); Serial.print(back.left.max); Serial.print(" - Back Right max: "); Serial.println(back.right.max);
  Serial.print("Back Left min: "); Serial.print(back.left.min); Serial.print(" - Back Right min: "); Serial.println(back.right.min);
  shift_register::reset();
  delay(1000);
  gyro::ResetZAngle();

  xTaskCreatePinnedToCore(core0, "Core0MainLoop", 10000, NULL, 0, &loop0, 0); // create second core loop
  attachInterrupt(ENC_SW, isr, RISING);
  post("Line");
}

void loop() {
    while (true){
      // read light values
      read();
      // Silver Line -> align with silver line
      // left
      if (refL.data.value > 800){
        motor::stop();
        shift_register::write(SR_LED_L_BLUE, LOW);
        while (refR.data.value < 500){
          motor::fwd(B, V3);
          refR.read();
        }
        motor::rev(AB,V2);
        delay(10);
        motor::stop();
        delay(1000);
        break;
      }
      // right
      else if (refR.data.value > 800){
        motor::stop();
        shift_register::write(SR_LED_R_BLUE, LOW);
        while (refL.data.value < 800){
          motor::fwd(A, V3);
          refL.read();
        }
        motor::rev(AB,V2);
        delay(10);
        motor::stop();
        delay(1000);
        break;
      }

      color::update(&white, &green, &red);  // update color checking

      if (color::on_red(RIGHT | LEFT)){ // red line
        motor::stop();
        shift_register::write(SR_LED_R_RED, LOW, true);
        shift_register::write(SR_LED_L_RED, LOW);
        #ifndef NOMOTORS
          delay(7000);
        #endif
        shift_register::write(SR_LED_R_RED, HIGH, true);
        shift_register::write(SR_LED_L_RED, HIGH);
      }

      // debug green detection
      #ifdef NOMOTORS
        if (color::on_green(RIGHT)){
          shift_register::write(SR_LED_R_GREEN, LOW, true);
        }
        else{
          shift_register::write(SR_LED_R_GREEN, HIGH, true);
        }
        if (color::on_green(LEFT)){
          shift_register::write(SR_LED_L_GREEN, LOW);
        }
        else{
          shift_register::write(SR_LED_L_GREEN, HIGH);
        }

      #else

        if (color::on_green(RIGHT | LEFT) && millis()-freeze > GREEN_FREEZE){ // green detected
          motor::hardstop();
          #ifdef DEBUG
            Serial.println("Green Detected!");
          #endif
          // read again to confirm
          for(uint8_t i; i < 15; i++){
            green.read();
            red.read();
            color::update(&white, &green, &red);
          }
          bool left = color::on_green(LEFT);
          bool right = color::on_green(RIGHT);
          shift_register::write(SR_LED_R_GREEN, !right, true); // show side on LED
          shift_register::write(SR_LED_L_GREEN, !left);

          delay(1000);
          if(right || left){
            motor::sensorFwd(V/2, V/2 , 1000, ls); // go fwd, until there is no green
            motor::stop();
            left = left   || color::on_green(LEFT);
            right = right || color::on_green(RIGHT);
            shift_register::write(SR_LED_R_GREEN, !right, true); // show side on LED
            shift_register::write(SR_LED_L_GREEN, !left);
            white.read();
            if((white.left_outer.value < 50 && left) || (white.right_outer.value < 50 && right)){ // check for black line
              motor::fwd(AB, V2);
              delay(600);
              if(left != right){ // only do if not turning 180 degrees
                delay(150);
              }
              motor::stop();
              int16_t turn = 0; // choose turn side
              if (left){ turn += 90;}
              if (right){ turn += 90;}
              if (right && (!left)){turn = -turn;}
              motor::gyro(V, turn); // do the turning
              freeze = millis();
            }
            shift_register::write(SR_LED_R_GREEN, HIGH, true); // LEDs off
            shift_register::write(SR_LED_L_GREEN, HIGH);
          }
        }
      #endif
      ////// LINE FOLLOWING //////
      // look at linefollower.cpp / linefollower.h for details
      //lf();
      static int16_t last = 0;
      #ifdef LF_USE_BLACK_INTRO
        static int black_timestamp;
      #endif

      #ifdef LF_ACTIVE
        #define diff_outer_factor 4 // Factor for the outer light 
        #define mul 2
        
        int16_t mot_diff;
        #ifdef LF_USE_BACK
          int16_t diff_back = back.left.value - back.right.value;
        #endif
        int16_t diff = (white.left.value - white.center.value) - (white.right.value - white.center.value);
        int16_t diff_green = (green.left.value-red.left.value)-(green.right.value-red.right.value); // difference to ignore green value
        int16_t diff_outer = white.left_outer.value - white.right_outer.value;
        mot_diff = ((diff + diff_green*2)*2 + diff_outer*diff_outer_factor) * mul;  // calculate inner to outer mult
        diff_interchange = mot_diff;
        #ifdef LF_USE_BLACK_INTRO
          //shift_register::write(SR_LED_R_BLUE, LOW, true);
          bool right = color::on_black(RIGHT);
          bool left  = color::on_black(LEFT);
          if (left || right){
            black_timestamp = millis();
            //mot_diff += -int(right)*150 + int(left)*150;
          }
          else{
            int t = millis();
            if (black_timestamp + 400 < t && black_timestamp + 2000 > t){
              shift_register::write(SR_LED_R_BLUE, !left, true);
              mot_diff += int(right)*150 - int(left)*150;
            }
          }

        #endif

        #ifndef NOMOTORS
          int16_t v = min(max(V2, int(VZ-mot_diff*1.5)), VZ); // scale bas speed based on difference

          #ifdef LF_USE_BACK
            float scale = 1/(max(abs(mot_diff), 1)*0.5); // only use back LS if no difference

            int16_t vback_a = min(0, int(diff_back)) * scale;
            int16_t vback_b = max(0, int(diff_back)) * scale;
          #else
            #define vback_a 0
            #define vback_b 0
          #endif

          // start motors
          motor::fwd(A, ( v + (mot_diff*2+last)/3) + vback_a);
          motor::fwd(B, ( v - (mot_diff*2+last)/3) + vback_b);
          last = mot_diff;
        #endif
        diff_interchange = mot_diff;
      #endif

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

        #ifdef LF_ACTIVE
          Serial.println(mot_diff);
        #endif
        #ifdef NOMOTORS
          delay(200);
        #endif
      #endif

      ////// OBSTACLE HANDLING //////
      if ((!digitalRead(T_L) || !digitalRead(T_R))){ // if buttons are pressed
        if (obstacle_freeze == 0){
          motor::rev(AB, V); // reverse from the obstacle
          delay(250);
          motor::stop();
          Serial.println("Obstacle Detected!");
          post("Obstacle Detected!");
          delay(500);
          motor::gyro(V, -90); // turn
          motor::stop();
          int16_t rdist;
          while (true){ // go around the obstacle
            rdist = -tof::readLeft(); // calc offset
            motor::fwd(A, (V2+rdist*0.6)); // add offset to best offset
            motor::fwd(B, (V2-rdist*0.6));
            if (abs(rdist) > 300){ // => to near
              motor::fwd(AB, V2);
            }
            white.read();
            if ((white.left_outer.value < 35 || white.right_outer.value < 35)){
              motor::fwd(AB, V2);
              delay(500);
              motor::stop();
              break;
            }
          }
          motor::fwd(AB, V2);
          delay(400);
          motor::stop();
          // turn until sensor is on Black line
          motor::fwd(A, V2);
          motor::rev(B, V2);
          do {
            white.read();
            color::update(&white, &green, &red);
          } while(!(color::on_black(RIGHT)));
          delay(100);
          motor::stop();
          post("Line");
          obstacle_freeze = 3000;
        }
        else{
          motor::rev(AB, V2);
          delay(300);
          motor::gyro(V, 180);
        }
      }

      obstacle_freeze = max(obstacle_freeze-1, 0);
    }
    // navigate the rescue room. check evac.cpp / evac.h for details
    navRoom();

}
////// CORE 0 LOOP //////
/*
Thi core is only used for debug and visualisation and doesn't contribute to the core0 program
*/
void core0(void * pvParameters){
  while (true){
    //menu::showDifference(diff_interchange*0.5, true);
    #ifdef BLE
      BLELoop(
        int(white.left_outer.value),
        int(white.left.value),
        int(white.center.value),
        int(white.right.value),
        int(white.right_outer.value),
        int(refL.data.value),
        int(refR.data.value),
        int(red.left.value),
        int(red.right.value),
        int(green.left.value),
        int(green.right.value)
      );
    #endif
  }
}
