#include "Arduino.h"
#include "rgb_led.h"
#include "shift_register.h"


RGB_led::RGB_led(int pinR, int pinG, int pinB)
{
  this->pinR = pinR;
  this->pinG = pinG;
  this->pinB = pinB;
}

void RGB_led::red()
{
  ShiftRegisterWrite(pinR, 0,false);
  ShiftRegisterWrite(pinG, 1,false);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::green()
{
  ShiftRegisterWrite(pinR, 1,false);
  ShiftRegisterWrite(pinG, 0,false);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::blue()
{
  ShiftRegisterWrite(pinR, 1,false);
  ShiftRegisterWrite(pinG, 1,false);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::yellow()
{
  ShiftRegisterWrite(pinR, 0,false);
  ShiftRegisterWrite(pinG, 0,false);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::magenta()
{
  ShiftRegisterWrite(pinR, 0,false);
  ShiftRegisterWrite(pinG, 1,false);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::cyan()
{
  ShiftRegisterWrite(pinR, 1,false);
  ShiftRegisterWrite(pinG, 0,false);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::white()
{
  ShiftRegisterWrite(pinR, 0,false);
  ShiftRegisterWrite(pinG, 0,false);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_led::off()
{
  ShiftRegisterWrite(pinR, 1,false);
  ShiftRegisterWrite(pinG, 1,false);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_led::_test()
{
  int pause = 500;

  this->red();
  delay(pause);
  this->green();
  delay(pause);
  this->blue();
  delay(pause);

  this->yellow();
  delay(pause);
  this->magenta();
  delay(pause);
  this->cyan();
  delay(pause);

  this->white();
  delay(pause);
  this->off();
  delay(pause);
}
