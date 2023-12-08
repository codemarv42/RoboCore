#ifndef GYRO_SENSOR_H
#define GYRO_SENSOR_H

#include "Arduino.h"

class Gyro_sensor{
    private:
        MPU6050 mpu(Wire);
    public:
        void init();
        void update();
        float AngleX;
        float AngleY;  // unnötig, abschalten
        float AngleZ;
        float AccelY;
        float RefZ = 0.0;
}

#endif