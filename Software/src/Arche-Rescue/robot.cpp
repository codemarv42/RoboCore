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

#include "pc_show.h"
#include "display_show.h"

#include "shift_register.h"

#include "CD74HC4067.h"

Robot robot = Robot();

Motor Motor_L = Motor(PWMA, SR_AIN1, SR_AIN2, SR_STBY);
Motor Motor_R = Motor(PWMB, SR_BIN1, SR_BIN2, SR_STBY);

Button_sensor Button_sensor_L = Button_sensor(T_L);
Button_sensor Button_sensor_M = Button_sensor(T_M);
Button_sensor Button_sensor_R = Button_sensor(T_R);

Light_sensor Light_sensor_REF_L = Light_sensor(ADC_PT_REF_L, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_L1 = Light_sensor(ADC_PT_L1, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_L0_w = Light_sensor(ADC_PT_L0, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_L0_r = Light_sensor(ADC_PT_L0, 4096, SR_PT_RED);
Light_sensor Light_sensor_L0_g = Light_sensor(ADC_PT_L0, 4096, SR_PT_GREEN);
Light_sensor Light_sensor_L0_b = Light_sensor(ADC_PT_L0, 4096, SR_PT_BLUE);
Light_sensor Light_sensor_M = Light_sensor(ADC_PT_M, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_R0_w = Light_sensor(ADC_PT_R0, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_R0_r = Light_sensor(ADC_PT_R0, 4096, SR_PT_RED);
Light_sensor Light_sensor_R0_g = Light_sensor(ADC_PT_R0, 4096, SR_PT_GREEN);
Light_sensor Light_sensor_R0_b = Light_sensor(ADC_PT_R0, 4096, SR_PT_BLUE);
Light_sensor Light_sensor_R1 = Light_sensor(ADC_PT_R1, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_REF_R = Light_sensor(ADC_PT_REF_R, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_RGB_w = Light_sensor(ADC_PT_RGB, 4096, SR_PT_WHITE);
Light_sensor Light_sensor_RGB_r = Light_sensor(ADC_PT_RGB, 4096, SR_PT_RED);
Light_sensor Light_sensor_RGB_g = Light_sensor(ADC_PT_RGB, 4096, SR_PT_GREEN);
Light_sensor Light_sensor_RGB_b = Light_sensor(ADC_PT_RGB, 4096, SR_PT_BLUE);

RGB_led RGB_led_L = RGB_led(SR_LED_L_RED, SR_LED_L_GREEN, SR_LED_L_BLUE);
RGB_led RGB_led_R = RGB_led(SR_LED_R_RED, SR_LED_R_GREEN, SR_LED_R_BLUE);

CD74HC4067 ADC_multiplexer = CD74HC4067(S0, S1, S2, S3);

void Robot::init() {
  Serial.begin(115200);

  ShiftRegisterInit();
  ShiftRegisterReset();

  Motor_L.init();
  Motor_R.init();

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

  kalibriere_LS(ANZ_KAL);

  return;
}

void Robot::actionLoop() {

  this->running = true;

  while (1) {

    if (Light_sensor_M.val > 80){
      RGB_led_L.cyan();
    }
    else{
      RGB_led_L.magenta();
    }
    delay(10);
  }

  this->running = false;
  
  return;
}

void sensorLoop(void* pvParameters) {
  while (1) {
    robot.messeLicht();
    Serial.println(Light_sensor_M.val);
  }
}

void Robot::input(){
  return;
}

void Robot::output(){
  return;
}

void Robot::kalibriere_LS(int anz){
  for (int i = 0; i < anz; i++){
    Light_sensor_L1.calibrate();
    Light_sensor_L0_w.calibrate();
    Light_sensor_L0_r.calibrate();
    Light_sensor_L0_g.calibrate();
    Light_sensor_L0_b.calibrate();
    Light_sensor_M.calibrate();
    Light_sensor_R0_w.calibrate();
    Light_sensor_R0_r.calibrate();
    Light_sensor_R0_g.calibrate();
    Light_sensor_R0_b.calibrate();
    Light_sensor_R1.calibrate();
    delay(100);
  }
}

void Robot::messeLicht(){
  
  Light_sensor_L1.measure();
  Light_sensor_L0_w.measure();
  Light_sensor_L0_r.measure();
  Light_sensor_L0_g.measure();
  Light_sensor_L0_b.measure();
  Light_sensor_M.measure();
  Light_sensor_R0_w.measure();
  Light_sensor_R0_r.measure();
  Light_sensor_R0_g.measure();
  Light_sensor_R0_b.measure();
  Light_sensor_R1.measure();

}

void Robot::linienFolger(){
  int diff = 0;
  diff = (int) ((Light_sensor_L0_w.val - Light_sensor_R0_w.val) * FAKTOR);
  diff = (int) diff + ((Light_sensor_L1.val - Light_sensor_R1.val) * FAKTOR * 2);
  Motor_L.Fwd(SOLL + diff);
  Motor_R.Fwd(SOLL - diff);
  return;
}

void Robot::pruefeQuerschwarz(){
  int mwL = his_wL1[0];
  int mwR = his_wR1[0];
  for (i = 0; i < 10;i++){
    his_wL1[i] = his_wL1[i+1];
    his_wR1[i] = his_wR1[i+1];
    mwL = (int) mwL + his_wL1[i];
    mwR = (int) mwR + his_wR1[i];
  }
  his_wL1[9] = (int) (Light_sensor_L1 + Light_sensor_R1) / 2;
  his_wR1[9] = (int) (Light_sensor_L1 + Light_sensor_R1) / 2;
  mwL  = (int) mwL/10;
  mwR  = (int) mwR/10;
  if (mwL < SCHWARZ)
  return;
}
