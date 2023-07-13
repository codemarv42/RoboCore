#ifndef LIGHTSENSOR_CPP
#define LIGHTSENSOR_CPP

#include "lightsensor.h"
#include "Pins.h"

void calibrate(LightSensor* sensors[],const int amount = CALIBRATION,const int time_between_read_ms = 3)
{
  for(int i = 0; i < amount; i++) {
    for(int j = 0; j < 4; j++) {
      if (sensors[j] != NULL) {
        sensors[j]->calibrate_turn();
        
      }
      delay(time_between_read_ms);
    }
  }
}





#endif