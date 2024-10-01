#include "Pins.h"

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - Button Test");

  pinMode(T_E, INPUT_PULLUP);
  pinMode(T_M, INPUT_PULLUP);
  pinMode(T_R, INPUT_PULLUP);
  pinMode(T_L, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(T_E) == 0) {
    Serial.println("Button: E (External)");
  }
  if (digitalRead(T_M) == 0) {
    Serial.println("Button: M (Metal/Center)");
  }
  if (digitalRead(T_R) == 0) {
    Serial.println("Button: R (Right)");
  }
  if (digitalRead(T_L) == 0) {
    Serial.println("Button: L (Left)");
  }
}