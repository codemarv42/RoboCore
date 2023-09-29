#ifndef BUTTON_SENSOR_H
#define BUTTON_SENSOR_H

#include <Arduino.h>


class Button {
    
    private:
        int pinButton;
        bool stateButton = LOW;

    public:
        Button(int pinButton);

        void init();
        bool state();
};

#endif
