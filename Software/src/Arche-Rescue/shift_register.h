#ifndef SHIFT_REGISTER
#define SHIFT_REGISTER

extern void ShiftRegisterInit();
extern void ShiftRegisterReset();
extern void ShiftRegisterWrite(int pin, bool state);

#endif
