#include <Wire.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - I2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Online: 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      if (address == 64 || address == 112){
        Serial.println(" - PCA9685\n");
      }
      if (address == 104){
        Serial.println(" - MPU6050\n");
      }
      if (address == 60 || address == 61){
        Serial.println(" - OLED\n");
      }
      if (address == 41){
        Serial.println(" - VL53L1X\n");
      }
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("\n");
  }
  delay(5000);          
}