#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include "CD74HC4067.h"


class Robot {
  private:
    bool running = false;
  
  public:
    void init();
    void run();
    void input();
    void output();
};

extern CD74HC4067 ADC_multiplexer;

#endif
