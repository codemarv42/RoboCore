#ifndef PINS_H
#define PINS_H

/////////////////PWM Bus (PCA9685)//////////////////

#define PB_SVO1        0
#define PB_SVO2        1
#define PB_SVO3        2
#define PB_SVO4        3
#define PB_SVO5        4
#define PB_CH5         5
#define PB_CH6         6
#define PB_CH7         7
#define PB_RED_L       8
#define PB_GREEN_L     9
#define PB_BLUE_L      10
#define PB_RED_R       11
#define PB_GREEN_R     12
#define PB_BLUE_R      13

/////////////Motor Driver (TB6612FNG)///////////////

#define PWMA1          21
#define PWMA2          33
#define PWMB1          34
#define PWMB2          35

////////////Shiftregister (74HC595PW)///////////////

#define SHCP           48
#define STCP           47
#define DS             42

//Shiftregister 1

#define SR_AIN1_1      0
#define SR_AIN2_1      1
#define SR_BIN1_1      2
#define SR_BIN2_1      3
#define SR_AIN1_2      4
#define SR_AIN2_2      5
#define SR_BIN1_2      6
#define SR_BIN2_2      7

//Shiftregister 2

#define SR_STBY1       8
#define SR_STBY2       9
#define SR_XSHT1       10
#define SR_XSHT2       11
#define SR_XSHT3       12
#define SR_XSHT4       13
#define SR_Q6          14
#define SR_Q7          15

/////////////////extern Buttons/////////////////////

#define T_E            16 //Button extern
#define T_M            41 //Button center
#define T_R            17 //Button right
#define T_L            18 //Button left

/////////////////////Inputs/////////////////////////

#define FLEX           14
#define VBAT           15

//RoboLight Front

#define PT_REF_L       1
#define PT_L_1         2
#define PT_L_0         4
#define PT_R_0         5
#define PT_R_1         6
#define PT_REF_R       7

//RoboLight Back

#define PT_L_3         10
#define PT_L_2         11
#define PT_R_2         12
#define PT_R_3         13

/////////////////////Outputs/////////////////////////

#define WHITE_L        36
#define WHITE_R        37
#define WHITE          38
#define RED            39
#define GREEN          40

///////////////////////I2C///////////////////////////

#define SDA            8
#define SCL            9
#define MPU6050_ADRESS 0x68
#define OLED_ADRESS    0x3C
#define TOF_ADRESS     0x29
#define PCA9685        0x40


#endif
