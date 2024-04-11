#ifndef SHIFTREGISTER_CPP
#define SHIFTREGISTER_CPP
#include <Arduino.h>
#include "Pins.h"
#include <CD74HC4067.h>

CD74HC4067 ADCMULTI(S0, S1, S2, S3);


namespace shift_register{
  static uint32_t shift_register_bits = 0;
  
  void write(int pin, bool state, bool pause_flush = false) {  //Sets the individual pins of the shift register to HIGH or LOW. Pin table is located in Pins.h
    
    uint32_t setbit = 1 << pin;
    if (state){
      shift_register_bits |= setbit;
    }
    else{
      shift_register_bits &= ~setbit;
    }
    if(pause_flush){
      return;
    } 
    digitalWrite(STCP, LOW);
    for (int i = 23; i >= 0; i--) {
      digitalWrite(SHCP, LOW);
      digitalWrite(DS, shift_register_bits & (1<<i)? 1 : 0);
      digitalWrite(SHCP, HIGH);
    }
    digitalWrite(STCP, HIGH);
  }
  

  void reset() {  //Sets all pins of the shift register to LOW
    digitalWrite(STCP, LOW);
    for (int i = 23; i >= 0; i--) {
      digitalWrite(SHCP, LOW);
      digitalWrite(DS, 0);
      digitalWrite(SHCP, HIGH);
    }

    digitalWrite(STCP, HIGH);
    // turn off all LEDs //
    for(int i = 9; i < 6 + 9; i++){
      write(i, HIGH);
    }
    write(SR_DE1, HIGH);
  }
}
int ADCRead(int pin) {  //Reads out the inputs of the analog multiplexer
  ADCMULTI.channel(pin);
  delayMicroseconds(10);
  return analogRead(ADC_MULTI);
}
bool ADCDigitalRead(int pin){
  ADCMULTI.channel(pin);
  delayMicroseconds(10);
  return analogRead(ADC_MULTI) > 0;
}


#endif


