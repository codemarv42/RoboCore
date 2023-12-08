#include "Arduino.h"
#include "rotary_encoder.h"


void Rotary_encoder::measure()
{
  int state_a = digitalRead(this->pin_a);

  if (state_a != this->last_state_a && state_a == 1) {

    if (digitalRead(this->pin_b) != state_a) {

      if (this->counter == 0) {
        this->counter = 0;
      } else {
        this->counter--;
      }

    } else {
      this->counter++;
    }
  }

  this->last_state_a = state_a;

  bool state_sw = digitalRead(pin_sw);

  if ((state_sw == 1) && (this->last_state_sw != 1)) {
    this->last_state_sw = 1;
  }
}
