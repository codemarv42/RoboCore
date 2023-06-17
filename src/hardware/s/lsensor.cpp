#include "Pins.h"
#include "Shift_register.cpp"
#include <Arduino.h>

class LightSensor {
  public:
    LightSensor(int led_pin){
      this->led_pin = led_pin;
    }
    void turn_on(){
      ShiftRegisterWrite(this->led_pin, HIGH);
    }
    void turn_off(){
      ShiftRegisterWrite(this->led_pin, LOW);
    }



  private:
    int led_pin;

};

