#include "Pins.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> //https://github.com/adafruit/Adafruit_SSD1306

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Encoder//

unsigned int ENCCounter = 0;
unsigned int currentStateENC_A;
unsigned int lastStateENC_A;
bool ENCButtonState = 0;  //0 - unpressed, 1 - pressed

unsigned int MenuPage = 0;
unsigned int PageValue = 8; //Number of pages

TaskHandle_t Loop1;


void setup() {
  Serial.begin(115200);

  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_SW, INPUT);

  Wire.begin();

  Wire.setClock(3000000); //3 MHz I2C

  DisplayInit();

  xTaskCreatePinnedToCore(  //Task is assigned to a processor core
    Loop1Code,              /* Task function. */
    "Loop1",                /* name of task. */
    10000,                  /* Stack size of task */
    NULL,                   /* parameter of the task */
    0,                      /* priority of the task */
    &Loop1,                 /* Task handle to keep track of created task */
    0);                     /* pin task to core 0 */
}

void loop() {
  ReadEncoder();

  if (ENCCounter > PageValue) {
    ENCCounter = PageValue;
  }
}

void Loop1Code(void* pvParameters) {  //Task of a processor core 0
  Serial.print("Loop1Code running on core:");
  Serial.println(xPortGetCoreID());
  for (;;) {
    Menu();
    delay(300);
  }
}

void ReadEncoder() {  //The encoder is read out

  currentStateENC_A = digitalRead(ENC_A);

  if (currentStateENC_A != lastStateENC_A && currentStateENC_A == 1) {

    if (digitalRead(ENC_B) != currentStateENC_A) {

      if (ENCCounter == 0) {
        ENCCounter = 0;
      } else {
        ENCCounter--;
      }

    } else {
      ENCCounter++;
    }
  }

  lastStateENC_A = currentStateENC_A;

  bool a = digitalRead(ENC_SW);

  if ((a == 1) && (ENCButtonState != 1)) {
    ENCButtonState = 1;
  }
}

void DisplayInit() {  //The display is initalized
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADRESS)) {
    Serial.println(F("SSD1306 allocation failed!"));
  }
  display.display();
  display.clearDisplay();
  BootPage();
}

void BootPage() {  //The boot overlay is displayed
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(28, 19);
  display.println(F("RoboCore"));

  display.setTextSize(1);
  display.setCursor(0, 55);
  display.println(F("Dev. by Marvin Ramm"));

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("REV: "));
  display.print("13354625");
  display.display();

  delay(1800);
}

void Menu() { //Creates the display menu

  if (MenuPage == 0) {  //Pointer on
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> Start program"));

    display.setCursor(0, 25);
    display.println(F("  Calibrate PT"));

    display.setCursor(0, 40);
    display.println(F("  Calibrate Green"));

    display.setCursor(0, 55);
    display.println(F("  Bluetooth"));

    display.display();

  } else if (MenuPage == 1) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> Calibrate PT"));

    display.setCursor(0, 25);
    display.println(F("  Calibrate Green"));

    display.setCursor(0, 40);
    display.println(F("  Bluetooth"));

    display.setCursor(0, 55);
    display.println(F("  Light values"));

    display.display();

  } else if (MenuPage == 2) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> Calibrate Green"));

    display.setCursor(0, 25);
    display.println(F("  Bluetooth"));

    display.setCursor(0, 40);
    display.println(F("  Light values"));

    display.setCursor(0, 55);
    display.println(F("  System check"));

    display.display();

  } else if (MenuPage == 3) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> Bluetooth"));

    display.setCursor(0, 25);
    display.println(F("  Light values"));

    display.setCursor(0, 40);
    display.println(F("  System check"));

    display.setCursor(0, 55);
    display.println(F("  Read MPU6050"));

    display.display();

  } else if (MenuPage == 4) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> Light values"));

    display.setCursor(0, 25);
    display.println(F("  System check"));

    display.setCursor(0, 40);
    display.println(F("  Read MPU6050"));

    display.setCursor(0, 55);
    display.println(F("  TOF values"));

    display.display();

  } else if (MenuPage == 5) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> System check"));

    display.setCursor(0, 25);
    display.println(F("  Read MPU6050"));

    display.setCursor(0, 40);
    display.println(F("  TOF values"));

    display.setCursor(0, 55);
    display.println(F("  Calibrate Red"));

    display.display();

  } else if (MenuPage == 6) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> Read MPU6050"));

    display.setCursor(0, 25);
    display.println(F("  TOF values"));

    display.setCursor(0, 40);
    display.println(F("  Calibrate Red"));

    display.display();
  } else if (MenuPage == 7) {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 10);
    display.println(F("> TOF values"));

    display.setCursor(0, 25);
    display.println(F("  Calibrate Red"));

    display.display();
  }
}