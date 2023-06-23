#include "Pins.h"

bool ShiftRegisterBits[24] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void ShiftRegisterWrite(int pin, bool state) {  //Sets the individual pins of the shift register to HIGH or LOW. Pin table is located in Pins.h

  if (ShiftRegisterBits[pin] != state) {

    ShiftRegisterBits[pin] = state;

    digitalWrite(STCP, LOW);
    for (int i = 23; i >= 0; i--) {
      digitalWrite(SHCP, LOW);
      digitalWrite(DS, ShiftRegisterBits[i]);
      digitalWrite(SHCP, HIGH);
    }

    digitalWrite(STCP, HIGH);
  }
}

void ShiftRegisterReset() {  //Sets all pins of the shift register to LOW
  digitalWrite(STCP, LOW);
  for (int i = 23; i >= 0; i--) {
    digitalWrite(SHCP, LOW);
    digitalWrite(DS, 0);
    digitalWrite(SHCP, HIGH);
  }

  digitalWrite(STCP, HIGH);
}

void setup() {

  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
}

void loop() {

  ShiftRegisterWrite(SR_LED_L_RED, HIGH);
  delay(500);
  ShiftRegisterWrite(SR_LED_L_GREEN, HIGH);
  delay(500);
  ShiftRegisterWrite(SR_LED_L_BLUE, HIGH);
  delay(500);
  ShiftRegisterWrite(SR_LED_R_RED, HIGH);
  delay(500);
  ShiftRegisterWrite(SR_LED_R_GREEN, HIGH);
  delay(500);
  ShiftRegisterWrite(SR_LED_R_BLUE, HIGH);
  delay(500);

  ShiftRegisterWrite(SR_LED_L_RED, LOW);
  delay(500);
  ShiftRegisterWrite(SR_LED_L_GREEN, LOW);
  delay(500);
  ShiftRegisterWrite(SR_LED_L_BLUE, LOW);
  delay(500);
  ShiftRegisterWrite(SR_LED_R_RED, LOW);
  delay(500);
  ShiftRegisterWrite(SR_LED_R_GREEN, LOW);
  delay(500);
  ShiftRegisterWrite(SR_LED_R_BLUE, LOW);
  delay(500);
}