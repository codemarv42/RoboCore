#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class  Light_sensor{
  private:
    int pin;
    int min;
    int max;
    int raw;
    int led;
  
  public:
    Light_sensor(int pin, int upper_limit, int led);
    int upper_limit;

    void init();
    int measure();
    void on();
    void off();
};

#endif