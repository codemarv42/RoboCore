#include "Arduino.h"
#include "button_sensor.h"

Button::Button(int pinButton) {
    this->pinButton = pinButton;
}
void Button::init() {
    pinMode(pinButton, INPUT);
}
bool Button::state() {
    stateButton = digitalRead(pinButton);
    return stateButton;
}