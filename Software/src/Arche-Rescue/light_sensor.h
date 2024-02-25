#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class  Light_sensor{
  private:
    int pin;
    unsigned short int upper_limit;
    unsigned short int raw;
    int led;
  
  public:
    unsigned short int min;
    unsigned short int max;
    int val;
    Light_sensor(int pin,unsigned short int upper_limit, int led);

    void init();
    int measure_raw();
    int measure();
    void calibrate();
};

#endif