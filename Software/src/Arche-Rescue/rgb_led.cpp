#include "rgb_led"
#include "shift_register" 


void RGB_Led::RGB_Led(byte pinR, byte pinG, byte pinG)
{
  this->pinR=pinR;
  this->pinG=pinG;
  this->pinG=pinG;
}

void RGB_Led::red()
{
  ShiftRegisterWrite(pinR);
}

void RGB_Led::green()
{
  ShiftRegisterWrite(pinG);
}


void RGB_Led::RGB_Led(byte pinR, byte pinG, byte pinG)
{
  this->pinR=pinR;
  this->pinG=pinG;
  this->pinG=pinG;
}

void RGB_Led::red()
{
  ShiftRegisterWrite(pinR);
}

void RGB_Led::green()
{
  ShiftRegisterWrite(pinG);
}

void RGB_Led::blue()
{
  ShiftRegisterWrite(pinB);
}

void RGB_Led::yellow()
{
  ShiftRegisterWrite(pinR);
  ShiftRegisterWrite(pinG);
}

void RGB_Led::magenta()
{
  ShiftRegisterWrite(pinR);
  ShiftRegisterWrite(pinB);
}

void RGB_Led::cyan()
{
  ShiftRegisterWrite(pinG);
  ShiftRegisterWrite(pinB);
}

void RGB_Led::white()
{
  ShiftRegisterWrite(pinR);
  ShiftRegisterWrite(pinG);
  ShiftRegisterWrite(pinB);
}
