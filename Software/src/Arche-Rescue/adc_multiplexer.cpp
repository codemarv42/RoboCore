#include "robot.h"


int ADCRead(int pin) {  //Reads out the inputs of the analog multiplexer
  ADC_multiplexer.channel(pin);
  delayMicroseconds(1);
  return analogRead(ADC_MULTI);
}
