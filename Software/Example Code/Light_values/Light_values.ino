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


  Serial.print("RAW_REF_L:");
  Serial.print(RAW_L[REF_L]);
  Serial.print(",");
  Serial.print("RAW_L_1:");
  Serial.print(RAW_L[L_1]);
  Serial.print(",");
  Serial.print("RAW_L_0:");
  Serial.print(RAW_L[L_0]);
  Serial.print(",");
  Serial.print("RAW_M:");
  Serial.print(RAW_L[M]);
  Serial.print(",");
  Serial.print("RAW_R_0:");
  Serial.print(RAW_L[R_0]);
  Serial.print(",");
  Serial.print("RAW_R_1:");
  Serial.print(RAW_L[R_1]);
  Serial.print(",");
  Serial.print("RAW_REF_R:");
  Serial.println(RAW_L[REF_R]);

  delay(100);
}
