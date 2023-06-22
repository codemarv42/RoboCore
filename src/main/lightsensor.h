#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H
#include "lightsensor.cpp"

const int CALIBRATION = 1000;
void calibrate(LightSensor* sensors[4],const int amount = CALIBRATION,const int time_between_read_ms = 3){
  for(int i = 0; i < amount; i++){
    for(int j = 0; j < 4; j++){
      if (sensors[j] != nullptr){sensors[j]->calibrate_turn();}
      delay(time_between_read_ms);
    }
  }
}


#endif