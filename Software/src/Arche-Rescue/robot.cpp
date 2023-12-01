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

Encoder Rotary_encoder()

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

void Robot::actionLoop(){

  this->running = true;

  while (1) {

    pruefeQuerschwarz();
    pruefeGruen();
    pruefeRot();
    linienFolger();
    gruenerPunkt();
    pruefeSilber();

  }

  this->running = false;
  
  return;
}

void sensorLoop(void* pvParameters){
  while (1) {
    robot.messeLicht();
    Serial.println(Light_sensor_M.val);
  }
}

void secureLoop(){
  continue;
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

void Robot::gruenerPunkt(){
  int rich = 0;
  if ((millis()-schwarz_quer_time) > TIME_SCHWARZ){
    switch (gruen_rich){
      case 0:
        break;
      case 1:
        rich = -1;
        abbiegenGruen(rich);
        break;
      case 2:
        rich = 1;
        abbiegenGruen(rich);
        break;
      case 3:
        kehrtwende();
        break;
      default:
        Serial.println("Fehler: Gruenerkennung");
        break;
    }
  };
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
  his_wL1[9] = (int) (Light_sensor_L1);
  his_wR1[9] = (int) (Light_sensor_R1);
  mwL  = (int) mwL/10;
  mwR  = (int) mwR/10;
  if (mwL < SCHWARZ){
    schwarz_quer_rich = -1;
    schwarz_quer_time = millis();
  };
  if (mwR < SCHWARZ){
    schwarz_quer_rich = 1;
    schwarz_quer_time = millis();
  };
  return;
}

void Robot::pruefeGruen(){
  int mwL = his_gL1[0];
  int mwR = his_gR1[0];
  for (i = 0; i < 10;i++){
    his_gL1[i] = his_gL1[i+1];
    his_gR1[i] = his_gR1[i+1];
    mwL = (int) mwL + his_gL1[i];
    mwR = (int) mwR + his_gR1[i];
  }
  his_gL1[9] = (int) (Light_sensor_L0_g - Light_sensor_L0_r);
  his_gR1[9] = (int) (Light_sensor_R0_g - Light_sensor_R0_r);
  mwL  = (int) mwL/10;
  mwR  = (int) mwR/10;
  gruen_rich = 0;
  if ((mwL > GRUEN_MIN)&&(mwL < GRUEN_MAX)){
    gruen_rich += 1;
  };
  if ((mwR > GRUEN_MIN)&&(mwR < GRUEN_MAX)){
    gruen_rich += 2;
  };
  return;
}

void Robot::pruefeRot(){
  int mwL = his_rL1[0];
  int mwR = his_rR1[0];
  for (i = 0; i < 10;i++){
    his_rL1[i] = his_rL1[i+1];
    his_rR1[i] = his_rR1[i+1];
    mwL = (int) mwL + his_rL1[i];
    mwR = (int) mwR + his_rR1[i];
  }
  his_rL1[9] = (int) (Light_sensor_L0_r - Light_sensor_L0_g);
  his_rR1[9] = (int) (Light_sensor_R0_r - Light_sensor_R0_g);
  mwL  = (int) mwL/10;
  mwR  = (int) mwR/10;
  if (((mwL > ROT_MIN)&&(mwL < ROT_MAX))&&((mwR > ROT_MIN)&&(mwR < ROT_MAX))){
    Motor_L.Off();
    Motor_R.Off();
    Serial.println("Ende");
    delay(10000); //Ende
  };
  return;
}

void Robot::pruefeSilber(){
  int mwL = his_sL1[0];
  int mwR = his_sR1[0];
  for (i = 0; i < 10;i++){
    his_sL1[i] = his_sL1[i+1];
    his_sR1[i] = his_sR1[i+1];
    mwL = (int) mwL + his_sL1[i];
    mwR = (int) mwR + his_sR1[i];
  }
  his_sL1[9] = (int) (Light_sensor_REF_L);
  his_sR1[9] = (int) (Light_sensor_REF_R);
  mwL  = (int) mwL/10;
  mwR  = (int) mwR/10;
  if ((mwL > SILBER)&&(mwR > SILBER)){
    Serial.println("Silber");
    secureLoop();
  };
  return;
}

void Robot::abbiegenGruen(int rich){
  continue;
}

void Robot::kehrtwende(){
  continue;
}
