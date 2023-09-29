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

Light_sensor Light_sensor_REF_L = Light_sensor(ADC_PT_REF_L);
Light_sensor Light_sensor_L1 = Light_sensor(ADC_PT_L1);
Light_sensor Light_sensor_L0 = Light_sensor(ADC_PT_L0);
Light_sensor Light_sensor_M = Light_sensor(ADC_PT_M);
Light_sensor Light_sensor_R0 = Light_sensor(ADC_PT_R0);
Light_sensor Light_sensor_R1 = Light_sensor(ADC_PT_R1);
Light_sensor Light_sensor_REF_R = Light_sensor(ADC_PT_REF_R);
Light_sensor Light_sensor_RGB = Light_sensor(ADC_PT_RGB);


void Robot::init() {

  ShiftRegisterInit();
  ShiftRegisterReset();

  ButtonL.init();
  ButtonM.init();
  ButtonR.init();

  Light_sensor_M.init();

  return;
}

void Robot::run() {

  Robot::running = true;



  while (true) {

    if (ButtonL.state()) ShiftRegisterWrite(SR_PT_WHITE, 1);
    else ShiftRegisterWrite(SR_PT_WHITE, 0);
    if (Light_sensor_M.measure() > 300) {
      ShiftRegisterWrite(SR_PT_BLUE, 1);
    }
    else {
      ShiftRegisterWrite(SR_PT_BLUE, 0);
    }
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
