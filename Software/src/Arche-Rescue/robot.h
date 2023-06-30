#ifndef ROBOT_H
#define ROBOT_H


class Robot {
  private:
    bool running = false;
  
  public:
    void run();
    void init();
    void input();
    void output();
};


#endif