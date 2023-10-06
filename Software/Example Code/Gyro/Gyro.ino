#include "Pins.h"

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
unsigned long timer = 0;

float XAngle;
float YAngle;
float ZAngle;
float CorrectionValueZAngle = 0.00;
float XAccel;

void MPU6050Init() {  //Initalizes the Gyro Sensor (MPU6050)
  byte status = mpu.begin();
  if(status != 0){
    Serial.println(F("Fail to detect MPU6050!"));
  }else{
    Serial.println(F("Found MPU6050"));
    Serial.println(F("While calculating the offsets value, do not move the MPU6050 sensor!"));
    delay(500);
    mpu.calcOffsets();
    Serial.println("Done!\n");
  }
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

void setup() {
  Serial.begin(115200);
  Wire.begin();

  MPU6050Init();
}

void loop() {
  UpdateMPU6050();

  if ((millis() - timer) > 10) { // print data every 10ms
    
    Serial.print("X : ");
    Serial.print(XAngle);
    Serial.print("90\xc2\xb0");
    
    Serial.print("\tY : ");
    Serial.print(YAngle);
    Serial.print("90\xc2\xb0");
    
    Serial.print("\tZ : ");
    Serial.print(ZAngle);
    Serial.println("90\xc2\xb0");
    
    timer = millis();
  }
}