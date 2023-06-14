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

unsigned int RAW_W[7] = { REF_L, L_1, L_0, M, R_0, R_1, REF_R };  //Raw light data 0 - 4096
unsigned int RAW_R[2] = { L_0, R_0 };  //Raw light data 0 - 4096
unsigned int RAW_G[2] = { L_0, R_0 };  //Raw light data 0 - 4096
unsigned int RAW_B[2] = { L_0, R_0 };  //Raw light data 0 - 4096
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

  unsigned int RAW_TMP1[7];
  unsigned int RAW_TMP2[7];
  unsigned int RAW_TMP3[7];

  ShiftRegisterWrite(SR_PT_WHITE, HIGH);  //White light


  for (int i = 0; i < 7; i++) {
    RAW_TMP1[i] = ADCRead(i);
    RAW_TMP2[i] = ADCRead(i);
    RAW_TMP3[i] = ADCRead(i);
    RAW_W[i] = (RAW_TMP1[i] + RAW_TMP2[i] + RAW_TMP3[i]) / 3;
  }


  ShiftRegisterWrite(SR_PT_WHITE, LOW);


  ShiftRegisterWrite(SR_PT_RED, HIGH);  //Red light

  RAW_TMP1[L_0] = ADCRead(L_0);
  RAW_TMP2[L_0] = ADCRead(L_0);
  RAW_TMP3[L_0] = ADCRead(L_0);
  RAW_R[L_0] = (RAW_TMP1[L_0] + RAW_TMP2[L_0] + RAW_TMP3[L_0]) / 3;

  RAW_TMP1[R_0] = ADCRead(R_0);
  RAW_TMP2[R_0] = ADCRead(R_0);
  RAW_TMP3[R_0] = ADCRead(R_0);
  RAW_R[R_0] = (RAW_TMP1[R_0] + RAW_TMP2[R_0] + RAW_TMP3[R_0]) / 3;


  ShiftRegisterWrite(SR_PT_RED, LOW);

  ShiftRegisterWrite(SR_PT_GREEN, HIGH);  //Green light

  RAW_TMP1[L_0] = ADCRead(L_0);
  RAW_TMP2[L_0] = ADCRead(L_0);
  RAW_TMP3[L_0] = ADCRead(L_0);
  RAW_G[L_0] = (RAW_TMP1[L_0] + RAW_TMP2[L_0] + RAW_TMP3[L_0]) / 3;

  RAW_TMP1[R_0] = ADCRead(R_0);
  RAW_TMP2[R_0] = ADCRead(R_0);
  RAW_TMP3[R_0] = ADCRead(R_0);
  RAW_G[R_0] = (RAW_TMP1[R_0] + RAW_TMP2[R_0] + RAW_TMP3[R_0]) / 3;


  ShiftRegisterWrite(SR_PT_GREEN, LOW);

  ShiftRegisterWrite(SR_PT_BLUE, HIGH);  //Blue light

  RAW_TMP1[L_0] = ADCRead(L_0);
  RAW_TMP2[L_0] = ADCRead(L_0);
  RAW_TMP3[L_0] = ADCRead(L_0);
  RAW_B[L_0] = (RAW_TMP1[L_0] + RAW_TMP2[L_0] + RAW_TMP3[L_0]) / 3;

  RAW_TMP1[R_0] = ADCRead(R_0);
  RAW_TMP2[R_0] = ADCRead(R_0);
  RAW_TMP3[R_0] = ADCRead(R_0);
  RAW_B[R_0] = (RAW_TMP1[R_0] + RAW_TMP2[R_0] + RAW_TMP3[R_0]) / 3;


  ShiftRegisterWrite(SR_PT_BLUE, LOW);

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

  Serial.print("RAW_W[REF_L]:");
  Serial.print(RAW_W[REF_L]);
  Serial.print(",");
  Serial.print("RAW_W[L_1]:");
  Serial.print(RAW_W[L_1]);
  Serial.print(",");
  Serial.print("RAW_W[L_0]:");
  Serial.print(RAW_W[L_0]);
  Serial.print(",");
  Serial.print("RAW_W[M]:");
  Serial.print(RAW_W[M]);
  Serial.print(",");
  Serial.print("RAW_W[R_0]:");
  Serial.print(RAW_W[R_0]);
  Serial.print(",");
  Serial.print("RAW_W[R_1]:");
  Serial.print(RAW_W[R_1]);
  Serial.print(",");
  Serial.print("RAW_W[REF_R]:");
  Serial.println(RAW_W[REF_R]);

  Serial.println("");

  Serial.print("RAW_R[L_0]:");
  Serial.print(RAW_R[L_0]);
  Serial.print(",");
  Serial.print("RAW_R[R_0]:");
  Serial.print(RAW_R[R_0]);
  Serial.print(",");
  Serial.print("RAW_G[L_0]:");
  Serial.print(RAW_G[L_0]);
  Serial.print(",");
  Serial.print("RAW_G[R_0]:");
  Serial.print(RAW_G[R_0]);
  Serial.print(",");
  Serial.print("RAW_B[L_0]:");
  Serial.print(RAW_B[L_0]);
  Serial.print(",");
  Serial.print("RAW_B[R_0]:");
  Serial.println(RAW_B[R_0]);

  Serial.println("");

  delay(100);
}
