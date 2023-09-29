#ifndef BUTTON_SENSOR_H
#define BUTTON_SENSOR_H

#include <Arduino.h>


class ButtonSensor {
    
    private:
        int pinButton;
        bool stateButton = LOW;

    public:
        ButtonSensor(int pinButton);

        void init();
        bool state();
};

#endif
