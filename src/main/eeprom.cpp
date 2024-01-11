#include <sys/_stdint.h>

#include "eeprom.h"
#include "Pins.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "lightsensor.h"


#define LS_BACKSHIFT 16
#define EEPROM_SIZE 50

namespace eeprom {
  void setup(){
    EEPROM.begin(EEPROM_SIZE);
  }

  void writeLS(int shift, LightSensor* ls){
    EEPROM.write(shift,                       ls->left_outer.min );
    EEPROM.write(shift + sizeof(int16_t),     ls->left_outer.max );
    EEPROM.write(shift + sizeof(int16_t) * 2, ls->left.min       );
    EEPROM.write(shift + sizeof(int16_t) * 3, ls->left.max       );
    EEPROM.write(shift + sizeof(int16_t) * 4, ls->center.min     );
    EEPROM.write(shift + sizeof(int16_t) * 5, ls->center.max     );
    EEPROM.write(shift + sizeof(int16_t) * 6, ls->right.min      );
    EEPROM.write(shift + sizeof(int16_t) * 7, ls->right.max      );
    EEPROM.write(shift + sizeof(int16_t) * 8, ls->right_outer.min);
    EEPROM.write(shift + sizeof(int16_t) * 9, ls->right_outer.max);
  }

  void writeLSData(LightSensor* white, LightSensor* green, LightSensor* red, LightSensor* blue){
    writeLS(LS_BACKSHIFT, white);
    writeLS(LS_BACKSHIFT + sizeof(int16_t) * 10, green);
    writeLS(LS_BACKSHIFT + sizeof(int16_t) * 20, red);
    if (blue != nullptr){writeLS(LS_BACKSHIFT + sizeof(int16_t) * 30, blue);}
    EEPROM.commit();
  }

  void loadLS(int shift, LightSensor* ls){
    EEPROM.get(shift,                       ls->left_outer.min );
    EEPROM.get(shift + sizeof(int16_t),     ls->left_outer.max );
    EEPROM.get(shift + sizeof(int16_t) * 2, ls->left.min       );
    EEPROM.get(shift + sizeof(int16_t) * 3, ls->left.max       );
    EEPROM.get(shift + sizeof(int16_t) * 4, ls->center.min     );
    EEPROM.get(shift + sizeof(int16_t) * 5, ls->center.max     );
    EEPROM.get(shift + sizeof(int16_t) * 6, ls->right.min      );
    EEPROM.get(shift + sizeof(int16_t) * 7, ls->right.max      );
    EEPROM.get(shift + sizeof(int16_t) * 8, ls->right_outer.min);
    EEPROM.get(shift + sizeof(int16_t) * 9, ls->right_outer.max);
  }

  void loadLSData(LightSensor* white, LightSensor* green, LightSensor* red, LightSensor* blue){
    loadLS(LS_BACKSHIFT, white);
    loadLS(LS_BACKSHIFT + sizeof(int16_t) * 10, green);
    loadLS(LS_BACKSHIFT + sizeof(int16_t) * 20, red);
    if (blue != nullptr){loadLS(LS_BACKSHIFT + sizeof(int16_t) * 30, blue);}
  }
}

