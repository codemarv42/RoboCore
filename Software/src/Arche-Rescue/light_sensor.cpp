#include "light_sensor.h"
#include "robot.h"
#include "shift_register.h"


Light_sensor::Light_sensor(int pin, int upper_limit){
    this->pin = pin;
    this->upper_limit = upper_limit;
}

void Light_sensor::init(){
    max = 0;
    min = upper_limit;
}

void Light_sensor::off(){
    ShiftRegisterWrite(pin, LOW);
}

void Light_sensor::on(){
    ShiftRegisterWrite(pin, HIGH);
}

int Light_sensor::measure(){
    raw = analogRead(pin);
    return raw;
}
