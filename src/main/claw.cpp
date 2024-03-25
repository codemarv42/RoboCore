#include "claw.h"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "Pins.h"
#include "shiftregister.h"

// claw pins: grab - 16 lift - 18

namespace claw{
  Servo grab;
  Servo lift;
  Servo push;
  
  void setup(){
    grab.attach(16);
    lift.attach(19);
    push.attach(17);
  }
  void unload_victims(bool pushv=false){
    //shift_register::write(SR_SOLONOID, HIGH);
    delay(2000);
    //shift_register::write(SR_SOLONOID, LOW);
    push.write(0);
    delay(1000);
    push.write(180);
  }

  void down(){
    lift.write(155);
    delay(2000);
  }
  void up(){
    lift.write(0);
    delay(2000);
  }
  void half(){
    lift.write(80);
    delay(2000);
  }

  void close(){
    grab.write(0);
    delay(1000);
  }

  void open(){
    grab.write(90);
    delay(1000);
  }
  void openwide(){
    grab.write(180);
    delay(1000);
  }

  void openLittle(){
    grab.write(10);
    delay(500);
  }
}

