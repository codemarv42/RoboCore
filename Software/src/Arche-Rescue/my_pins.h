#ifndef PINS_H
#define PINS_H

///////////ADC Multiplexer (CD74HC4067S)////////////

#define S0               4
#define S1               5
#define S2               12
#define S3               13
#define ADC_MULTI        39

#define ADC_PT_REF_L     0
#define ADC_PT_L1        1
#define ADC_PT_L0        2
#define ADC_PT_M         3
#define ADC_PT_R0        4
#define ADC_PT_R1        5
#define ADC_PT_REF_R     6
#define ADC_PT_RGB       7
#define ADC_AE1          8  //External Analog Input
#define ADC_AE2          9  //External Analog Input
#define ADC_AE3          10 //External Analog Input


/////////////////////Servos/////////////////////////

#define SERVO1           19
#define SERVO2           18
#define SERVO3           17
#define SERVO4           16

/////////////Motor Driver (TB6612FNG)///////////////

#define PWMB             25
#define PWMA             23

////////////Shiftregister (74HC595PW)///////////////

#define SHCP             27
#define STCP             32
#define DS               33

//Shiftregister IC10

#define SR_AIN1          0
#define SR_AIN2          1
#define SR_BIN1          2
#define SR_BIN2          3
#define SR_STBY          4
#define SR_PT_WHITE      5 //Light sensor bar white led //HIGH - ON
#define SR_PT_RED        6 //Light sensor bar red led
#define SR_PT_GREEN      7 //Light sensor bar green led

//Shiftregister IC12

#define SR_PT_BLUE       8 //Light sensor bar blue led
#define SR_LED_L_RED     9 //LOW - ON
#define SR_LED_L_GREEN   10
#define SR_LED_L_BLUE    11
#define SR_LED_R_RED     12
#define SR_LED_R_GREEN   13
#define SR_LED_R_BLUE    14
#define SR_XSHT1         15 //TOF Select

//Shiftregister IC13

#define SR_XSHT2         16
#define SR_XSHT3         17
#define SR_XSHT4         18
#define SR_DE1           19
#define SR_DE2           20
#define SR_DE3           21 //External Digital Pins

/////////////////Rotary Encoder/////////////////////

#define ENC_A            35
#define ENC_B            34
#define ENC_SW           15

/////////////////extern Buttons/////////////////////

#define T_L              2  //Button left
#define T_R              14 //Button right
#define T_M              26 //Button center

/////////////////////Battery////////////////////////

#define VBAT             36

//////////////////////I2C///////////////////////////

#define SDA              21
#define SCL              22
#define MPU6050_ADRESS   0x51
#define OLED_ADRESS      0x3C
#define TOF_ADRESS       0x29


////////////////////////////////////////////////////

#define PAUSE_MESSEN    100 //anpassen nach unten?

#define ANZ_KAL         80 //150
#define ANZ_LS          13

#define SCHNELL         200
#define SOLL            130
#define LANGSAM         75
#define FAKTOR          1.4

#define WEISS           90
#define GRAU            60
#define SCHWARZ         20      //30
#define SILBER          140     //130
#define GRUEN_MAX       28      //anpassen? 24
#define GRUEN_MIN       10       //anpassen? 10
#define ROT_MAX         95      //anpassen
#define ROT_MIN         55      // anpassen

#define TIME_SCHWARZ    1000
#define ANZ_GRUEN       15
#define ANZ_ROT         5
#define ANZ_SILBER      3
#define ANZ_SCHWARZ     10

#define VRICH           1       // 1: rechtsrum; -1: linksrum
#define RADIUS          15      // radius in cm

#define TOF_LINKS       SR_XSHT1    // ANPASSEN auf HARDWARE
#define TOF_RECHTS      SR_XSHT2
#define TOF_UNTEN       SR_XSHT3
#define TOF_OBEN        SR_XSHT4

#define EEPROM_START    0

#define OMEGA_SOLL      100
#define OMEGA           (8983)/360    // millisec pro grad bei OMEGA_SOLL

#define VOR_SOLL        100
#define VOR             (3200)/25         // millisec pro cm bei VOR_SOLL

#endif