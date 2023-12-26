#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"
#include "Pins.h"

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306

#include "icons.h"
#include "ESP32_BLE.h"
#include "shiftregister.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SHOW_VBAT

namespace menu {

  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  void DisplayInit(){
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADRESS)) {
      Serial.println(F("SSD1306 allocation failed!"));
    }
    display.display();
    display.clearDisplay();
    display.drawBitmap(0,0,logo_Bitmap, 128, 64, SSD1306_WHITE);
    display.display();
    delay(1000);
  }

  void overlay(){
    if (BLEStat == 2){
      display.drawBitmap(128-7,0, bluetooth, 7,9, SSD1306_WHITE);
    }
    #ifdef SHOW_VBAT
        display.setCursor(0, 0);
        //display.print(F(analogRead(VBAT)));
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

  
  #define menuOptions 2
  bool in_menu = false;
  int menu_selected = 0;

  int menu(){
    int selected = 0;
    const unsigned char * icons[menuOptions] = {iconRun, iconCalibrate};
    const char * texts[menuOptions] = {"Run", "Calibrate"};
    in_menu = true;
    bool last_RE_state = analogRead(ENC_B);
    Serial.println("InMenu");
    while (in_menu){
      #ifdef BLE
        BLELoop(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      #endif
      display.clearDisplay();
      overlay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      for(int i = 0; i < menuOptions; i++){
        if (i == selected){
          display.fillRoundRect(32*i, 16, 32, 32, 4, SSD1306_WHITE);
          display.drawBitmap(32*i, 16, icons[i], 32, 32, SSD1306_INVERSE);
        }
        else{
          display.drawBitmap(32*i, 16, icons[i], 32, 32, SSD1306_INVERSE);
        }
      }
      display.setCursor(8, 49);
      display.print(texts[selected]);
      bool enc = analogRead(ENC_B);
      //Serial.println(enc);
      if (enc != last_RE_state && enc == HIGH){
        if (analogRead(ENC_A) == LOW){
          selected++;
          if (selected >= menuOptions){selected = 0;}
        }
        else{
          selected--;
          if (selected < 0){selected = menuOptions-1;}
        }
      }
      last_RE_state = enc;

      display.display();
      if (digitalRead(ENC_SW) == HIGH){
        in_menu = false;
        return selected;
      }
    }
  }


  void showDifference(int16_t value, const char descr[1], bool clear=false){
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
    //display.setCursor(xShift, 16);
    //display.println(F(descr));
    display.display();
  }
}
