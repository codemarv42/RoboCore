#include "gyro_sensor.h"
#include "Wire.h"
#include "MPU6050_light.h"

Gyro_sensor :: Gyro_sensor(){
    return;
}

void Gyro_sensor :: init(){
    Serial.println("Stand clear for MPU6050 calibration");
    delay(200);
    if (mpu.begin() == 1){
        Serial.println("No Connection to MPU6050");
    }
    else{
        delay(500);
        mpu.calcOffsets();
        Serial.println("Completed MPU6050 calibration");
    }
    return;
}

void Gyro_sensor :: update(){
    mpu.update();

    AngleX = mpu.getAngleX();
    AngleY = mpu.getAngleY(); // unnötig
    AngleZ = mpu.getAngleZ() - RefZ;
    AccelY = mpu.getAccAngleY();

    return;
}
