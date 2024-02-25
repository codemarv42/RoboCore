#include "Arduino.h"
#include "my_pins.h"


uint32_t ShiftRegisterBits = 0;

void ShiftRegisterWrite(int pin, bool state, bool push = true) {  //Sets the individual pins of the shift register to HIGH or LOW. Pin table is located in Pins.h
  uint32_t setbit = 1 << pin;
  if(state){
    ShiftRegisterBits |= setbit;
  }
  else{
    ShiftRegisterBits &= ~setbit;
  }
  if(!push){
    return; 
  } 

  digitalWrite(STCP, LOW);
  for (int i = 23; i >= 0; i--) {
    
    digitalWrite(SHCP, LOW);
    digitalWrite(DS, ShiftRegisterBits & (1<<i)? 1 : 0);
    digitalWrite(SHCP, HIGH);
  }

  digitalWrite(STCP, HIGH);
}

void ShiftRegisterReset() {  //Sets all pins of the shift register to LOW
  
  digitalWrite(STCP, LOW);
  
  for (int i = 23; i >= 0; i--) {
    
    digitalWrite(SHCP, LOW);
    digitalWrite(DS, 0);
    digitalWrite(SHCP, HIGH);
  }

  digitalWrite(STCP, HIGH);
}

void ShiftRegisterInit() {
  
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
}
