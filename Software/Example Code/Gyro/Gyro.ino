#include "Pins.h"

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
unsigned long timer = 0;

//Notice! For smaller deviation of the 
//Y-axis calculation the getAngleZ function must be executed every 10ms!

void setup() {
  Serial.begin(9600);
  Wire.begin();

  byte status = mpu.begin(MPU6050_ADRESS);
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { }

  Serial.println(F("While calculating the offsets value, do not move the MPU6050 sensor"));
  delay(1000);
  // mpu.upsideDownMounting = true; // Comment on this line if MPU6050 is installed backwards
  mpu.calcOffsets();
  Serial.println("Done!\n");
}

void loop() {
  mpu.update();

  if ((millis() - timer) > 10) { // print data every 10ms
    
    Serial.print("X : ");
    Serial.print(mpu.getAngleX());
    Serial.print("90\xc2\xb0");
    
    Serial.print("\tY : ");
    Serial.print(mpu.getAngleY());
    Serial.print("90\xc2\xb0");
    
    Serial.print("\tZ : ");
    Serial.print(mpu.getAngleZ());
    Serial.println("90\xc2\xb0");
    
    timer = millis();
  }
}