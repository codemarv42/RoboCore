#ifndef ROBOT_H
#define ROBOT_H


class Robot {
  private:
    bool running = false;
  
  public:
    int run();
    void init();
};


#endif