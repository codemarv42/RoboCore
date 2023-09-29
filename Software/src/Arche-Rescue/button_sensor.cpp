#include <Arduino.h>

#include "button_sensor.h"


ButtonSensor::ButtonSensor(int pinButton) {
    this->pinButton = pinButton;
}

void ButtonSensor::init() {
    pinMode(pinButton, INPUT);
}

bool ButtonSensor::state() {
    stateButton = digitalRead(pinButton);
    return stateButton;
}
