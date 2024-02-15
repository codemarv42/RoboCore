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
#define V 100
#define V2 80
#define V3 40

//FLAGS
//#define BLE
//#define DEBUG
//#define NOMOTORS
//#define LED_TEST
#define LF_ACTIVE

  
// Init Light Sesnsors
LightSensor white = LightSensor(SR_PT_WHITE);
LightSensor red   = LightSensor(SR_PT_RED);
LightSensor green = LightSensor(SR_PT_GREEN);
DirectSensor refL = DirectSensor(SR_PT_WHITE, ADC_PT_REF_L);
DirectSensor refR = DirectSensor(SR_PT_WHITE, ADC_PT_REF_R);

LightSensor* all_sensors[] = {&white,&green,&red,nullptr};  // nullptr is placeholder for an (optional) blue LightSensor

TaskHandle_t loop0; //used for handling the second main loop
int16_t diff_interchange;
static int16_t last = 0;

void HardwareInit(){
  /// get the shift register's Pins ///
  Serial.println("Fetching shiftregister and ADC pins...");
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(ENC_SW, INPUT);
  Serial.println("Seting up button pins..");
  pinMode(T_L, INPUT_PULLUP);
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
  claw::open();

  eeprom::setup();

  #ifdef LED_TEST
    Serial.println("LED test (WGR)...");
    shift_register::write(SR_PT_WHITE, HIGH);
    delay(2000);
    shift_register::write(SR_PT_WHITE, LOW, true);
    shift_register::write(SR_PT_GREEN, HIGH);
    delay(2000);
    shift_register::write(SR_PT_GREEN, LOW, true);
    shift_register::write(SR_PT_RED, HIGH);
    delay(2000);
    shift_register::write(SR_PT_RED, LOW, true);
  #endif

  #ifdef BLE
    StartBLE();
    BLELoop(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Send placeholder values to avoid timeout
  #endif
  
  triangleData* t;

  // Menu
  while (true){
    shift_register::write(SR_PT_GREEN, HIGH); // turn on cool green LED's
    int option = menu::menu(); // run displayMenu
    if (option == MENU_CALIBRATE){ // handle results

      menu::showWaiting("Calibration...");
      Serial.println("Calibration...");
      delay(1000);
      calibrate(all_sensors, 5000, 3);
      // Print min/max of WHITE
      Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
      Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
      eeprom::writeLSData(&white,&green,&red, nullptr); // save light values
    }
    else if (option == MENU_RUN){
      break;
    }
    else if (option == 2){
      //shift_register::write(SR_DE1, HIGH);
      //delay(100);
      //Serial.println(ADCRead(ADC_AE1));
      //shift_register::write(SR_DE1, LOW);
      //delay(1000);
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
      //menu::showWaiting("Baum");
      //claw::unload_victims(true);
    }
  }

  // Done before Loop
  eeprom::loadLSData(&white,&green,&red, nullptr);
  Serial.print("White Left max: "); Serial.print(white.left.max); Serial.print(" - White Right max: "); Serial.println(white.right.max);
  Serial.print("White Left min: "); Serial.print(white.left.min); Serial.print(" - White Right min: "); Serial.println(white.right.min);
  delay(1000);
  gyro::ResetZAngle();

  xTaskCreatePinnedToCore(core0, "Core0MainLoop", 10000, NULL, 0, &loop0, 0); // create second core loop
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
  start:
    while (true){

      if (digitalRead(ENC_SW) == HIGH){
        Serial.println("L.O.P. occured");
        motor::stop();
        delay(4000);
        goto start; // restart loop()
      }

      
      for (auto sensor:all_sensors){ // read light values
        if (sensor != nullptr){sensor->read();}
      }
      refL.read(); // read Reflective sensors
      refR.read();

      // Silver Line -> align with silver line
      // left
      if (refL.data.value > 1000){
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
            motor::sensorFwd(V/2, V/2 , 2500, all_sensors); // go fwd, until there is no green
            motor::stop();
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
        #define mul 2
        
        int16_t mot_diff;
        int16_t diff = (white.left.value - white.center.value) - (white.right.value - white.center.value);
        int16_t diff_green = (green.left.value-red.left.value)-(green.right.value-red.right.value); // difference to ignore green value
        int16_t diff_outer = white.left_outer.value - white.right_outer.value*1.5;
        mot_diff = ((diff+diff_green*2)*4 + diff_outer*diff_outer_factor) * mul;  // calculate inner to outer mult
        diff_interchange = mot_diff;
        
        //cache(mot_diff); // cache W.I.P.
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
          int16_t v = min(max(V2, V-mot_diff), V); // scale bas speed based on difference
          motor::fwd(A, ( v + (mot_diff*2+last)/3));
          motor::fwd(B, ( v - (mot_diff*2+last)/3));
          last = mot_diff;
          diff_interchange = v;
        #endif
      #endif

      ////// RESCUE-KIT //////
      /*if(digitalRead(T_M) == LOW){ // rescue kit detected
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
      }*/

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
          rottof.write(0);
          delay(2000);
          dist = tof::readUpper(); // set approximation dist TODO
          int16_t rdist;
          while (true){
            rdist = 150-tof::readUpper();
            motor::fwd(A, (V2-rdist)*0.6);
            motor::fwd(B, (V2+rdist)*0.6);
            if (abs(rdist) > 300){
              motor::rev(AB, V2);
            }
            white.read();
            if (white.left_outer.value < 35 || white.right_outer.value < 35){
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
          while (true){ // TODO
            rdist = 150-tof::readLeft();
            motor::fwd(A, (V2+rdist)*0.6);
            motor::fwd(B, (V2-rdist)*0.6);
            if (abs(rdist) > 300){
              motor::rev(AB, V2);
            }
            white.read();
            if (white.left_outer.value < 35 || white.right_outer.value < 35){
              break;
            }
          }
          motor::gyro(V, -90);
        }
      }
    }
  

}

////// CORE 0 LOOP //////
void core0(void * pvParameters){
  while (true){
    menu::showDifference(diff_interchange, true);
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
