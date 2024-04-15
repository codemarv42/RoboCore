
#pragma once
#include "tof.h"

#include <Arduino.h>
#include "Pins.h"
#include "hardware.h"
#include "menu.h"

extern TaskHandle_t loop0; //used for handling the second main loop
extern void evacuationZone();
extern void navRoom(bool skipGap=false);


