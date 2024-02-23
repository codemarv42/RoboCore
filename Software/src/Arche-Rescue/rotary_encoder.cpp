#include "Arduino.h"
#include "rotary_encoder.h"


void Rotary_encoder::measure()
{
  int state_a = digitalRead(this->pin_a);
  int state_b = digitalRead(this->pin_b);

  if (state_a != this->last_state_a && state_a == 1) {

    if (state_b != state_a) {

      this->counter++;  // im Uhrzeigersinn

    } 
    else {
      this->counter--;  // gegen Uhrzeigersinn
    }
  }

  this->last_state_a = state_a;

  bool state_sw = digitalRead(pin_sw);

}
