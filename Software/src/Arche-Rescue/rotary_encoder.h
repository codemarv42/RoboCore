#ifndef ROTARY_ENCODER
#define ROTARY_ENCODER

class Rotary_encoder {

  public:
    int counter = 0;

  private:
    int pin_a;
    int pin_b;
    int last_state_a;
    
    int pin_sw;
    int last_state_sw = 0;

  public:
    int measure();

};

#endif
