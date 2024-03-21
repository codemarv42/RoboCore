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
  pinMode(T_L, INPUT);
  pinMode(T_R, INPUT);
  pinMode(T_M, INPUT);
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
  claw::close();

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
      /*tof::rotate(90);
      delay(500);*/
      attachInterrupt(ENC_SW, isr, RISING);
      evacuationZone();
      /*int dir = 90;
      bool last_RE_state = analogRead(ENC_B); // used for rotary encoder detection
      while (true){
        bool enc = analogRead(ENC_B); // rotary encoder turn detection
        if (enc != last_RE_state && enc == HIGH){
          if (analogRead(ENC_A) == LOW){
            dir += 10;
            if (dir >= 270){dir = 0;}
            dir -= 10;
            if (dir < 0){dir = 270;}
          }
          else{
            dir -= 10;
            if (dir < 0){dir = 270;}  
          }
          rottof.write(dir);
        }
        last_RE_state = enc;
      }*/

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
            motor::sensorFwd(V/2, V/2 , 300, ls); // go fwd, until there is no green
            motor::stop();
            left = left   || color::on_green(LEFT);
            right = right || color::on_green(RIGHT);
            shift_register::write(SR_LED_R_GREEN, !right, true); // show side on LED
            shift_register::write(SR_LED_L_GREEN, !left);
            white.read();
            if((white.left_outer.value < 50 && left) || (white.right_outer.value < 50 && right)){ // check for black line
              motor::fwd(AB, V2);
              delay(500);
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
      lf();

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
      if (!(bool(digitalRead(T_L)) || bool(digitalRead(T_R)))){ // if buttons are pressed
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
        delay(500);
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
      }
    }
    // navigate the rescue room. check evac.cpp / evac.h for details
    navRoom(ls);

}
////// CORE 0 LOOP //////
/*
Thi core is only used for debug and visualisation and doesn't contribute to the core0 program
*/
void core0(void * pvParameters){
  while (true){
    menu::showDifference(diff_interchange*0.5, true);
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
