#include "Pins.h"

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306

#include "icons.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define _ 0x00
#define Q 0xFF

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

void showDifference(int16_t value, const char descr[1], bool clear=false){
  if (clear){
    display.clearDisplay();
    display.drawFastVLine(64, 32-8, 16, SSD1306_WHITE);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
  }
  int16_t xShift = value;
  display.drawFastVLine(xShift + 64, 32-8, 16, SSD1306_WHITE);
  //display.drawFastHLine(xShift, 64, 32, uint16_t color);
  //display.setCursor(xShift, 16);
  //display.println(F(descr));
  display.display();
}

