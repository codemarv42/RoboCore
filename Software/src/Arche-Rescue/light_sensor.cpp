#include "light_sensor.h"
#include "robot.h"
#include "my_pins.h"
#include "shift_register.h"
#include "adc_multiplexer.h"


Light_sensor::Light_sensor(int pin, int upper_limit, int led){
    this->pin = pin;
    this->upper_limit = upper_limit;
    this->led = led;
}

void Light_sensor::init(){
    max = 0;
    min = upper_limit;
}

int Light_sensor::measure(){
    ShiftRegisterWrite(pin, 1);
    delayMicroseconds(PAUSE_MESSEN);
    raw = ADCMultiplexerRead(pin);
    ShiftRegisterWrite(pin, 0);
    return raw;
}
