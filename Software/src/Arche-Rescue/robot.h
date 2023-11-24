#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include "CD74HC4067.h"


class Robot {
  private:
    bool running = false;
    int his_wL1[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int his_wR1[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int schwarz_quer_rich = 0;
    unsigned long schwarz_quer_time = 0;
  
  public:
    void init();
    void actionLoop();
    void input();
    void output();
    void kalibriere_LS(int anz);
    void messeLicht();
    void linienFolger();
    void gruenerPunkt();
    void pruefeQuerschwarz();
    int pruefeGruen();
    void abbiegenGruen();
    void kehrtwende();
};

extern Robot robot;

extern CD74HC4067 ADC_multiplexer;

extern void sensorLoop(void* pvParameters);

#endif
