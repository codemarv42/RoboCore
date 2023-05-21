# RoboCore

The RoboCore is basically a dual PCB solution, which is mounted on a chassis and takes over the evaluation and control of sensors and motors. Furthermore, this project was created by several people to develop the best robot for the RoboCup in the Rescue Line division. 

# Software

Required libraries:

- Adafruit_GFX.h - https://github.com/adafruit/Adafruit-GFX-Library
- Adafruit_SSD1306.h - https://github.com/adafruit/Adafruit_SSD1306
- ESP32_Servo.h - https://github.com/jkb-git/ESP32Servo
- MPU6050_light.h - https://github.com/rfetick/MPU6050_light
- CD74HC4067.h - https://github.com/waspinator/CD74HC4067
- VL53L1X.h - https://github.com/pololu/vl53l1x-arduino

## USB driver

In some cases, the USB controller is not automatically detected, which means that you have to install an additional driver:

- https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

## Compilation settings

To compile your own program and then upload it to the RoboCore, some steps are necessary:

1. First you have to install the kernel of the ESP32 in Arduino: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
2. Next, the compilation settings must be specified. To do this, first select "ESP32 Dev Module" under Tools --> Board.
3. In the last step, the parameters of the ESP32 on the NanoBot must be set. To do this, select the following under Tools: 
- Board: "ESP32 Dev Module"
- Upload Speed: "921600"
- CPU Frequency: "240MHz (WiFi/BT)"
- Flash Frequency: "80MHz"
- Flash Mode: "QIO"
- Flash Size: "16MB (128Mb)" **4MB if you installed the W25Q32JVZPIQ Flash**
- Partition Scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)" **for smaller code: Default**
- Programmer: "AVRISP mkll"
- Erase all flash: "Disabled"
