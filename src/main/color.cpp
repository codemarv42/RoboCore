
//#ifndef COLOR_CPP
//#define COLR_CPP
#include "Pins.h"
#include "color.h"
#include <Arduino.h>
#include "lightsensor.h"

#define check_length 1
#define GREEN_THRESHOLD 15
#define BLACK_THRESHOLD 15

namespace color{
  struct ColorCount{
    uint8_t left = 0;
    uint8_t right = 0;
  };

  ColorCount green;
  ColorCount red;
  ColorCount black;


  bool on_black(int side){
  return (side & LEFT && black.left >= check_length) || (side & RIGHT && black.right >= check_length);
  }
  bool on_green(int side){
  return (side & LEFT && green.left >= check_length) || (side & RIGHT && green.right >= check_length);
  }

  void update(LightSensor* s_white, LightSensor* s_green, LightSensor* s_red){
    if (s_white->left_outer.value < BLACK_THRESHOLD && black.left < check_length){
      black.left++;
    }
    else if(black.left > 0){
      black.left--;
    }

    if (s_white->right_outer.value < BLACK_THRESHOLD && black.right < check_length){
      black.right++;
    }
    else if(black.right > 0){
      black.right--;
    }

    if (s_green->left.value - s_red->left.value*1.2 > GREEN_THRESHOLD){
      if (green.left < check_length){
        green.left++;
      }
    }
    else if(green.left > 0){
      green.left--;
    }

    if (s_green->right.value - s_red->right.value > GREEN_THRESHOLD){
      if (green.right < check_length){
        green.right++;
      }
    }
    else if(green.right > 0){
      green.right--;
    }
    //TODO: Add Red recog
  }
}

//#endif
