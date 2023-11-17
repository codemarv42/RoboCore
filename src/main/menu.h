#pragma once

extern void DisplayInit();
extern void DisplayMenu();
extern void showDifference(int16_t value, const char descr[1], bool clear=false);
extern bool in_menu;
extern int menu_selected;
extern void menu();
extern void showGyroWaiting();
