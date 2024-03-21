#ifndef GYRO_H
#define GYRO_H
#include <Arduino.h>
#include "Pins.h"
#include "MPU6050_light.h"
#include "Wire.h"

extern MPU6050 mpu;

namespace gyro{
  extern bool MPU6050Init();
  extern void ResetZAngle();
  extern void UpdateMPU6050();

  extern float XAngle;
  extern float YAngle;
  extern float ZAngle;

  extern float XAccel;
}
#endif