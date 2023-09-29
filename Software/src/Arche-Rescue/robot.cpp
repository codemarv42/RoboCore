#include "Arduino.h"
#include "my_pins.h"
#include "robot.h"

#include "button_sensor.h"
#include "gyro_sensor.h"
#include "light_sensor.h"
#include "tof_sensor.h"
#include "electric_sensor.h"

#include "drive_motor.h"
#include "servo_motor.h"

#include "rgb_led.h"
#include "white_led.h"

#include "pc_show.h"
#include "display_show.h"

#include "shift_register.h"

void Robot::init() {
  ShiftRegisterInit();
  ShiftRegisterReset();
  // ShiftRegisterWrite(SR_PT_WHITE, HIGH);
  return;
}

void Robot::run() {

  Robot::running = true;

  while (true) {
    
  }

  Robot::running = false;
  
  return;
}

void Robot::input(){
  return;
}

void Robot::output(){
  return;
}