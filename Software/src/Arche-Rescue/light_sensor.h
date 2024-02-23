#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class  Light_sensor{
  private:
    int pin;
    short int upper_limit;
    short int raw;
    int led;
  
  public:
    short int min;
    short int max;
    int val;
    Light_sensor(int pin, short int upper_limit, int led);

    void init();
    int measure_raw();
    int measure();
    void calibrate();
};

#endif