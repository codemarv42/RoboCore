#ifndef RGB_LED
#define RGB_LED

class RGB_led {
    
  private:
    int pinR;
    int pinG;
    int pinB;

  public:
    RGB_led(int pinR, int pinG, int pinB);

    void red();
    void green();
    void blue();

    void yellow(); // red plus green
    void magenta(); // red plus blue
    void cyan(); // green plus blue

    void white(); 

    void off();

    void _test();
};

#endif
