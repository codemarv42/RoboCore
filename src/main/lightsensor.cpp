#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
#include <sys/_stdint.h>
#include <sys/_types.h>
#include <sys/_intsup.h>
#ifndef LIGHTSENSOR_CPP
#define LIGHTSENSOR_CPP

#include "lightsensor.h"
#include "Pins.h"
#include "motor.h"

void calibrate(LightSensor* sensors[],const int amount = CALIBRATION,const int time_between_read_ms = 3)
{
  for(int i = 0; i < amount; i++) {
    for(int j = 0; j < 4; j++) {
      if (sensors[j] != nullptr) {
        sensors[j]->calibrate_turn();
        
      }
      //delay(time_between_read_ms);
    }
  }
}

void LightSensor::led_on(){
  shift_register::write(led_pin, HIGH);
  delayMicroseconds(80);
}

void LightSensor::led_off(){
  delayMicroseconds(20);
  shift_register::write(led_pin, LOW, true);
}
inline int16_t LightSensor::map(int16_t value, int16_t minv, int16_t maxv){
  return ((value - minv) * 100) / (maxv - minv);
}
LightSensor::LightSensor(int led_pin){
  this->led_pin = led_pin;
}

void LightSensor::calibrate_turn(){
  led_on();
  int16_t current_value = ADCRead(ADC_PT_L_1); // TODO: change part of the ifs to else ifs
  left_outer.max = max(left_outer.max, current_value);
  left_outer.min = min(left_outer.min, current_value);
  
  current_value = ADCRead(ADC_PT_L_0);
  left.max = max(left.max, current_value);
  left.min = min(left.min, current_value);
  
  current_value = ADCRead(ADC_PT_M);
  center.max = max(center.max, current_value);
  center.min = min(center.min, current_value);
  
  current_value = ADCRead(ADC_PT_R_0);
  right.max = max(right.max, current_value);
  right.min = min(right.min, current_value);
  
  current_value = ADCRead(ADC_PT_R_1);
  right_outer.max = max(right_outer.max, current_value);
  right_outer.min = min(right_outer.min, current_value);
  led_off();
}

void LightSensor::read(){
  led_on();
  left_outer.value  = map(ADCRead(ADC_PT_L_1), left_outer.min,  left_outer.max);
  left.value        = map(ADCRead(ADC_PT_L_0), left.min,        left.max);
  center.value      = map(ADCRead(ADC_PT_M  ), center.min,      center.max);
  right.value       = map(ADCRead(ADC_PT_R_0), right.min,       right.max);
  right_outer.value = map(ADCRead(ADC_PT_R_1), right_outer.min, right_outer.max);
  led_off();
}

DirectSensor::DirectSensor(uint8_t setLedPin, uint8_t setAdcPin){
  ledPin = setLedPin;
  adcPin = setAdcPin;
}

void DirectSensor::ledOn(){
  shift_register::write(ledPin, HIGH);
  delayMicroseconds(80);
}

void DirectSensor::ledOff(){
  delayMicroseconds(20);
  shift_register::write(ledPin, LOW, true);
}
void DirectSensor::read(){
  ledOn();
  data.value = ADCRead(adcPin);
  ledOff();
}




#endif