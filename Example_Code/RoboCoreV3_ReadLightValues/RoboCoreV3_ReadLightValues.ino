#include "pins.h"

#define REF_L 0
#define L_1   1
#define L_0   2
#define R_0   3
#define R_1   4
#define REF_R 5

#define L_3   6
#define L_2   7
#define R_2   8
#define R_3   9

unsigned int RAW_W[10] = {REF_L, L_1, L_0, R_0, R_1, REF_R, L_3, L_2, R_2, R_3};  //Raw light data 0 - 511
unsigned int RAW_R[4] = {L_0, R_0, L_2, R_2};  //Raw light data 0 - 511
unsigned int RAW_G[4] = {L_0, R_0, L_2, R_2};  //Raw light data 0 - 511

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - Read Light Values");

  analogReadResolution(9); //12 - default, 9 - faster ADC

  pinMode(WHITE_L, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(WHITE_R, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(PT_REF_L, INPUT);
  pinMode(PT_L_1, INPUT);
  pinMode(PT_L_0, INPUT);
  pinMode(PT_R_0, INPUT);
  pinMode(PT_R_1, INPUT);
  pinMode(PT_REF_R, INPUT);

  pinMode(PT_L_3, INPUT);
  pinMode(PT_L_2, INPUT);
  pinMode(PT_R_2, INPUT);
  pinMode(PT_R_3, INPUT);

  digitalWrite(WHITE_L, LOW);
  digitalWrite(WHITE_R, LOW);
  digitalWrite(WHITE, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  delay(500);
}

void loop() {

  MeasureRawLight();

  Serial.print("RAW_W: ");
  for(int i = 0; i <= 9; i++){
    Serial.print(RAW_W[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  delay(500);
}

void MeasureRawLight() {  //Measure the raw light values from the ADC multiplexer

  digitalWrite(WHITE_L, HIGH);

  delay(100);
  
  RAW_W[L_1] = analogRead(PT_L_1);

  RAW_W[L_0] = analogRead(PT_L_0);

  RAW_W[L_3] = analogRead(PT_L_3);
  
  RAW_W[L_2] = analogRead(PT_L_2);

  digitalWrite(WHITE_L, LOW);

  digitalWrite(WHITE_R, HIGH);

  delay(100);

  RAW_W[R_1] = analogRead(PT_R_1);

  RAW_W[R_0] = analogRead(PT_R_0);

  RAW_W[R_3] = analogRead(PT_R_3);
  
  RAW_W[R_2] = analogRead(PT_R_2);

  digitalWrite(WHITE_R, LOW);

  digitalWrite(WHITE, HIGH);

  delay(100);

  RAW_W[REF_L] = analogRead(PT_REF_L);

  RAW_W[REF_R] = analogRead(PT_REF_R);

  digitalWrite(WHITE, LOW);

  delay(100);
}
