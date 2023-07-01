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



void Robot::run() {

  Robot::running = true;

  while (true) {
    
  }

  Robot::running = false;
  
  return;
}

void Robot::init() {
  return;
}

void Robot::input(){
  return;
}

void Robot::output(){
  return;
}