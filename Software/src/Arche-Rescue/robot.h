#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include "CD74HC4067.h"


class Robot {
  private:
    bool running = false;
  
  public:
    void init();
    void actionLoop();
    void input();
    void output();
    void kalibriere_LS(int anz);
    void messeLicht();
};

extern Robot robot;

extern CD74HC4067 ADC_multiplexer;

extern void sensorLoop(void* pvParameters);

#endif
