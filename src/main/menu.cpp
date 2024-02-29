#include "Arduino.h"
#include "menu.h"

//
// MENU.CPP
// 
// this module contains high-level functions that do output on the Display
//


// No idea why arduino added this
#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"

#include "Pins.h" // pin layout

#include <SPI.h> // comms
#include <Wire.h>
#include <cmath>

#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306

#include "icons.h" // icons to be displayed
#include "ESP32_BLE.h" // BLE - here only used for the bluetooth symbol
#include "shiftregister.h"
#include "gyro.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SHOW_VBAT
#define VIEW_START 26 // We only have 128 pixels on the screen, so we only display from 26 to 154
#define VIEW_END 154

namespace menu {

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  void DisplayInit(){
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADRESS)) {
      Serial.println("SSD1306 allocation failed!");
    }
    display.display();
    display.clearDisplay();
    display.drawBitmap(0,0,logo_Bitmap, 128, 64, SSD1306_WHITE);
    display.display();
    delay(1000);
  }

  void overlay(){
    #ifdef BLE
      if (BLEStat == 2){
        display.drawBitmap(128-7,0, bluetooth, 7,9, SSD1306_WHITE);
      }
    #endif
    #ifdef SHOW_VBAT
        display.setCursor(0, 0);
        display.print(String((1649/649*analogRead(VBAT))/1000));
        display.print("V");
    #endif
  }

  void showWaiting(const char* msg){
    display.clearDisplay();
    overlay();
    display.setCursor(0,16);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println(msg);
    display.display();
  }

  
  #define menuOptions 3

  int menu(){
    int selected = 0;
    const unsigned char * icons[menuOptions] = {iconRun, iconCalibrate, placeholder}; // icons to display in menu
    const char * texts[menuOptions] = {"Run", "Calibrate", "test"}; // text of the options

    bool in_menu = true;
    bool last_RE_state = analogRead(ENC_B); // used for rotary encoder detection
    Serial.println("InMenu");
    while (in_menu){
      #ifdef BLE
        // BLELoop to not timeout while in menu
        BLELoop(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      #endif
      display.clearDisplay();
      overlay();
      display.setTextColor(SSD1306_WHITE);
      for(int i = 0; i < menuOptions; i++){ // draw icons
        if (i == selected){ // highlight selected
          display.fillRoundRect(32*i, 16, 32, 32, 4, SSD1306_WHITE);
          display.drawBitmap(32*i, 16, icons[i], 32, 32, SSD1306_INVERSE);
        }
        else{
          display.drawBitmap(32*i, 16, icons[i], 32, 32, SSD1306_INVERSE);
        }
      }
      display.setCursor(8, 49);
      display.print(texts[selected]);

      bool enc = analogRead(ENC_B); // rotary encoder turn detection
      if (enc != last_RE_state && enc == HIGH){
        if (analogRead(ENC_A) == LOW){
          selected--;
          if (selected < 0){selected = menuOptions-1;}
        }
        else{
          selected++;
          if (selected >= menuOptions){selected = 0;}
        }
      }
      last_RE_state = enc;

      display.display();
      if (digitalRead(ENC_SW) == HIGH){ // -> rotary clicked
        in_menu = false; // -> break while loop
        return selected;
      }
    }
    return selected;
  }


  void showDifference(int16_t value, bool clear=false){
    overlay();
    if (clear){
      display.clearDisplay();
      display.drawFastVLine(64, 32-8, 16, SSD1306_WHITE);
      //display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
    }
    int16_t xShift = value;
    display.drawFastVLine(xShift + 64, 32-8, 16, SSD1306_WHITE);
    //display.drawFastHLine(xShift, 64, 32, uint16_t color);
    display.display();
  }

  void showView(triangleData *t){
    display.clearDisplay();
    overlay();
    for(uint8_t i = 0; i < SCREEN_WIDTH; i++){
      Serial.print(t->upper[i]);
      Serial.print(" ");
      Serial.println(t->lower[i]);
      uint16_t dataMappedUpper = 32-(float(t->upper[i + VIEW_START])/1200*32); // map from 0mm - 1200mm to 0 - 32
      display.drawFastVLine(i, 32-dataMappedUpper, dataMappedUpper, SSD1306_WHITE);

      uint16_t dataMappedLower = 32-(float(t->lower[i + VIEW_START])/1200*32); // map from 0mm - 1200mm to 0 - 32
      display.drawFastVLine(i, 32, dataMappedLower, SSD1306_WHITE);

    }
    display.display();
  }

  void showRotation(){
    overlay();
    display.clearDisplay();
    display.drawCircle(64,32, 8, SSD1306_WHITE);
    display.drawFastVLine(64, 15, 17, SSD1306_WHITE);
    display.drawLine(64, 32, round(64 + 9 * cos(gyro::ZAngle)), round(32 + 9 * sin(gyro::ZAngle)), SSD1306_WHITE);

    display.display();
  }
}
