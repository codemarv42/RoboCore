#ifndef SHIFTREGISTER_CPP
#define SHIFTREGISTER_CPP
#include <Arduino.h>
#include "Pins.h"
#include <CD74HC4067.h>

CD74HC4067 ADCMULTI(S0, S1, S2, S3);

namespace shift_register{
  bool shiftregister_bits[24] = {0};

  void write(int pin, bool state) {  //Sets the individual pins of the shift register to HIGH or LOW. Pin table is located in Pins.h

    if (shiftregister_bits[pin] != state) {

      shiftregister_bits[pin] = state;

      digitalWrite(STCP, LOW);
      for (int i = 23; i >= 0; i--) {
        digitalWrite(SHCP, LOW);
        digitalWrite(DS, shiftregister_bits[i]);
        digitalWrite(SHCP, HIGH);
      }

      digitalWrite(STCP, HIGH);
    }
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
  }
}
int ADCRead(int pin) {  //Reads out the inputs of the analog multiplexer
  ADCMULTI.channel(pin);
  delayMicroseconds(1);
  return analogRead(ADC_MULTI);
}



#endif


