#include <sys/_types.h>
#include <sys/_stdint.h>
#include "esp32-hal.h"
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
unsigned long timestamp;

bool MPU6050Init() {  //Initalizes the Gyro Sensor (MPU6050)
  byte status = mpu.begin();
  if(status != 0){
    Serial.println(F("Fail to detect MPU6050!"));
    return false;
  }
  else{
    delay(500);
    mpu.calcOffsets();
  }
  return true;
}

void UpdateMPU6050() {  //Reads out the values of the gyro sensor via I2C
  
  mpu.update();
  if (timestamp == 0){ // -> first measurement
    timestamp = micros();
    return;
  }
  unsigned long t = micros();
  unsigned long diff = t - timestamp;
  timestamp = t;
  
  XAngle += (mpu.getGyroX() * diff) / 1000000.0;
  YAngle += (mpu.getGyroY() * diff) / 1000000.0;
  ZAngle += (mpu.getGyroZ() * diff) / 1000000.0;
}

void ResetZAngle() {  //The Z axis is reset
  mpu.calcOffsets();
  timestamp = 0;
  XAngle = 0;
  YAngle = 0;
  ZAngle = 0;
}
}
