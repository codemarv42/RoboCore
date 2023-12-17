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
  //motor::fwd(AB,1);
  for(int i = 0; i < amount; i++) {
    for(int j = 0; j < 4; j++) {
      if (sensors[j] != nullptr) {
        sensors[j]->calibrate_turn();
        
      }
      //delay(time_between_read_ms);
    }
  }
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