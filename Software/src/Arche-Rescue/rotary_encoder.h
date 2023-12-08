#ifndef ROTARY_ENCODER
#define ROTARY_ENCODER
#include "Arduino.h"
#include "my_pins.h"

class Rotary_encoder {

  public:
    int counter = 0;

  private:
    int pin_a = ENC_A;
    int pin_b = ENC_B;
    int last_state_a;
    
    int pin_sw = ENC_SW;
    int last_state_sw = 0;

  public:
    void measure();

};

#endif
