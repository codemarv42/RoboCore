# RoboCore

The RoboCore is basically a dual PCB solution, which is mounted on a chassis and takes over the evaluation and control of sensors and motors. Furthermore, this project was created by several people to develop the best robot for the RoboCup in the Rescue Line division. 

![RoboCore V1](https://github.com/codemarv42/RoboCore/blob/main/Hardware/RoboCoreV3/Raspberry_Pi4_Card_2024-Aug-26_06-24-03PM-000_CustomizedView7502878050.png?raw=true)

# Hardware

A combination of two PCBs is used as our hardware, where the main PCB (RoboCoreV3) is plugged onto a Raspberry Pi and therefore forms the main control unit. The second circuit board (RoboLightV4) is installed on the underside of the robot and is responsible for measuring the light values on the line track. Below is our current hardware combination.

## Changelog

RoboCore:

- V3: Raspberry PI hat design, external Display and buttons as user interface, ESP32-S3, two motor driver, Neopixel LED, Voltage regulator update
- V2: MPU6050 as secound PCB, dual light sensor bar, Pullup Fix near buttons
- V1: Inital Version

RoboLight:

- V4: Use Neopixel RGB LEDs instead, remove center photo transistor
- V3: Control Leds individual via a shiftregister
- V2: Fix distance between inner photo transistors
- V1: Inital Version

## Control unit PCB

![RoboCoreV3](https://github.com/codemarv42/RoboCore/blob/main/Hardware/RoboCoreV3/Raspberry_Pi4_Card_2024-Aug-26_06-26-57PM-000_CustomizedView41285548237.png?raw=true)

Parameter for PCB (RoboCoreV3):

- Layer: 4
- Thickness: 1,6mm
- Length: 65,05mm
- Height: 56,03mm
- Minimum Copper Trace Width: 0,125mm

## Light sensors PCB

![RoboLightV4](https://github.com/codemarv42/RoboCore/blob/main/Hardware/RoboCoreV3/RoboLightV4_2024-Aug-26_07-30-13PM-000_CustomizedView8316888771.png?raw=true)

Parameter for PCB (RoboLightV4):

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

## Compilation settings

To compile your program and then upload it to the RoboCore, some steps are necessary:

1. First you have to install the kernel of the ESP32 in Arduino: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
2. Next, the compilation settings must be specified. To do this, first select "ESP32S3 Dev Module" under Tools --> Board.
3. In the last step, the parameters of the ESP32 on the RoboCore must be set. To do this, select the following under Tools: 
- Board: "ESP32S3 Dev Module"
- USB CDC On Boot: "Enabled"
- CPU Frequency: "240MHz (WiFi/BT)"
- Flash Mode: "QIO 120MHz"
- Flash Size: "8MB (32Mb)"
- Partition Scheme: "8MB with SPIFFS"
- Erase all flash: "Disabled"
- Upload Speed: "921600"
