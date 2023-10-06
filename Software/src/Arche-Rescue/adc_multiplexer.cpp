#include "adc_multiplexer.h"
#include "my_pins.h"
#include "robot.h"


int ADCMultiplexerRead(int pin) {  //Reads out the inputs of the analog multiplexer
  ADC_multiplexer.channel(pin);
  delayMicroseconds(1);
  return analogRead(ADC_MULTI);
}
