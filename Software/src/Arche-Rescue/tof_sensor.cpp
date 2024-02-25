#include "tof_sensor.h"
#include "my_pins.h"
#include "robot.h"
#include "Wire.h"
#include "VL53L1X.h"
#include "shift_register.h"

VL53L1X Tof_links;
VL53L1X Tof_rechts;
VL53L1X Tof_unten;
VL53L1X Tof_oben;

void TofInit(){
    Tof_links.setAddress(TOF_ADRESS);
    Tof_links.setTimeout(500);
    Tof_rechts.setAddress(TOF_ADRESS);
    Tof_rechts.setTimeout(500);
    Tof_unten.setAddress(TOF_ADRESS);
    Tof_unten.setTimeout(500);
    Tof_oben.setAddress(TOF_ADRESS);
    Tof_oben.setTimeout(500);

    ShiftRegisterWrite(SR_XSHT1, HIGH);
    delay(10);
    if (!Tof_links.init()){
        // Serial.print("Tof Links failed to init");
        RGB_led_L.red();
    }
    ShiftRegisterWrite(SR_XSHT1, LOW, false);
    ShiftRegisterWrite(SR_XSHT2, HIGH);
    delay(10);
    if (!Tof_rechts.init()){
        // Serial.print("Tof Rechts failed to init");
        RGB_led_L.red();
    }
    ShiftRegisterWrite(SR_XSHT3, HIGH, false);
    ShiftRegisterWrite(SR_XSHT2, LOW);
    delay(10);
    if (!Tof_unten.init()){
        // Serial.print("Tof Unten failed to init");
        RGB_led_L.red();
    }
    ShiftRegisterWrite(SR_XSHT4, HIGH, false);
    ShiftRegisterWrite(SR_XSHT3, LOW);
    delay(10);
    if (!Tof_oben.init()){
        // Serial.print("Tof Oben failed to init");
        RGB_led_L.red();
    }
    ShiftRegisterWrite(SR_XSHT4, LOW);
    return;
}

int TofRead(int xsht){
    ShiftRegisterWrite(xsht,HIGH);
    delay(1);
    int r = 0;
    switch (xsht){
    case TOF_LINKS:
        r = Tof_links.readSingle();
        break;
    case TOF_RECHTS:
        r = Tof_rechts.readSingle();
        break;
    case TOF_UNTEN:
        r = Tof_unten.readSingle();
        break;
    case TOF_OBEN:
        r = Tof_oben.readSingle();
        break;
    default:
        break;
    }
    ShiftRegisterWrite(xsht,LOW,false);
    return r;
}