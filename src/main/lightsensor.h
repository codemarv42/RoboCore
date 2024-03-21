#pragma once
#include <sys/types.h>
#include <sys/_stdint.h>
#include <sys/_types.h>
#include "esp32-hal.h"
#include "sys_arch.h"


#include <Arduino.h>
#include "Pins.h"
#include "shiftregister.h"



struct SensorData{
  /*
  Struct that contains the data of a single sensor, along with its max and min values
  */
  int16_t min=0x7FFF; // max value for a 16-Bit integer in Hexadecimal
  int16_t max=1;
  int16_t value=0;
};

class LSBase{
  /*
  Class that is the base class of all light sensor classes to allow the all to be read in a single read() call
  */
  protected:
    uint8_t led_pin;
    virtual void ledOn(); // turns on the LED
    virtual void ledOff(); // turns off the LED
  
  public:
    virtual void read() = 0; // Read out this sensor
    virtual void calibrate_turn(int i); // calibrate this sensor one time. i is the number of the iteration
};

class mapper{
  /*
  Class that defines the map() function
  */
  public:
    inline int16_t map(int16_t value, int16_t minv, int16_t maxv);
};

class LightSensorArray : public LSBase, private mapper{
  /*
  Class for a light sensor array of one color
  */
  public:
    SensorData left_outer;
    SensorData left;
    SensorData center;
    SensorData right;
    SensorData right_outer;

    LightSensorArray(int led_pin);
    void calibrate_turn(int i);
    void read();
};


class DirectSensor : public LSBase{
  /*
  Class that reads raw values (used for silver detection)
  */
  public:
    SensorData data;
    DirectSensor(uint8_t ledPin, uint8_t adcPin);
    void read();
    void calibrate_turn(int i){}; // Placeholder
  private:
    uint8_t adcPin;
};

class LightSensorPair : public LSBase, private mapper{
  /*
  Class that is used for a pair of light sensors with a single LED
  */
  public:
    SensorData left;
    SensorData right;
    void read();
    void ledOn();
    void ledOff();
    void calibrate_turn(int i);
    LightSensorPair(int ledPin, int PinLeft, int PinRight);
  
  private:
    int adcPinLeft;
    int adcPinRight;
};

// calibrate all sensors in 'sensors' 'amount' times with 'time_between_read_ms' spacing
extern void calibrate(LSBase* sensors[],const int amount,const int time_between_read_ms);

// read all sensors
extern void read();

// read all sensors in this array
extern void read(LSBase* sensors[]);
extern void read(LightSensorArray* sensors[]);

// read all sensors passed by initializer
extern void read(std::initializer_list<LSBase*> sensors);

