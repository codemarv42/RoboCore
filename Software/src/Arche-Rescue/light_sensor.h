#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class  Light_sensor{
  private:
    int pin;
    int upper_limit;
    int min;
    int max;
    int raw;
    int led;
  
  public:
    Light_sensor(int pin, int upper_limit, int led);
    int val;

    void init();
    int measure();
    void on();
    void off();
};

#endif