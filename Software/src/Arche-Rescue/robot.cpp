#include "Arduino.h"
#include "my_pins.h"
#include "robot.h"

#include "rotary_encoder.h"
#include "button_sensor.h"
#include "rotary_encoder.h"
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

#include "Wire.h"
#include "CD74HC4067.h"
#include "SPI.h"
#include "EEPROM.h"


Robot robot = Robot();

Motor Motor_R = Motor(PWMA, SR_AIN1, SR_AIN2, SR_STBY,0);
Motor Motor_L = Motor(PWMB, SR_BIN1, SR_BIN2, SR_STBY,1);

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
Light_sensor* lightsensors[ANZ_LS] = {&Light_sensor_REF_L,&Light_sensor_L1,&Light_sensor_L0_w,&Light_sensor_L0_r,&Light_sensor_L0_g,&Light_sensor_L0_b,&Light_sensor_M,&Light_sensor_R0_w,&Light_sensor_R0_r,&Light_sensor_R0_g,&Light_sensor_R0_b,&Light_sensor_R1,&Light_sensor_REF_R};

RGB_led RGB_led_L = RGB_led(SR_LED_L_RED, SR_LED_L_GREEN, SR_LED_L_BLUE);
RGB_led RGB_led_R = RGB_led(SR_LED_R_RED, SR_LED_R_GREEN, SR_LED_R_BLUE);

Rotary_encoder Rotary = Rotary_encoder();

CD74HC4067 ADC_multiplexer = CD74HC4067(S0, S1, S2, S3);

Gyro_sensor MPU = Gyro_sensor();

void Robot::init() {
  Serial.begin(115200);
  Wire.begin();

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

  pinMode(ENC_A,INPUT);
  pinMode(ENC_B,INPUT);
  pinMode(ENC_SW,INPUT);

  while (true){
    Rotary.measure();
    if (Rotary.counter == 1){
      kalibriere_LS(ANZ_KAL);
      schreibeKalWerte();
      break;
    }
    if (Rotary.counter == -1){
      leseKalWerte();
      break;
    }
  }

  Serial.println("vor MPU");

  // MPU.init();

  Serial.println("nach MPU");

  return;
}

void Robot::actionLoop(){

  this->running = true;

  while (1) {
    messeLicht();
    linienFolger();
    pruefeRot();
    pruefeQuerschwarz();
    gruenerPunkt();

    // Serial.print("L0: ");
    // Serial.println(Light_sensor_L0_w.val);
    // Serial.print("R0: ");
    // Serial.println(Light_sensor_R0_w.val);
    // Serial.print("L1: ");
    // Serial.println(Light_sensor_L1.val);
    Serial.print("GruenL: ");
    Serial.println(Light_sensor_L0_g.val-Light_sensor_L0_r.val);
    Serial.print("GruenR: ");
    Serial.println(Light_sensor_R0_g.val-Light_sensor_R0_r.val);
    // Serial.print("Rot: ");
    // Serial.println(Light_sensor_L0_r.val-Light_sensor_L0_g.val);
    // Serial.print("Silber: ");
    // Serial.println(Light_sensor_REF_L.val);
    // delay(1000);
    
    // Serial.println(Light_sensor_M.val);
    // Serial.println(Light_sensor_L0_w.val);
    // delay(200);
    // gruenerPunkt();
    // pruefeSilber();
    // Serial.println("Core 1 is running");
    // Serial.println(battery_voltage());
    // Serial.print("X : ");
    // Serial.println(MPU.AngleX);
    // Serial.print("Y : ");
    // Serial.println(MPU.AngleY);
    // Serial.print("Z : ");
    // Serial.println(MPU.AngleZ);
    // Serial.print("AccY : ");
    // Serial.println(MPU.AccelY);
    // Serial.println("");

  }

  this->running = false;
  
  return;
}

void sensorLoop(void* pvParameters){
  while (1) {
    if (!robot.running){
      continue;
    }
    robot.messeLicht();
    // MPU.update();

    // Rotary.measure();
  }
}

void Robot::secureLoop(){
  return;
}

void Robot::input(){
  return;
}

void Robot::output(){
  return;
}

float Robot::battery_voltage(){
  return 1649/649*analogRead(VBAT);
}

void Robot::kalibriere_LS(int anz){
  for (int i = 0; i < anz; i++){
    for (int j = 0; j < ANZ_LS; j++){
      lightsensors[j]->calibrate();
    }
    delay(100);
  }
}

void Robot::schreibeKalWerte() {
  int addr = 10;
  for (int i = 0; i < ANZ_LS; i++){
    EEPROM.write (addr, lightsensors[i]->max >> 8);
    EEPROM.write (addr + 1, lightsensors[i]->max & 0xff);
    EEPROM.write (addr + 2,lightsensors[i]->min >> 8);
    EEPROM.write (addr + 3, lightsensors[i]->min & 0xff);
    addr = addr + 4;
  }
}

void Robot::leseKalWerte() {    //liest falsch min > max
  int addr = 10;
  for (int i = 0; i < ANZ_LS; i++){
    lightsensors[i]->max = EEPROM.read (addr);
    lightsensors[i]->max = lightsensors[i]->max << 8;
    lightsensors[i]->max = lightsensors[i]->max | EEPROM.read (addr + 1);
    lightsensors[i]->min = EEPROM.read (addr + 2);
    lightsensors[i]->min = lightsensors[i]->min << 8;
    lightsensors[i]->min = lightsensors[i]->min | EEPROM.read (addr + 3);
    addr = addr + 4;
  }
}

void Robot::messeLicht(){
  for (int i = 0; i < ANZ_LS; i++){
    lightsensors[i]->measure();
  }
}

void Robot::linienFolger(){
  int diff = 0;
  diff = (int) ((Light_sensor_L0_w.val - Light_sensor_R0_w.val) * FAKTOR);
  diff += (int) (Light_sensor_L1.val - Light_sensor_R1.val) * FAKTOR/1.5;
  Motor_L.Fwd(SOLL + diff);
  Motor_R.Fwd(SOLL - diff);
  // Serial.println(diff);
  return;
}

void Robot::gruenerPunkt(){
  if ((millis()-schwarz_quer_time) > TIME_SCHWARZ){
    int gruen_rich = 0;
    int unterl = Light_sensor_L0_g.val - Light_sensor_L0_r.val;
    int unterr = Light_sensor_R0_g.val - Light_sensor_R0_r.val;
    if ((unterl > GRUEN_MIN)&&(unterl < GRUEN_MAX)){
      count_green_l += 1;
    }
    else{
      count_green_l -=1;
      if(count_green_l < 0){
        count_green_l = 0;
      };
    }
    if ((unterr > GRUEN_MIN)&&(unterr < GRUEN_MAX)){
      count_green_r += 1;
    }
    else{
      count_green_r -= 1;
      if(count_green_r < 0){
        count_green_r = 0;
      };
    }
    if(count_green_l==ANZ_GRUEN){
      gruen_rich += 1;
      count_green_l = 0;
      if(count_green_r>0){
        gruen_rich += 2;
        count_green_r = 0;
      };
    }
    else if(count_green_r==ANZ_GRUEN){
      gruen_rich += 2;
      count_green_r = 0;
      if(count_green_l>0){
        gruen_rich += 1;
        count_green_l = 0;
      };
    };
    Serial.println(gruen_rich);
    int rich = 0;
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
  if(Light_sensor_L1.val<SCHWARZ){
    count_schwarz_l +=1;
    if(count_schwarz_l == ANZ_SCHWARZ){
      schwarz_quer_time = millis();
      count_schwarz_l = 0;
      return;
    };
  }
  else{
    count_schwarz_l -=1;
    if(count_schwarz_l < 0){
      count_schwarz_l = 0;
    };
  }
  if(Light_sensor_R1.val<SCHWARZ){
    count_schwarz_r +=1;
    if(count_schwarz_r == ANZ_SCHWARZ){
      schwarz_quer_time = millis();
      count_schwarz_r = 0;
      return;
    };
  }
  else{
    count_schwarz_r -=1;
    if(count_schwarz_r < 0){
      count_schwarz_r = 0;
    };
  }
  return;
}

void Robot::pruefeRot(){
  int unterl = Light_sensor_L0_r.val - Light_sensor_L0_g.val;
  int unterr = Light_sensor_R0_r.val - Light_sensor_R0_g.val;
  if ((unterl > ROT_MIN)&&(unterl < ROT_MAX)){
    count_red_l += 1;
  }
  else{
    count_red_l -= 1;
    if(count_red_l < 0){
      count_red_l = 0;
    };
  }
  if ((unterr > ROT_MIN)&&(unterr < ROT_MAX)){
    count_red_r += 1;
  }
  else{
    count_red_r -= 1;
    if(count_red_r < 0){
      count_red_r = 0;
    };
  }
  if ((count_red_l>ANZ_ROT)||(count_red_r>ANZ_ROT)){
    Motor_L.Off();
    Motor_R.Off();
    Serial.println("Ende");
    delay(10000); //Ende
    count_red_l = 0;
    count_red_r = 0;
  };
  return;
}

void Robot::pruefeSilber(){
  if(Light_sensor_REF_L.val>SILBER){
    count_silver_l +=1;
  }
  else{
    count_silver_l -=1;
    if(count_silver_l < 0){
      count_silver_l = 0;
    };
  }
  if(Light_sensor_REF_R.val>SILBER){
    count_silver_r +=1;
  }
  else{
    count_silver_r -=1;
    if(count_silver_r < 0){
      count_silver_r = 0;
    };
  }
  if ((count_silver_l > ANZ_SILBER)&&(count_silver_r > ANZ_SILBER)){
    count_silver_l = 0;
    count_silver_r = 0;
    Serial.println("Silber");
    secureLoop();
  };
  return;
}

void Robot::abbiegenGruen(int rich){
  Motor_L.Off();
  Motor_R.Off();
  delay(3000);
  return;
}

void Robot::kehrtwende(){
  Motor_L.Fwd(-100);
  Motor_R.Fwd(-100);
  delay(3000);
  return;
}
