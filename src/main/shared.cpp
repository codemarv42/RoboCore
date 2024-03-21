
#include "shared.h"
#include "lightsensor.h"
#include "Pins.h"

LightSensorArray white(SR_PT_WHITE);
LightSensorArray red(SR_PT_RED);
LightSensorArray green(SR_PT_GREEN);
DirectSensor refL(SR_PT_WHITE, ADC_PT_REF_L);
DirectSensor refR(SR_PT_WHITE, ADC_PT_REF_R);
LightSensorPair back(SR_DE1, ADC_AE2, ADC_AE3);

LSBase* all_sensors[7] = {&white,&green,&red,nullptr, &refL,&refR, &back};  // nullptr is placeholder for an (optional) blue LightSensorArray
LightSensorArray* ls[4] = {&white,&green,&red,nullptr};

int16_t diff_interchange;
