#include <Wire.h>
#include <numeric>
#include "Pins.h"
#include "shiftregister.h"
#include "motor.h"
#include "lightsensor.h"
#include "gyro.h"

LightSensorArray white(SR_PT_WHITE);
LightSensorArray red(SR_PT_RED);
LightSensorArray green(SR_PT_GREEN);
DirectSensor refL(SR_PT_WHITE, ADC_PT_REF_L);
DirectSensor refR(SR_PT_WHITE, ADC_PT_REF_R);
LightSensorPair back(SR_DE1, ADC_AE2, ADC_AE3);

LSBase* all_sensors[] = {&white,&green,&red,nullptr, &refL,&refR, &back};  // nullptr is placeholder for an (optional) blue LightSensorArray
LightSensorArray* ls[] = {&white,&green,&red,nullptr};
int16_t whiteHistory[10] = {0};
uint timestamp = 0;

void IRAM_ATTR isr() {
	ESP.restart();
}

void setup(){
  Serial.begin(115200);
  /// get the shift register's Pins ///
  Serial.println("Fetching shiftregister and ADC pins...");
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(ENC_SW, INPUT);
  Serial.println("Seting up button pins..");
  pinMode(T_L, INPUT);
  pinMode(T_R, INPUT);
  pinMode(T_M, INPUT);
  Serial.println("Fetching Tof servo pin...");

  Serial.println("Resetting shiftregister pins...");
  shift_register::reset(); /// set all values to LOW
  Wire.begin();
  shift_register::write(SR_LED_L_RED, LOW);
  timestamp = millis();
  attachInterrupt(ENC_SW, isr, RISING);
  calibrate(all_sensors, 5000, 0);
}
void loop(){
  static int historyIndex = 0;
  for (auto sensor:all_sensors){ // read light values
    if (sensor != nullptr){sensor->read();}
  }
  

  if (millis() > timestamp + 50){
    whiteHistory[historyIndex] = 100-white.left.value;
    timestamp = millis();
    historyIndex = (historyIndex + 1) % 10;
    Serial.println(std::accumulate(&whiteHistory[0], &whiteHistory[9], 0)/10);
  }


}
