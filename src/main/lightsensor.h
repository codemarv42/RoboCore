#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>
#include "Pins.h"
#include "shiftregister.h"



struct SensorData{
  uint16_t min=0xFFFF; // max value vor a 16-Bit integer in Hexadecimal
  uint16_t max=0;
  uint16_t value=0;
};


class LightSensor{
  private:
    uint8_t led_pin;
    void led_on();
    void led_off();
  public:
    SensorData left_outer;
    SensorData left;
    SensorData center;
    SensorData right;
    SensorData right_outer;
    LightSensor(int led_pin){
      this->led_pin = led_pin;
    }
    void calibrate_turn(){
      led_on();
      uint16_t current_value = ADCRead(ADC_PT_L_1); // TODO: change part of the ifs to else ifs
      this->left_outer.max = max(this->left_outer.max, current_value);
      this->left_outer.min = min(this->left_outer.min, current_value);
      
      current_value = ADCRead(ADC_PT_L_0);
      this->left.max = max(this->left.max, current_value);
      this->left.min = min(this->left.min, current_value);
      
      current_value = ADCRead(ADC_PT_M);
      this->center.max = max(this->center.max, current_value);
      this->center.min = min(this->center.min, current_value);
      
      current_value = ADCRead(ADC_PT_R_0);
      Serial.println(ADCRead(ADC_PT_R_0));
      this->right.max = max(this->right.max, current_value);
      this->right.min = min(this->right.min, current_value);
      
      current_value = ADCRead(ADC_PT_R_1);
      this->right_outer.max = max(this->right_outer.max, current_value);
      this->right_outer.min = min(this->right_outer.min, current_value);
      led_off();
    }
    void read(){
      led_on();
      this->left_outer.value  = map(ADCRead(ADC_PT_L_1), left_outer.min,  left_outer.max, 0, 100);
      this->left.value        = map(ADCRead(ADC_PT_L_0), left.min,        left.max, 0, 100);
      this->center.value      = map(ADCRead(ADC_PT_M  ), center.min,      center.max, 0, 100);
      this->right.value       = map(ADCRead(ADC_PT_R_0), right.min,       0xFFFF, 0, 100);
      this->right_outer.value = map(ADCRead(ADC_PT_R_1), right_outer.min, right_outer.max, 0, 100);
      led_off();
    }

};



#define CALIBRATION 1000
extern void calibrate(LightSensor* sensors[],const int amount,const int time_between_read_ms);


#endif