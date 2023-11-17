#include <Arduino.h>

#include "button_sensor.h"


Button_sensor::Button_sensor(int pinButton) {
  this->pinButton = pinButton;
}

void Button_sensor::init() {
  pinMode(pinButton, INPUT);
}

bool Button_sensor::state() {
  stateButton = digitalRead(pinButton);
  return stateButton;
}
