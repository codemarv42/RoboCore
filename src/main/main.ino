#include "Pins.h"
#include <Arduino.h>

#include "hardware.h"
#include "Wire.h"
#include <time.h>
#include "menu.h"
#include "eeprom.h"
#include "evac.h"

/*
copyright (c) ImmernochKeinName, 2023

licensed under GPL v3. This applies for ALL files included in this Branch

view LICENSE.md for details
*/


// SPEED
#define VZ 100
#define V 100
#define V2 80
#define V3 40

//FLAGS
//#define BLE
//#define DEBUG
//#define NOMOTORS
//#define LED_TEST
#define LF_ACTIVE
#define LF_USE_BACK

  
// Init Light Sesnsors
LightSensorArray white(SR_PT_WHITE);
LightSensorArray red(SR_PT_RED);
LightSensorArray green(SR_PT_GREEN);
DirectSensor refL(SR_PT_WHITE, ADC_PT_REF_L);
DirectSensor refR(SR_PT_WHITE, ADC_PT_REF_R);
LightSensorPair back(SR_DE1, ADC_AE2, ADC_AE3);

LSBase* all_sensors[] = {&white,&green,&red,nullptr, &refL,&refR, &back};  // nullptr is placeholder for an (optional) blue LightSensorArray
LightSensorArray* ls[] = {&white,&green,&red,nullptr};

TaskHandle_t loop0; //used for handling the second main loop
int16_t diff_interchange;
static int16_t last = 0;

void IRAM_ATTR isr() {
	ESP.restart();
}

void HardwareInit(){
  /// get the shift register's Pins ///
  Serial.println("Fetching shiftregister and ADC pins...");
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
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
  rottof.write(90);
  delay(2000);

  Serial.println("Claw Setup...");
  claw::setup();

  menu::showWaiting("Resetting claw...");
  Serial.println("Resetting claw...");
  claw::up();
  claw::close();

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
  
  triangleData* t;

  // Menu
  while (true){
    shift_register::write(SR_PT_GREEN, HIGH); // turn on cool green LED's
    post("In Menu...");
    int option = menu::menu(); // run displayMenu
    if (option == MENU_CALIBRATE){ // handle results

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
    else if (option == 2){
      shift_register::reset();
      //evacuationZone();
      //navRoom(&white);
      rottof.write(40);
      /*while (true){
        uint16_t upper = tof::readUpper();
        uint16_t lower = tof::readLower();
        Serial.print(upper);
        Serial.print("\t");
        Serial.print(lower);
        Serial.print("\t");
        Serial.println(upper - lower);
        delay(100);
      }*/
      //claw::down();
      //claw::open();
      shift_register::write(SR_DE1, LOW);
      delayMicroseconds(80);
      Serial.println(ADCRead(ADC_AE3));
      delay(1000);
      shift_register::write(SR_DE1, HIGH);
    }
  }

  // Done before Loop
  eeprom::loadLSData(&white,&green,&red, nullptr, &back);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
  Serial.print("Back Left max: "); Serial.print(back.left.max); Serial.print(" - Back Right max: "); Serial.println(back.right.max);
  Serial.print("Back Left min: "); Serial.print(back.left.min); Serial.print(" - Back Right min: "); Serial.println(back.right.min);
  delay(1000);
  gyro::ResetZAngle();

  xTaskCreatePinnedToCore(core0, "Core0MainLoop", 10000, NULL, 0, &loop0, 0); // create second core loop
  attachInterrupt(ENC_SW, isr, RISING);
}

void loop() {
    while (true){
      for (auto sensor:all_sensors){ // read light values
        if (sensor != nullptr){sensor->read();}
      }

      // Silver Line -> align with silver line
      // left
      if (refL.data.value > 1000){
        motor::rev(AB, V3);
        delay(250);
        motor::stop();
        shift_register::write(SR_LED_L_BLUE, LOW);
        while (refR.data.value < 1000){
          motor::fwd(B, V3);
          motor::rev(A, V3/2);
          refR.read();
        }
        motor::rev(AB,V2);
        delay(10);
        motor::stop();
        delay(1000);
        break;
      }
      // right
      else if (refR.data.value > 1000){
        motor::rev(AB, V3);
        delay(250);
        motor::stop();
        shift_register::write(SR_LED_R_BLUE, LOW);
        while (refL.data.value < 1000){
          motor::fwd(A, V3);
          motor::rev(B, V3/2);
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
      #endif

      #ifndef NOMOTORS
        if (color::on_green(RIGHT | LEFT)){
          //delay(25);
          motor::stop();
          motor::rev(AB, V/2);
          delay(25);
          motor::stop();
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
            motor::sensorFwd(V/2, V/2 , 700, ls); // go fwd, until there is no green
            motor::stop();
            left = left   || color::on_green(LEFT);
            right = right || color::on_green(RIGHT);
            shift_register::write(SR_LED_R_GREEN, !right, true); // show side on LED
            shift_register::write(SR_LED_L_GREEN, !left);
            white.read();
            if((white.left_outer.value < 50 && left) || (white.right_outer.value < 50 && right)){ // check for black line
              delay(1000);
              if(left != right){ // only do if not turning 180 degrees
                motor::fwd(AB, V2);
                delay(500);
                motor::stop();
              }
              int16_t turn = 0; // choose turn side
              if (left){ turn += 90;}
              if (right){ turn += 90;}
              if (right && (!left)){turn = -turn;}
              motor::gyro(V, turn);
              motor::fwd(AB, V);
              delay(100);
              motor::stop();
            }
            shift_register::write(SR_LED_R_GREEN, HIGH, true); // LEDs off
            shift_register::write(SR_LED_L_GREEN, HIGH);
          }
        }
      #endif
      ////// LINE FOLLOWING //////
      #ifdef LF_ACTIVE
        #define diff_outer_factor 2 // Factor for the outer light 
        #define mul 1
        
        int16_t mot_diff;
        #ifdef LF_USE_BACK
          int16_t diff_back = back.left.value - back.right.value;
        #endif
        int16_t diff = (white.left.value - white.center.value) - (white.right.value - white.center.value);
        int16_t diff_green = (green.left.value-red.left.value)-(green.right.value-red.right.value); // difference to ignore green value
        int16_t diff_outer = white.left_outer.value - white.right_outer.value;
        mot_diff = ((diff+diff_green*2)*4 + diff_outer*diff_outer_factor) * mul;  // calculate inner to outer mult
        diff_interchange = mot_diff;

        #ifndef NOMOTORS
          int16_t v = min(max(V2, int(VZ-mot_diff*1.5)), VZ); // scale bas speed based on difference

          #ifdef LF_USE_BACK
            float scale = 1/max(abs(mot_diff), 1);

            int16_t vback_a = max(0, int(diff_back)) * scale;
            int16_t vback_b = min(0, int(diff_back)) * scale;
          #else
            #define vback_a 1;
            #define vback_b 1;
          #endif

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
        Serial.println(mot_diff);
        delay(200);
      #endif

      ////// OBSTACLE HANDLING //////
      if (!(bool(digitalRead(T_L)) || bool(digitalRead(T_R)))){ // if buttons are pressed
        motor::rev(AB, V); // reverse from the obstacle
        delay(250);
        motor::stop();
        Serial.println("Obstacle Detected!");
        delay(500);
        uint16_t dist = tof::readLeft();
        if (dist > 250){ // => check if there is enough space to pass on this side
          motor::gyro(V, 90);
          motor::stop();
          Serial.println("Baum");
          rottof.write(-10);
          delay(2000);
          dist = tof::readUpper(); // set approximation dist TODO
          int16_t rdist;
          uint timestamp = millis();
          while (true){
            rdist = 100-tof::readUpper();
            motor::fwd(A, (V2-rdist));
            motor::fwd(B, (V2+rdist));
            if (abs(rdist) > 300){
              motor::rev(AB, V3);
            }
            white.read();
            if ((white.left_outer.value < 35 || white.right_outer.value < 35) && millis() > timestamp + 5000){
              //motor::fwd(B, V2);
              //delay(500);
              motor::stop();
              break;
            }
          }
          rottof.write(90);
          motor::gyro(V, 90);
        }
        else{
          motor::gyro(V, -90);
          motor::stop();
          int16_t rdist;
          while (true){
            rdist = 50-tof::readLeft();
            motor::fwd(A, (V2+rdist*0.6));
            motor::fwd(B, (V2-rdist*0.6));
            if (abs(rdist) > 300){
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
          motor::gyro(V, -90);
        }
      }
    }

    navRoom(&white);
}

////// CORE 0 LOOP //////
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
