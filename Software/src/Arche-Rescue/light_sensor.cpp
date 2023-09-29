#include "light_sensor.h"
#include "robot.h"
#include "shift_register.h"


Light_sensor::LightSensor(int pin, int upper_limit){
    this->pin = pin;
    this->upper_limit = upper_limit;
}

Light_sensor::init(){
    max = 0;
    min = upper_limit;
}

Light_sensor::off(){
    ShiftRegisterWrite(pin, LOW);
}

Light_sensor::on(){
    ShiftRegisterWrite(pin, HIGH);
}

Light_sensor::measure(){
    raw = 0;
    return raw;
}
