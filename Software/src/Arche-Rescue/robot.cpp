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

#include "CD74HC4067.h"


Button_sensor Button_sensor_L = Button_sensor(T_L);
Button_sensor Button_sensor_M = Button_sensor(T_M);
Button_sensor Button_sensor_R = Button_sensor(T_R);

Light_sensor Light_sensor_REF_L = Light_sensor(ADC_PT_REF_L, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_L1 = Light_sensor(ADC_PT_L1, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_L0_w = Light_sensor(ADC_PT_L0, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_L0_r = Light_sensor(ADC_PT_L0, 1024, SR_PT_RED);
Light_sensor Light_sensor_L0_g = Light_sensor(ADC_PT_L0, 1024, SR_PT_GREEN);
Light_sensor Light_sensor_L0_b = Light_sensor(ADC_PT_L0, 1024, SR_PT_BLUE);
Light_sensor Light_sensor_M = Light_sensor(ADC_PT_M, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_R0_w = Light_sensor(ADC_PT_R0, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_R0_r = Light_sensor(ADC_PT_R0, 1024, SR_PT_RED);
Light_sensor Light_sensor_R0_g = Light_sensor(ADC_PT_R0, 1024, SR_PT_GREEN);
Light_sensor Light_sensor_R0_b = Light_sensor(ADC_PT_R0, 1024, SR_PT_BLUE);
Light_sensor Light_sensor_R1 = Light_sensor(ADC_PT_R1, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_REF_R = Light_sensor(ADC_PT_REF_R, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_RGB_w = Light_sensor(ADC_PT_RGB, 1024, SR_PT_WHITE);
Light_sensor Light_sensor_RGB_r = Light_sensor(ADC_PT_RGB, 1024, SR_PT_RED);
Light_sensor Light_sensor_RGB_g = Light_sensor(ADC_PT_RGB, 1024, SR_PT_GREEN);
Light_sensor Light_sensor_RGB_b = Light_sensor(ADC_PT_RGB, 1024, SR_PT_BLUE);

CD74HC4067 ADC_multiplexer = CD74HC4067(S0, S1, S2, S3);

void Robot::init() {
  Serial.begin(115200);

  ShiftRegisterInit();
  ShiftRegisterReset();

  Button_sensor_L.init();
  Button_sensor_M.init();
  Button_sensor_R.init();

  Light_sensor_REF_L.init();
  Light_sensor_L1.init();
  Light_sensor_L0_w.init();
  Light_sensor_L0_r.init();
  Light_sensor_L0_g.init();
  Light_sensor_L0_b.init();
  Light_sensor_M.init();
  Light_sensor_R0_w.init();
  Light_sensor_R0_r.init();
  Light_sensor_R0_g.init();
  Light_sensor_R0_b.init();
  Light_sensor_R1.init();
  Light_sensor_REF_R.init();

  return;
}

void Robot::run() {

  Robot::running = true;



  while (true) {

    if (Button_sensor_L.state()) ShiftRegisterWrite(SR_PT_WHITE, 1);
    else ShiftRegisterWrite(SR_PT_WHITE, 0);
    Serial.println(Light_sensor_M.measure());
    if (Light_sensor_M.measure() > 300) {
      ShiftRegisterWrite(SR_PT_WHITE, 1);
    }
    else {
      ShiftRegisterWrite(SR_PT_WHITE, 0);
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

void Robot::measureRaw(){
  int pause = 50;

  

}
