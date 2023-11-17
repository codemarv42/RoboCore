#include "light_sensor.h"
#include "Arduino.h"
#include "robot.h"
#include "my_pins.h"
#include "shift_register.h"
#include "adc_multiplexer.h"


Light_sensor::Light_sensor(int pin, int upper_limit, int led){
  this->pin = pin;
  this->upper_limit = upper_limit;
  this->led = led;
}

void Light_sensor::init(){
  max = 0;
  min = upper_limit;
}

int Light_sensor::measure_raw(){
  ShiftRegisterWrite(led, 1);
  delayMicroseconds(PAUSE_MESSEN);
  raw = ADCMultiplexerRead(pin);
  ShiftRegisterWrite(led, 0);
  delayMicroseconds(PAUSE_MESSEN);
  return raw;
}

int Light_sensor::measure(){
  ShiftRegisterWrite(led, 1);
  delayMicroseconds(PAUSE_MESSEN);
  raw = ADCMultiplexerRead(pin);
  ShiftRegisterWrite(led, 0);
  delayMicroseconds(PAUSE_MESSEN);
  val = map(raw, min, max, 0, 100);
  return val;
}

void Light_sensor::calibrate(){
  measure_raw();
  if (raw < min){min = raw;}
  else if (raw > max){max = raw;};
  Serial.println(min);
  Serial.println(max);
  Serial.println("");
}
