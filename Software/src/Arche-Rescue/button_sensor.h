#ifndef BUTTON_SENSOR_H
#define BUTTON_SENSOR_H

#include <Arduino.h>


class Button_sensor {
  
  private:
    int pinButton;
    bool stateButton = LOW;

  public:
    Button_sensor(int pinButton);

    void init();
    bool state();
};

#endif
