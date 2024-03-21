#ifndef SHIFTREGISTER_CPP
#define SHIFTREGISTER_CPP

#include "shiftregister.cpp"

namespace shift_register{
  extern void reset();
  extern void write(int pin,bool state, bool pause_flush = false);
}
extern int ADCRead(int pin);
extern bool ADCDigitalRead(int pin);
#endif