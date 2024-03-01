#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H

#include "Arduino.h"
#include "VL53L1X.h"

class tof : public VL53L1X{
    public:
        int data;
        int xsht;
};
extern void TofInit();
extern void TofRead();

#endif