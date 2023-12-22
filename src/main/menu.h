#pragma once

#define MENU_RUN 0
#define MENU_CALIBRATE 1
#define MENU_INVOKE_EEPROM 2

namespace menu {
  extern void DisplayInit();
  extern void showDifference(int16_t value, const char descr[1], bool clear=false);
  extern bool in_menu;
  extern int menu_selected;
  extern int menu();
  extern void showWaiting(const char* msg);
}