#include <sys/_stdint.h>
#include <string>

#include <SPIFFS.h>
#include <FS.h>
#include "eeprom.h"
#include "Pins.h"
#include <Arduino.h>
#include "lightsensor.h"


#define LS_BACKSHIFT 16
#define EEPROM_SIZE 100

namespace eeprom {
  void setup(){
    Serial.println("mounting SPIFFS");
    if (!SPIFFS.begin(true)) {
      Serial.println("An Error has occurred while mounting SPIFFS");
    }
  }

  inline void writeMinMax(File& f, int16_t min, int16_t max){
    f.print(String(min)); f.print("\n");
    f.print(String(max)); f.print("\n");
  }

  void writeLS(File& f, LightSensorArray* l){
    writeMinMax(f, l->left_outer.min, l->left_outer.max);
    writeMinMax(f, l->left.min, l->left.max);
    writeMinMax(f, l->center.min, l->center.max);
    writeMinMax(f, l->right.min, l->right.max);
    writeMinMax(f, l->right_outer.min, l->right_outer.max);
  }

  void writeLSData(LightSensorArray* white, LightSensorArray* green, LightSensorArray* red, LightSensorArray* blue){
    File file = SPIFFS.open("/calibration.txt","w");
    if (!file){
      Serial.println("ERROR opening write file!");
      file.close();
      return;
    }
    writeLS(file, white);
    writeLS(file, green);
    writeLS(file, red);
    if (blue != nullptr){
      writeLS(file, blue);
    }
    file.close();
  }

  int16_t loadValue(File& f){
    String s = f.readStringUntil('\n');
    Serial.println(s.toInt());
    return s.toInt();
  }

  void loadLS(File& f, LightSensorArray* l){
    l->left_outer.min = loadValue(f);  l->left_outer.max = loadValue(f);
    l->left.min = loadValue(f);        l->left.max = loadValue(f);
    l->center.min = loadValue(f);      l->center.max = loadValue(f);
    l->right.min = loadValue(f);       l->right.max = loadValue(f);
    l->right_outer.min = loadValue(f); l->right_outer.max = loadValue(f);
  }

  void loadLSData(LightSensorArray* white, LightSensorArray* green, LightSensorArray* red, LightSensorArray* blue){
    Serial.println("Loading from file...");
    File f = SPIFFS.open("/calibration.txt","r");
    if (!f){
      Serial.println("ERROR opening read file!");
      f.close();
      return;
    }
    loadLS(f, white);
    loadLS(f, green);
    loadLS(f, red);
    if (blue != nullptr){
      loadLS(f, blue);
    }
    f.close();
  }
}

