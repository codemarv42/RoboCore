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
#define V2 140
#define V3 40
#define BLE
#define DEBUG
#define NOMOTORS
//#define LED_TEST
//#define LF_ACTIVE

  
// Init Light Sesnsors
LightSensor white = LightSensor(SR_PT_WHITE);
LightSensor red   = LightSensor(SR_PT_RED);
LightSensor green = LightSensor(SR_PT_GREEN);
DirectSensor refL = DirectSensor(SR_PT_WHITE, ADC_PT_REF_L);
DirectSensor refR = DirectSensor(SR_PT_WHITE, ADC_PT_REF_R);

LightSensor* all_sensors[] = {&white,&green,&red,nullptr};  // nullptr is placeholder for an (optional) blue LightSensor
Servo rottof;

TaskHandle_t loop0; //used for handling the second main loop
int16_t diff_interchange;

bool HardwareInit(){
  /// get the shift register's Pins ///
  claw::setup();
  claw::up();
  claw::open();
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(T_L, INPUT_PULLUP);
  pinMode(T_R, INPUT);
  pinMode(T_M, INPUT);
  rottof.attach(18);

  shift_register::reset(); /// set all values to LOW
  return true;
}


void setup(){
  Serial.begin(115200);

  ///// start core 0 //////
  //Serial.println("Hallo");
  Serial.print("Loop running on core:");
  Serial.println(xPortGetCoreID());
  //xTaskCreatePinnedToCore(core0, "Core0MainLoop", 10000, NULL, 0, &loop0, 0);

  Wire.begin();
  Serial.println("HardwareInit...");
  HardwareInit();
  
  Serial.println("MPU-detection...");
  gyro::MPU6050Init();
  Serial.println("Resetting Claw...");
  claw::up(); // reset the claw
  claw::close();
  tof::init();
  rottof.write(90);
  delay(2000);/*
  rottof.write(180);
  delay(2000);*/
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
  shift_register::write(SR_PT_GREEN, HIGH); // turn on cool green LED's
  delay(1000);
  gyro::ResetZAngle();
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
  refL.read(); // read Reflective sensors
  refR.read();

  // Silver Line -> align with silver line
  // left
  if (refL.data.value > 1000){
    shift_register::write(SR_LED_L_BLUE, LOW);
    while (refR.data.value < 1000){
      motor::fwd(B, V2);
      motor::rev(A, V2/2);
      refR.read();
    }
    motor::stop();
    delay(1000);
  }
  // right
  else if (refR.data.value > 1000){
    shift_register::write(SR_LED_R_BLUE, LOW);
    while (refL.data.value < 1000){
      motor::fwd(A, V2);
      motor::rev(B, V2/2);
      refL.read();
    }
    motor::stop();
    delay(1000);
  }


  color::update(&white, &green, &red);  // update color checking

  if (color::on_red(RIGHT | LEFT)){ // red line
    motor::stop();
    shift_register::write(SR_LED_R_RED, LOW);
    shift_register::write(SR_LED_L_RED, LOW);
    delay(7000);
    shift_register::write(SR_LED_R_RED, HIGH);
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
      #ifdef DEBUG
        Serial.println("Green Detected!");
      #endif
      bool left = color::on_green(LEFT);
      bool right = color::on_green(RIGHT);
      shift_register::write(SR_LED_R_GREEN, !right); // show side on LED
      shift_register::write(SR_LED_L_GREEN, !left);
      motor::stop();
      delay(1000);
      if(right || left){
        motor::sensorFwd(V/2, V/2 , 2000, all_sensors); // go fwd, until there is no green
        motor::stop();
        //motor::readFwd(AB, V, 1000, all_sensors);
        white.read();
        if(white.left_outer.value < 50 || white.right_outer.value < 50){ // check for black line
          delay(1000);
          int16_t turn = 0; // choose turn side W.I.P.
          if (left){ turn += 80;}
          if (right){ turn += 80;}
          if (right && (!left)){turn = -turn;}
          motor::gyro(V, turn);
          motor::fwd(AB, V);
          delay(300);
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
    int16_t diff = (white.left.value - white.center.value) - (white.right.value - white.center.value);
    int16_t diff_outer = white.left_outer.value - white.right_outer.value;
    int16_t diff_green = (green.left.value-red.left.value)-(green.right.value-red.right.value); // difference to ignore green value
    //if (abs(diff_outer) < 25){diff_outer = 0;} // set diff to 0 when no difference is recognised
    mot_diff = ((diff+diff_green*2)*4 + diff_outer*diff_outer_factor) * mul;  // calculate inner to outer mult
    diff_interchange = mot_diff;
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
      int16_t v = V;
      if (mot_diff > 100){
        v = V2;
      }
      motor::fwd(A, ( v + mot_diff)); // TODO: change both sides to be equal, when hardware-problem is solved
      motor::fwd(B, ( v - mot_diff));
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
      rottof.write(0);
      delay(2000);
      dist = tof::readUpper(); // set approximation dist
      int16_t rdist;
      while (true){ // TODO
        rdist = 150-tof::readUpper();
        motor::fwd(A, (V2-rdist)*0.6);
        motor::fwd(B, (V2+rdist)*0.6);
        
      }
    }
    else{
      motor::gyro(V, -90);
      motor::stop();
      int16_t rdist;
      while (true){ // TODO
        rdist = 150-tof::readLeft();
        motor::fwd(A, (V2+rdist)*0.6);
        motor::fwd(B, (V2-rdist)*0.6);
      }
    }
  }
}

////// CORE 0 LOOP //////
void core0(void * pvParameters){
  pinMode(ENC_SW, INPUT);
  
  // Begin I2C
  //Wire.begin();
  Serial.print("WIRE on Core");
  Serial.println(xPortGetCoreID());

  // Init Display
  DisplayInit();
  delay(1000);
  showGyroWaiting();

  delay(2000);
  //menu();

  // Start BLE
  #ifdef BLE
    StopBLE();
    StartBLE();
  #endif
  
  while (true){
    //delay(100);
    showDifference(diff_interchange, "D", true);
    #ifdef BLE
      BLELoop(
        int(white.left_outer.value),
        int(white.left.value),
        int(white.center.value),
        int(white.right.value),
        int(white.right_outer.value),
        0,
        0,
        int(red.left.value),
        int(red.right.value),
        int(green.left.value),
        int(green.right.value)
        );
    #endif
  }
}
