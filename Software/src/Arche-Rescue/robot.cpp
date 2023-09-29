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


Button_sensor Button_sensor_L = Button_sensor(T_L);
Button_sensor Button_sensor_M = Button_sensor(T_M);
Button_sensor Button_sensor_R = Button_sensor(T_R);


void Robot::init() {

  ShiftRegisterInit();
  ShiftRegisterReset();

  ButtonL.init();
  ButtonM.init();
  ButtonR.init();

  return;
}

void Robot::run() {

  Robot::running = true;



  while (true) {

    if (ButtonL.state()) ShiftRegisterWrite(SR_PT_WHITE, 1);
    else ShiftRegisterWrite(SR_PT_WHITE, 0);

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
