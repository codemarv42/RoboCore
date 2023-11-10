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
  ShiftRegisterWrite(pinR, 0);
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
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_Led::green()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_Led::blue()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_Led::yellow()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 1);
}

void RGB_Led::magenta()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_Led::cyan()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_Led::white()
{
  ShiftRegisterWrite(pinR, 0);
  ShiftRegisterWrite(pinG, 0);
  ShiftRegisterWrite(pinB, 0);
}

void RGB_Led::off()
{
  ShiftRegisterWrite(pinR, 1);
  ShiftRegisterWrite(pinG, 1);
  ShiftRegisterWrite(pinB, 1);
}
