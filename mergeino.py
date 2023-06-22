"""
MergeINO

merges all specified .ino, .cpp and .h files into one
"""
import os

target_file="main/main.ino"
pins = "hardware/pins.h"

files = ["hardware/shiftregister.cpp",
         "hardware/hardware.ino",
         "hardware/lightsensors.cpp",
         "hardware/gyro.cpp",
         "main/main.ino",
         "hardware/motor.cpp",
         ]

strbuffer = []

for file in files:
    f = open("src/" + file,"r")
    strbuffer.append(f.read())
    f.close()
    
f = open(target_file, "w")
f.write("")
f.close()
f = open(target_file, "a")
for s in strbuffer:
    f.write(s + "\n")
f.close()

input("press any key")


