
#include "Pins.h"
#include "MPU6050_light.h"
#include "CD74HC4067.h"
#include <Arduino.h>

bool ShiftRegisterBits[24] = {0};

MPU6050 mpu(Wire);
CD74HC4067 ADCMULTI(S0, S1, S2, S3);
unsigned long timer = 0;

float XAngle;
float YAngle;
float ZAngle;
float CorrectionValueZAngle = 0.00;
float XAccel;


void ShiftRegisterWrite(int pin, bool state) {  //Sets the individual pins of the shift register to HIGH or LOW. Pin table is located in Pins.h

  if (ShiftRegisterBits[pin] != state) {

    ShiftRegisterBits[pin] = state;

    digitalWrite(STCP, LOW);
    for (int i = 23; i >= 0; i--) {
      digitalWrite(SHCP, LOW);
      digitalWrite(DS, ShiftRegisterBits[i]);
      digitalWrite(SHCP, HIGH);
    }

    digitalWrite(STCP, HIGH);
  }
}

void ShiftRegisterReset() {  //Sets all pins of the shift register to LOW
  digitalWrite(STCP, LOW);
  for (int i = 23; i >= 0; i--) {
    digitalWrite(SHCP, LOW);
    digitalWrite(DS, 0);
    digitalWrite(SHCP, HIGH);
  }

  digitalWrite(STCP, HIGH);
  // turn off all LEDs //
  for(int i = 9; i < 6 + 9; i++){
    ShiftRegisterWrite(i, HIGH);
  }
}

struct SensorData{
  uint16_t min=0;
  uint16_t max=0xFFFF;
  uint16_t value=0;
};
const uint8_t CALIBRATION = 255;

class LightSensor{
  private:
    uint8_t led_pin;
    void led_on(){
      ShiftRegisterWrite(this->led_pin, LOW); // TODO test and make private
    }
    void led_off(){
      ShiftRegisterWrite(this->led_pin, HIGH);
    }

  public:
    SensorData left_outer;
    SensorData left;
    SensorData center;
    SensorData right;
    SensorData right_outer;
    LightSensor(int led_pin){
      this->led_pin = led_pin;
    }
    void calibrate_turn(){
      led_on();
      uint16_t current_value = ADCRead(ADC_PT_L_1);
      if (current_value > this->left_outer.max){this->left_outer.max = current_value;}
      if (current_value < this->left_outer.min){this->left_outer.min = current_value;}
      current_value = ADCRead(ADC_PT_L_0);
      if (current_value > this->left.max){this->left.max = current_value;}
      if (current_value < this->left.min){this->left.min = current_value;}
      current_value = ADCRead(ADC_PT_M);
      if (current_value > this->center.max){this->center.max = current_value;}
      if (current_value < this->center.min){this->center.min = current_value;}
      current_value = ADCRead(ADC_PT_R_0);
      if (current_value > this->right.max){this->right.max = current_value;}
      if (current_value < this->right.min){this->right.min = current_value;}
      current_value = ADCRead(ADC_PT_R_1);
      if (current_value > this->right_outer.max){this->right_outer.max = current_value;}
      if (current_value < this->right_outer.min){this->right_outer.min = current_value;}
      led_off();
    }
    void read(){
      led_on();
      this->left_outer.value  = map(ADCRead(ADC_PT_L_1), left_outer.min,  left_outer.max, 0, 100);
      this->left.value        = map(ADCRead(ADC_PT_L_0), left.min,        left.max, 0, 100);
      this->center.value      = map(ADCRead(ADC_PT_M  ), center.min,      center.max, 0, 100);
      this->right.value       = map(ADCRead(ADC_PT_R_0), right.min,       right.max, 0, 100);
      this->right_outer.value = map(ADCRead(ADC_PT_R_1), right_outer.min, right_outer.max, 0, 100);
      led_off();
    }

};


bool HardwareInit(){
  /// get the shift register's Pins ///
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);

  ShiftRegisterReset(); /// set all values to LOW
  Wire.begin();

  bool mpu_is_inited = MPU6050Init();

  return mpu_is_inited;
}
LightSensor sensor1 = LightSensor(SR_PT_WHITE);
void setup(){
  HardwareInit();
  Serial.begin(9600);
}



void loop() {
  // put your main code here, to run repeatedly:
  sensor1.read();
  Serial.print(sensor1.left.value);
  Serial.print(sensor1.left.min);
  Serial.println(sensor1.left.max);
}

bool MPU6050Init() {  //Initalizes the Gyro Sensor (MPU6050)
  byte status = mpu.begin();
  if(status != 0){
    Serial.println(F("Fail to detect MPU6050!"));
    return false;
  }else{
    Serial.println(F("Found MPU6050"));
    Serial.println(F("While calculating the offsets value, do not move the MPU6050 sensor!"));
    delay(500);
    mpu.calcOffsets();
    Serial.println("Done!\n");
  }
  return true;
}

void UpdateMPU6050() {  //Reads out the values of the gyro sensor via I2C
  mpu.update();

  XAngle = mpu.getAngleX();
  YAngle = mpu.getAngleY();
  ZAngle = mpu.getAngleZ();
  XAccel = mpu.getAccAngleX();
}

void ResetZAngle() {  //The Z axis is reset
  CorrectionValueZAngle = ZAngle;
}

int ADCRead(int pin) {  //Reads out the inputs of the analog multiplexer
  ADCMULTI.channel(pin);
  delayMicroseconds(1);
  return analogRead(ADC_MULTI);
}
