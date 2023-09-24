#include <Wire.h>

#define I2C_Freq 100000
#define SDA_0 21
#define SCL_0 22

TwoWire I2C_0 = TwoWire(0);

void setup() {
  Serial.begin(115200);
  I2C_0.begin(SDA_0, SCL_0, I2C_Freq);
}

void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    I2C_0.beginTransmission(address);
    error = I2C_0.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000);  // wait 5 seconds for next scan
}