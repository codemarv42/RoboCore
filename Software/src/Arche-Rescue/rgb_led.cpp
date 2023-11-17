#include "rgb_led.h"
#include "shift_register.h" 


void RGB_led::RGB_led(byte pinR, byte pinG, byte pinG)
{
  this->pinR=pinR;
  this->pinG=pinG;
  this->pinG=pinG;
}

void RGB_led::red()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::green()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::blue()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::yellow()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::magenta()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::cyan()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::white()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::off()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 1);
}
