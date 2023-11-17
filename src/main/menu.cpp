#include "esp32-hal-adc.h"
#include "Pins.h"

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306

#include "icons.h"
#include "ESP32_BLE.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void DisplayInit(){
if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADRESS)) {
    Serial.println(F("SSD1306 allocation failed!"));
  }
  display.display();
  display.clearDisplay();

  /*
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(18, 19);
  display.println(F("RoboCore"));
  */
  display.drawBitmap(0,0,logo_Bitmap, 128, 64, SSD1306_WHITE);
  display.display();
  delay(1000);
}

void DisplayMenu(){

}

#define MENU_RUN 0
#define MENU_CALIBRATE 1
#define MENU_INVOKE_EEPROM 2
#define SHOW_VBAT
bool in_menu = false;
int menu_selected = 0;

void overlay(){
  if (BLEStat == 2){
    display.drawBitmap(0,0, bluetooth, 7,9, SSD1306_WHITE);
    #ifdef SHOW_VBAT
      display.setCursor(0, 0);
      display.print(F(analogRead(VBAT)));
    #endif
  }
}

void showGyroWaiting(){
  display.clearDisplay();
  //overlay();
  display.setCursor(0,4);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Calibrating Gyro...");
  display.display();
}

void menu(){
  in_menu = true;
  Serial.println("InMenu");
  while (in_menu){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,10);
    display.setTextSize(1);
    //overlay();
    display.setCursor(0,10);
    display.setTextSize(2);
    display.println("RUN");
    display.println("calibrate");
    display.println("eeprom");
    display.display();
    delay(10);
    if (digitalRead(ENC_SW) == HIGH){
      in_menu = false;
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

