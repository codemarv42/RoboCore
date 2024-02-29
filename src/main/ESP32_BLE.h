#ifndef ESP32_BLE_H
#define ESP32_BLE_H
#include <Arduino.h>

extern unsigned int BLEStat;

extern void BLELoop(int, int, int, int, int, int, int, int, int, int, int);

extern void StopBLE();
extern void StartBLE();
extern void post(String msg);

#endif
