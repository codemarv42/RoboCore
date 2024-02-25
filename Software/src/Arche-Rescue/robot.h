#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include "CD74HC4067.h"
#include "rgb_led.h"


class Robot {
  private:
    int his_wL1[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int his_wR1[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int count_green_l = 0;
    int count_green_r = 0;
    int count_red_l = 0;
    int count_red_r = 0;
    int count_silver_l = 0;
    int count_silver_r = 0;
    int count_schwarz_l = 0;
    int count_schwarz_r = 0;
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
    void pruefeRot();
    void pruefeSilber();
    void abbiegenGruen(int rich);
    void kehrtwende();
    void pruefeHindernis();
    void secureLoop();
    float battery_voltage();
    void leseKalWerte();
    void schreibeKalWerte();
};

extern Robot robot;
extern CD74HC4067 ADC_multiplexer;
extern RGB_led RGB_led_L,RGB_led_R;

extern void sensorLoop(void* pvParameters);

#endif
