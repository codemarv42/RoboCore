#include <sys/types.h>
#include <sys/_stdint.h>
#include <sys/_types.h>
#include "esp32-hal.h"
#include "sys_arch.h"
#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>
#include "Pins.h"
#include "shiftregister.h"



struct SensorData{
  int16_t min=0x7FFF; // max value vor a 16-Bit integer in Hexadecimal
  int16_t max=0;
  int16_t value=0;
};

class LightSensor{
  private:
    uint8_t led_pin;

    void led_on();
    void led_off();
    inline int16_t map(int16_t value, int16_t minv, int16_t maxv);

  public:
    SensorData left_outer;
    SensorData left;
    SensorData center;
    SensorData right;
    SensorData right_outer;

    LightSensor(int led_pin);
    void calibrate_turn();
    void read();
};

class DirectSensor{
  public:
    SensorData data;
    DirectSensor(uint8_t ledPin, uint8_t adcPin);
    void read();
  private:
    uint8_t ledPin;
    uint8_t adcPin;
    void ledOn();
    void ledOff();
};



#define CALIBRATION 1000
extern void calibrate(LightSensor* sensors[],const int amount,const int time_between_read_ms);


#endif