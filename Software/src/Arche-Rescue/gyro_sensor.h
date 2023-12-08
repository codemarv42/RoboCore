#ifndef GYRO_SENSOR_H
#define GYRO_SENSOR_H

#include "Arduino.h"
#include "Wire.h"
#include "MPU6050_light.h"

class Gyro_sensor{
    public:
        void init();
        void update();
        float AngleX;
        float AngleY;  // unnötig, abschalten
        float AngleZ;
        float AccelY;
        float RefZ = 0.0;
};

#endif