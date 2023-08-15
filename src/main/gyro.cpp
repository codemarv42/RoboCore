#include <Arduino.h>
#include "Pins.h"
#include "gyro.h"
#include "MPU6050_light.h"
#include "Wire.h"

MPU6050 mpu(Wire);


namespace gyro {
unsigned long timer = 0;

float XAngle;
float YAngle;
float ZAngle;
float CorrectionValueZAngle = 0.00;
float XAccel;

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
    Serial.println("...Done!\n");
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


}
