#include "Pins.h"

#define OUT_A 1
#define OUT_B 2
#define OUT_AB 3

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

void Forward(byte mot, int v) {  //Turns a certain motor forward at a certain speed
  if (v < -255)
    v = -255;
  else if (v > 255)
    v = 255;
  ShiftRegisterWrite(SR_STBY1, HIGH);
  ShiftRegisterWrite(SR_STBY2, HIGH);
  if (v >= 0) {
    if (mot & OUT_A) {
      ShiftRegisterWrite(SR_AIN1_1, LOW);
      ShiftRegisterWrite(SR_AIN2_1, HIGH);
      analogWrite(PWMA1, v);

      ShiftRegisterWrite(SR_AIN1_2, LOW);
      ShiftRegisterWrite(SR_AIN2_2, HIGH);
      analogWrite(PWMA2, v);
    }
    if (mot & OUT_B) {
      ShiftRegisterWrite(SR_BIN1_1, LOW);
      ShiftRegisterWrite(SR_BIN2_1, HIGH);
      analogWrite(PWMB1, v);

      ShiftRegisterWrite(SR_BIN1_2, LOW);
      ShiftRegisterWrite(SR_BIN2_2, HIGH);
      analogWrite(PWMB2, v);
    }
  } else {
    v = -v;
    if (mot & OUT_A) {
      ShiftRegisterWrite(SR_AIN1_1, HIGH);
      ShiftRegisterWrite(SR_AIN2_1, LOW);
      analogWrite(PWMA1, v);

      ShiftRegisterWrite(SR_AIN1_2, HIGH);
      ShiftRegisterWrite(SR_AIN2_2, LOW);
      analogWrite(PWMA2, v);
    }
    if (mot & OUT_B) {
      ShiftRegisterWrite(SR_BIN1_1, HIGH);
      ShiftRegisterWrite(SR_BIN2_1, LOW);
      analogWrite(PWMB1, v);

      ShiftRegisterWrite(SR_BIN1_2, HIGH);
      ShiftRegisterWrite(SR_BIN2_2, LOW);
      analogWrite(PWMB2, v);
    }
  }
}

void Backward(byte mot, int v) {  //Turns a certain motor backward at a certain speed
  Forward(mot, -v);
}

void Stop() {  //Stops all motors
  ShiftRegisterWrite(SR_STBY1, LOW);
  ShiftRegisterWrite(SR_STBY2, LOW);

  ShiftRegisterWrite(SR_AIN1_1, LOW);
  ShiftRegisterWrite(SR_AIN2_1, LOW);
  ShiftRegisterWrite(SR_BIN1_1, LOW);
  ShiftRegisterWrite(SR_BIN2_1, LOW);

  ShiftRegisterWrite(SR_AIN1_2, LOW);
  ShiftRegisterWrite(SR_AIN2_2, LOW);
  ShiftRegisterWrite(SR_BIN1_2, LOW);
  ShiftRegisterWrite(SR_BIN2_2, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - Motor Test");

  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);

  pinMode(PWMA1, OUTPUT);
  pinMode(PWMA2, OUTPUT);
  pinMode(PWMB1, OUTPUT);
  pinMode(PWMB2, OUTPUT);
}

void loop() {
  Serial.println("Forward");
  Forward(OUT_AB, 150);
  delay(1500);
  Serial.println("Backward");
  Backward(OUT_AB, 150);
  delay(1500);
  Serial.println("Stop");
  Stop();
  delay(1500);
}