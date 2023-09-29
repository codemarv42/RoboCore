#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class  {
  private:
    bool running = false;
  
  public:
    void init();
    void run();
    void input();
    void output();
};

#endif