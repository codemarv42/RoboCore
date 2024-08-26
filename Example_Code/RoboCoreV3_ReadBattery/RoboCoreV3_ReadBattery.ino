#include "pins.h"

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - Read Battery Voltage");

  pinMode(VBAT, INPUT);
  analogReadResolution(12);
}

void loop() {
  float BATVoltage = (float)analogRead(VBAT) / 4096 * 4.8 * 5.1 / 2.2;
  unsigned int BATPercent = (float)(BATVoltage - 7) * (100 - 0) / (8.4 - 7) + 0;

  if (BATPercent > 100) {
    BATPercent = 100;
    if (BATVoltage < 7.00){
      BATPercent = 0;
    }
  }

  Serial.print("Battery: ");
  Serial.print(BATVoltage);
  Serial.println("V");
  Serial.print("Battery: ");
  Serial.print(BATPercent);
  Serial.println("%");
  delay(500);
}
