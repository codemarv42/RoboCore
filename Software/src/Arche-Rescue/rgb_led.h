#ifndef RGB_LED
#define RGB_LED

class RGB_led {
    
  private:
    byte pinR;
    byte pinG;
    byte pinB;

  public:
    RGB_led(byte pinR, byte pinG, byte pinB);

    void red();
    void green();
    void blue();

    void yellow(); // red plus green
    void magenta(); // red plus blue
    void cyan(); // green plus blue

    void white(); 
}

#endif
