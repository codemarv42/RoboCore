#include "gyro_sensor.h"
#include "robot.h"
#include "Wire.h"
#include "MPU6050_light.h"

MPU6050 mpu(Wire);

bool Gyro_sensor::init(){
    // Serial.println("Stand clear for MPU6050 calibration");
    delay(200);
    if (mpu.begin() == 1){
        // Serial.println("No Connection to MPU6050");
        RGB_led_L.red();
        return false;
    }
    else{
        delay(500); //unnötig eigentlich
        mpu.calcOffsets();
        mpu.update();
        t0 = micros();
        AngleX = 0.0;
        AngleZ = 0.0;
        // Serial.println("Completed MPU6050 calibration");
        RGB_led_L.green();
        return true;
    }
}

void Gyro_sensor::update(){
    mpu.update();
    float delta = micros()-t0;
    AngleX += (mpu.getGyroX() * delta);
    AngleZ += (mpu.getGyroZ() * delta);
    t0 = micros();
    return;
}
