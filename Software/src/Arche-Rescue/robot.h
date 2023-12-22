#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include "CD74HC4067.h"


class Robot {
  private:
    int his_wL1[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int his_wR1[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int his_gL1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int his_gR1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int his_rL1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int his_rR1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int his_sL1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int his_sR1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int gruen_rich = 0;
    int schwarz_quer_rich = 0;
    unsigned long schwarz_quer_time = 0;
  
  public:
    bool running = false;
    void init();
    void actionLoop();
    void input();
    void output();
    void kalibriere_LS(int anz);
    void messeLicht();
    void linienFolger();
    void gruenerPunkt();
    void pruefeQuerschwarz();
    void pruefeGruen();
    void pruefeRot();
    void pruefeSilber();
    void abbiegenGruen(int rich);
    void kehrtwende();
    void secureLoop();
    float battery_voltage();
};

extern Robot robot;

extern CD74HC4067 ADC_multiplexer;

extern void sensorLoop(void* pvParameters);

#endif
