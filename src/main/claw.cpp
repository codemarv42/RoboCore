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
  Servo flap;
  
  void setup(){
    grab.attach(16);
    lift.attach(19);
    push.attach(17);
    flap.attach(SR_DE2);
  }
  void divide(int side){
    if (side == LEFT) {
        push.write(135);
    }
    else if(side == RIGHT){
        push.write(45);
    }
  }

  void closeFlap(){
    flap.write(90);
  }

  void unloadVictims(int side){
    if (side == LEFT){
      flap.write(5);
    }
    else if(side == RIGHT){
      flap.write(175);
    }
    else{
      Serial.println("WARNING: unloadVictims called with invalid side parameter - skipping");
    }
  }

  void down(){
    lift.write(140);
    delay(2000);
  }
  void up(){
    lift.write(5);
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

