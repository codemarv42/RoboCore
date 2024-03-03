#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include "CD74HC4067.h"
#include "rgb_led.h"
#include "tof_sensor.h"
#include "drive_motor.h"
#include <vector>


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
    int schwarz_quer_rich = 0;

    unsigned long smooth_time = 0;
  
  public:
    bool running = false;
    bool secure = false;
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
    void halteKurs();
    std::vector<tof*> active_tof;
};

extern Robot robot;
extern CD74HC4067 ADC_multiplexer;
extern RGB_led RGB_led_L,RGB_led_R;
extern tof Tof_links;
extern tof Tof_rechts;
extern tof Tof_unten;
extern tof Tof_oben;
extern Motor Motor_L,Motor_R;

extern void sensorLoop(void* pvParameters);

#endif
