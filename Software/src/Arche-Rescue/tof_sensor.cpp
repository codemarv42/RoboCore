#include "tof_sensor.h"
#include "my_pins.h"
#include "robot.h"
#include "Wire.h"
#include "VL53L1X.h"
#include "shift_register.h"


void TofInit(){
    Tof_links.setAddress(TOF_ADRESS);
    Tof_links.setTimeout(500);
    Tof_links.xsht = TOF_LINKS;
    // Mode setten? Tof_links.setDistanceMode(); mit enum DistanceMode { Short, Medium, Long, Unknown };
    // Tof_rechts.setAddress(TOF_ADRESS);
    // Tof_rechts.setTimeout(500);
    // Tof_rechts.xsht = TOF_RECHTS;
    // Tof_unten.setAddress(TOF_ADRESS);
    // Tof_unten.setTimeout(500);
    // Tof_unten.xsht = TOF_RECHTS;
    // Tof_oben.setAddress(TOF_ADRESS);
    // Tof_oben.setTimeout(500);
    // Tof_oben.xsht = TOF_RECHTS;

    ShiftRegisterWrite(Tof_links.xsht, HIGH);
    delay(10);
    if (!Tof_links.init()){
        // Serial.print("Tof Links failed to init");
        RGB_led_L.red();
    }
    ShiftRegisterWrite(Tof_links.xsht, LOW, false);
    // ShiftRegisterWrite(SR_XSHT2, HIGH);
    // delay(10);
    // if (!Tof_rechts.init()){
    //     // Serial.print("Tof Rechts failed to init");
    //     RGB_led_L.red();
    // }
    // ShiftRegisterWrite(SR_XSHT3, HIGH, false);
    // ShiftRegisterWrite(SR_XSHT2, LOW);
    // delay(10);
    // if (!Tof_unten.init()){
    //     // Serial.print("Tof Unten failed to init");
    //     RGB_led_L.red();
    // }
    // ShiftRegisterWrite(SR_XSHT4, HIGH, false);
    // ShiftRegisterWrite(SR_XSHT3, LOW);
    // delay(10);
    // if (!Tof_oben.init()){
    //     // Serial.print("Tof Oben failed to init");
    //     RGB_led_L.red();
    // }
    // ShiftRegisterWrite(SR_XSHT4, LOW);
    return;
}

void TofRead(){/*
    for(int i=0;i++;i<4){
        int xsht = robot.active_tof[i];
        switch (xsht){
            case -1:
                robot.tof_rechts = -2;
                break;
            case 15:
                ShiftRegisterWrite(xsht,HIGH);
                delay(10);
                robot.tof_links = Tof_links.readSingle();
                ShiftRegisterWrite(xsht,LOW,false);
                break;
            case TOF_RECHTS:
                ShiftRegisterWrite(xsht,HIGH);
                delay(10);
                robot.tof_rechts = Tof_rechts.readSingle();
                ShiftRegisterWrite(xsht,LOW,false);
                break;
            case TOF_UNTEN:
                ShiftRegisterWrite(xsht,HIGH);
                delay(10);
                robot.tof_unten = Tof_unten.readSingle();
                ShiftRegisterWrite(xsht,LOW,false);
                break;
            case TOF_OBEN:
                ShiftRegisterWrite(xsht,HIGH);
                delay(10);
                robot.tof_oben = Tof_oben.readSingle();
                ShiftRegisterWrite(xsht,LOW,false);
                break;
            default:
                robot.tof_rechts = -2;
                break;
        }

    }*/
    for (auto tof : robot.active_tof){
        if(tof==nullptr){continue;};
        ShiftRegisterWrite(tof->xsht,HIGH);
        delayMicroseconds(10);
        tof->data = tof->readSingle();
        ShiftRegisterWrite(tof->xsht,LOW,false);
    }
    return;
}