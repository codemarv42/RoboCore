#ifndef GYRO_SENSOR_H
#define GYRO_SENSOR_H

#include "Arduino.h"
#include "Wire.h"
#include "MPU6050_light.h"

class Gyro_sensor{
    public:
        bool init();
        void update();
        unsigned long t0;
        float AngleX;
        float AngleZ;
        // float AccelY;    // eher nicht nutzen
        float RefZ = 0.0;
};

extern MPU6050 mpu; //löschen

#endif