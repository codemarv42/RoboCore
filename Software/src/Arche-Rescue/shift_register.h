#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

extern void ShiftRegisterInit();
extern void ShiftRegisterReset();
extern void ShiftRegisterWrite(int pin, bool state, bool push = true);

#endif
