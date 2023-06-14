#include "Pins.h"

#include <CD74HC4067.h>

CD74HC4067 ADCMULTI(S0, S1, S2, S3);

#define REF_L 0
#define L_1   1
#define L_0   2
#define M     3
#define R_0   4
#define R_1   5
#define REF_R 6

#define OUT_A 1
#define OUT_B 2
#define OUT_AB 3

#define FollowLineDiffFactor 4 //Integers only!

//Light measure//

unsigned int RAW_L[7] = { REF_L, L_1, L_0, M, R_0, R_1, REF_R};  //Raw light data 0 - 4096
unsigned int RAW_MIN[7];                            //Raw data 0 - 4096
unsigned int RAW_MAX[7];                            //Raw data 0 - 4096
unsigned int CAL_L[7];                              //Raw data 0 - 1000

//Shift register//

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

int ADCRead(int pin) {  //Reads out the inputs of the analog multiplexer
  ADCMULTI.channel(pin);
  delayMicroseconds(1);
  return analogRead(ADC_MULTI);
}

void MeasureRawLight() {  //Measure the raw light values from the ADC multiplexer

  ShiftRegisterWrite(SR_PT_WHITE, HIGH);

  unsigned int RAW_L_TMP[7];

  unsigned int RAW_L_TMP2[7];

  unsigned int RAW_L_TMP3[7];

  for (int i = 0; i < 7; i++) {
    RAW_L_TMP[i] = ADCRead(i);
    RAW_L_TMP2[i] = ADCRead(i);
    RAW_L_TMP3[i] = ADCRead(i);
    RAW_L[i] = (RAW_L_TMP[i] + RAW_L_TMP2[i] + RAW_L_TMP3[i]) / 3;
  }


  ShiftRegisterWrite(SR_PT_WHITE, LOW);
}

void Forward(byte mot, int v) {  //Turns a certain motor forward at a certain speed
  if (v < -255)
    v = -255;
  else if (v > 255)
    v = 255;
  ShiftRegisterWrite(SR_STBY, HIGH);
  if (v >= 0) {
    if (mot & OUT_A) {
      ShiftRegisterWrite(SR_AIN1, LOW);
      ShiftRegisterWrite(SR_AIN2, HIGH);
      analogWrite(PWMA, v);
    }
    if (mot & OUT_B) {
      ShiftRegisterWrite(SR_BIN1, LOW);
      ShiftRegisterWrite(SR_BIN2, HIGH);
      analogWrite(PWMB, v);
    }
  } else {
    v = -v;
    if (mot & OUT_A) {
      ShiftRegisterWrite(SR_AIN1, HIGH);
      ShiftRegisterWrite(SR_AIN2, LOW);
      analogWrite(PWMA, v);
    }
    if (mot & OUT_B) {
      ShiftRegisterWrite(SR_BIN1, HIGH);
      ShiftRegisterWrite(SR_BIN2, LOW);
      analogWrite(PWMB, v);
    }
  }
}

void Backward(byte mot, int v) {  //Turns a certain motor backward at a certain speed
  Forward(mot, -v);
}

void Stop() {  //Stops all motors
  ShiftRegisterWrite(SR_STBY, LOW);
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
}

void Left(int v) {  //The robot is rotated to the left at a certain speed
  Forward(OUT_B, v);
  Backward(OUT_A, v);
}

void Right(int v) {  //The robot is rotated to the right at a certain speed
  Forward(OUT_A, v);
  Backward(OUT_B, v);
}

void FollowLine(int v) {  //The robot follows the black line
  int diff = (RAW_L[R_0] - RAW_L[L_0]) * FollowLineDiffFactor;

  if (abs(diff) < 80) {
    Forward(OUT_AB, v - 20);
  } else {
    int vl;
    int vr;
    if (diff > 0) {
      vl = v + (diff);
      vr = v - (diff / 2);
    } else {
      vl = v + (diff / 2);
      vr = v - (diff);
    }

    if ((vl < 110) && (vl > 0)) {
      Forward(OUT_B, 100);
    } else if ((vl > -100) && (vl < 0)) {
      Forward(OUT_B, -100);
    } else {
      Forward(OUT_B, vl);
    }

    if ((vr < 100) && (vr > 0)) {
      Forward(OUT_A, 100);
    } else if ((vr > -100) && (vr < 0)) {
      Forward(OUT_A, -100);
    } else {
      Forward(OUT_A, vr);
    }
  }
}


void setup() {

  Serial.begin(115200);

  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(ADC_MULTI, INPUT);

  ShiftRegisterReset();
}

void loop() {

  MeasureRawLight();

  FollowLine(100);
}
