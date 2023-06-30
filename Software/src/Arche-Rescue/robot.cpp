#include "robot.h"

#include "sensor/ButtonSensor.h"
#include "sensor/GyroSensor.h"
#include "sensor/LightSensor.h"
#include "sensor/TOFSensor.h"
#include "sensor/ElectricSensor.h"

#include "motor/DriveMotor.h"
#include "motor/ServoMotor.h"

#include "led/RGBLed.h"
#include "led/WhiteLed.h"

#include "show/PCShow.h"
#include "show/DisplayShow.h"



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