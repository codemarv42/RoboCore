#include <Adafruit_NeoPixel.h> //https://github.com/adafruit/Adafruit_NeoPixel
#include "pins.h"

// How many NeoPixels are attached?
#define NUMPIXELS 6

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 80 // Set BRIGHTNESS to about 1/5 (max = 255)

Adafruit_NeoPixel RGB(NUMPIXELS, RGB_LED, NEO_GRB + NEO_KHZ800);

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

#define NUM_AVG 3

unsigned int RAW_W[10] = {REF_L, L_1, L_0, R_0, R_1, REF_R, L_3, L_2, R_2, R_3};  //Raw light data 0 - 4096
unsigned int RAW_R[4] = {L_0, R_0, L_2, R_2};  //Raw light data 0 - 4096
unsigned int RAW_G[4] = {L_0, R_0, L_2, R_2};  //Raw light data 0 - 4096

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - Read Light Values");

  analogReadResolution(12);

  pinMode(WHITE_L, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(WHITE_R, OUTPUT);

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

  RGB.begin();           // INITIALIZE NeoPixel RGB object (REQUIRED)
  RGB.show();            // Turn OFF all pixels ASAP
  RGB.setBrightness(BRIGHTNESS);

  digitalWrite(WHITE_L, HIGH);
  delay(1000);
}

void loop() {

  //RGB.clear();
  //RGB.setPixelColor(i, RGB.Color(0, 150, 0));
  //RGB.show();
  
  //delay(500);

  MeasureRawLight();

  Serial.print("RAW_W: ");
  for(int i = 0; i <= 9; i++){
    Serial.print(RAW_W[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  Serial.print("RAW_R: ");
  for(int i = 0; i <= 3; i++){
    Serial.print(RAW_R[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  Serial.print("RAW_G: ");
  for(int i = 0; i <= 3; i++){
    Serial.print(RAW_G[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  delay(500);

}

void MeasureRawLight() {  //Measure the raw light values from the ADC multiplexer

  digitalWrite(WHITE_L, HIGH);

  delayMicroseconds(10);
  
  RAW_W[L_1] = analogRead(PT_L_1);

  RAW_W[L_0] = analogRead(PT_L_0);

  RAW_W[L_3] = analogRead(PT_L_3);
  
  RAW_W[L_2] = analogRead(PT_L_2);

  digitalWrite(WHITE_L, LOW);

  delayMicroseconds(10);

  digitalWrite(WHITE_R, HIGH);

  delayMicroseconds(10);

  RAW_W[R_1] = analogRead(PT_R_1);

  RAW_W[R_0] = analogRead(PT_R_0);

  RAW_W[R_3] = analogRead(PT_R_3);
  
  RAW_W[R_2] = analogRead(PT_R_2);

  digitalWrite(WHITE_R, LOW);

  delayMicroseconds(10);

  digitalWrite(WHITE, HIGH);

  delayMicroseconds(10);

  RAW_W[REF_L] = analogRead(PT_REF_L);

  RAW_W[REF_R] = analogRead(PT_REF_R);

  digitalWrite(WHITE, LOW);

  delayMicroseconds(10);

  RGB.setPixelColor(2, RGB.Color(255, 0, 0));
  RGB.setPixelColor(3, RGB.Color(255, 0, 0));
  RGB.setPixelColor(4, RGB.Color(255, 0, 0));
  RGB.setPixelColor(5, RGB.Color(255, 0, 0));
  RGB.show();

  delay(1);

  RAW_R[L_0] = analogRead(PT_L_0);

  RAW_R[R_0] = analogRead(PT_R_0);

  RAW_R[L_2] = analogRead(PT_L_2);

  RAW_R[R_2] = analogRead(PT_R_2);

  RGB.clear();

  delay(1);

  RGB.setPixelColor(2, RGB.Color(0, 255, 0));
  RGB.setPixelColor(3, RGB.Color(0, 255, 0));
  RGB.setPixelColor(4, RGB.Color(0, 255, 0));
  RGB.setPixelColor(5, RGB.Color(0, 255, 0));
  RGB.show();

  delay(1);

  RAW_G[L_0] = analogRead(PT_L_0);

  RAW_G[R_0] = analogRead(PT_R_0);

  RAW_G[L_2] = analogRead(PT_L_2);

  RAW_G[R_2] = analogRead(PT_R_2);

  RGB.setPixelColor(2, RGB.Color(0, 0, 0));
  RGB.setPixelColor(3, RGB.Color(0, 0, 0));
  RGB.setPixelColor(4, RGB.Color(0, 0, 0));
  RGB.setPixelColor(5, RGB.Color(0, 0, 0));
  RGB.show();

  delay(1);
}
