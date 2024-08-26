#include "Pins.h"

bool ShiftRegisterBits[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void ShiftRegisterWrite(int pin, bool state) {  //Sets the individual pins of the shift register to HIGH or LOW. Pin table is located in Pins.h

  if (ShiftRegisterBits[pin] != state) {

    ShiftRegisterBits[pin] = state;

    digitalWrite(STCP, LOW);
    for (int i = 15; i >= 0; i--) {
      digitalWrite(SHCP, LOW);
      digitalWrite(DS, ShiftRegisterBits[i]);
      digitalWrite(SHCP, HIGH);
    }

    digitalWrite(STCP, HIGH);
  }
}

void ShiftRegisterReset() {  //Sets all pins of the shift register to LOW
  digitalWrite(STCP, LOW);
  for (int i = 15; i >= 0; i--) {
    digitalWrite(SHCP, LOW);
    digitalWrite(DS, 0);
    digitalWrite(SHCP, HIGH);
  }

  digitalWrite(STCP, HIGH);
}

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - Shiftregister Test");

  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
}

void loop() {
  ShiftRegisterReset();
  Serial.println("XSHT1: ON");
  ShiftRegisterWrite(SR_XSHT1, HIGH);
  delay(1000);
  Serial.println("XSHT2: ON");
  ShiftRegisterWrite(SR_XSHT1, LOW);
  ShiftRegisterWrite(SR_XSHT2, HIGH);
  delay(1000);
  Serial.println("XSHT3: ON");
  ShiftRegisterWrite(SR_XSHT2, LOW);
  ShiftRegisterWrite(SR_XSHT3, HIGH);
  delay(1000);
  Serial.println("XSHT4: ON");
  ShiftRegisterWrite(SR_XSHT3, LOW);
  ShiftRegisterWrite(SR_XSHT4, HIGH);
  delay(1000);
  Serial.println("All: OFF");
  ShiftRegisterWrite(SR_XSHT4, LOW);
  delay(1000);



}