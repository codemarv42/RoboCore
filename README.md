# RoboCore

The RoboCore is basically a dual PCB solution, which is mounted on a chassis and takes over the evaluation and control of sensors and motors. Furthermore, this project was created by several people to develop the best robot for the RoboCup in the Rescue Line division. 

![RoboCore V1](https://github.com/codemarv42/RoboCore/blob/main/Hardware/RoboCoreV3/Raspberry_Pi4_Card_2024-Aug-26_06-24-03PM-000_CustomizedView7502878050.png?raw=true)

# Hardware

The hardware of this project consists mainly of the two PCBs and the chassis.

## Arche-RoboCore PCB

![RoboCoreV3](https://github.com/codemarv42/RoboCore/blob/main/Hardware/RoboCoreV3/Raspberry_Pi4_Card_2024-Aug-26_06-26-57PM-000_CustomizedView41285548237.png?raw=true)

Parameter for PCB (RoboCoreV3):

- Layer: 4
- Thickness: 1,6mm
- Length: 65,05mm
- Height: 56,03mm
- Minimum Copper Trace Width: 0,125mm

## Light sensors PCB

![RoboCore light sensor bar V1](https://github.com/codemarv42/RoboCore/blob/main/Hardware/RoboCore%20light%20sensor%20bar%20V1/RoboCore%20Light%20sensor%20bar_6.png?raw=true)

Parameter for PCB (RoboCore light sensor bar V1):

- Layer: 2
- Thickness: 1,2mm / 1,6mm
- Length: 72,0mm
- Height: 15,23mm
- Minimum Copper Trace Width: 0,15mm

# Software

Required libraries:

- Adafruit_GFX.h - https://github.com/adafruit/Adafruit-GFX-Library
- Adafruit_BusIO.h - https://github.com/adafruit/Adafruit_BusIO/tree/master
- Adafruit_PWMServoDriver.h - https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
- Adafruit_SSD1306.h - https://github.com/adafruit/Adafruit_SSD1306
- MPU6050_light.h - https://github.com/rfetick/MPU6050_light
- VL53L1X.h - https://github.com/pololu/vl53l1x-arduino
- 

## USB driver

In some cases, the USB controller is not automatically detected, which means that you have to install an additional driver:

- https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

## Compilation settings

To compile your own program and then upload it to the RoboCore, some steps are necessary:

1. First you have to install the kernel of the ESP32 in Arduino: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
2. Next, the compilation settings must be specified. To do this, first select "ESP32 Dev Module" under Tools --> Board.
3. In the last step, the parameters of the ESP32 on the RoboCore must be set. To do this, select the following under Tools: 
- Board: "ESP32 Dev Module"
- Upload Speed: "921600"
- CPU Frequency: "240MHz (WiFi/BT)"
- Flash Frequency: "80MHz"
- Flash Mode: "QIO"
- Flash Size: "16MB (128Mb)" **4MB if you installed the W25Q32JVZPIQ Flash**
- Partition Scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)" **for smaller code: Default**
- Programmer: "AVRISP mkll"
- Erase all flash: "Disabled"
