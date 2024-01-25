#include <sys/_stdint.h>
#include "tof.h"

#include <Arduino.h>
#include "Pins.h"
#include "hardware.h"
#include "evac.h"
#include "menu.h"
#include <utility>

#define VICTIM1 1 // dead victim
#define VICTIM2 2
#define VICTIM3 4
#define V 100

#define VICTIM_DIFF 100

std::pair<uint8_t, bool> getOptimalVictimAngle(triangleData* t){
  for(static uint8_t i = 0; i < 180; i++){
    if (t->upper[i] - t->lower[i] >= VICTIM_DIFF && i > 15 && i < 165){
      return std::make_pair<uint8_t, bool>(i+1, false);
    }
  }
  
  return std::make_pair<uint8_t, bool>(0, true);
}

void evacuationZone(){
  uint8_t transporting = 0; // which victims are transported
  motor::fwd(AB, V);
  delay(1000);
  motor::stop();

  triangleData* t;
  menu::showWaiting("Reading tData");
  while (transporting != (VICTIM1 | VICTIM2 | VICTIM3)){
    t = tof::readPos();
    menu::showView(t);
    std::pair<uint8_t, bool> angle = getOptimalVictimAngle(t);
    if (!angle.second){
      motor::gyro(V, angle.first - 90);
    }

    delete t;
  }

}


