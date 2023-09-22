#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"


class Robot {
  private:
    bool running = false;
  
  public:
    void init();
    void run();
    void input();
    void output();
};


#endif