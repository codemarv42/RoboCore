#include "claw.h"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pins.h"

// claw pins: grab - 16 lift - 18

namespace claw{
  Servo grab;
  Servo lift;
  
  void setup(){
    grab.attach(16);
    lift.attach(18);
  }

  void down(){
    lift.write(180);
    //lift.writeMicroseconds(2800);
    delay(2000);
  }
  void up(){
    lift.write(30);
    delay(2000);
  }
  void half(){
    lift.write(180);
    delay(2000);
  }

  void close(){
    grab.write(0);
    delay(1000);
  }

  void open(){
    grab.write(180);
    delay(1000);
  }
}

