#include "claw.h"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pins.h"

namespace claw{
  Servo grab;
  Servo lift;
  
  void setup(){
    grab.attach(16);
    lift.attach(18);
  }

  void down(){
    lift.write(20);
  }
  void up(){
    lift.write(180);
  }
  void half(){
    lift.write(90);
  }

  void close(){
    grab.write(0);
  }

  void open(){
    grab.write(180);
  }
}

