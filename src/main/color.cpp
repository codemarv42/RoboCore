
#include "Pins.h"
#include "color.h"
#include <Arduino.h>
#include "lightsensor.h"

#define check_length 7  // How often a color must be checked to allow a true call 
#define GREEN_THRESHOLD 15 // Minimum diff for a recognised green
#define BLACK_THRESHOLD 35 // Max value for black
#define CHECKRED

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
  bool on_red(int side){
  return (side & LEFT && red.left >= check_length) || (side & RIGHT && red.right >= check_length);
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

    if (s_green->left.value - s_red->left.value > GREEN_THRESHOLD){
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
    #ifdef CHECKRED
      if (-s_red->left.value + s_red->left.value > GREEN_THRESHOLD){
        if (red.left < check_length){
          red.left++;
        }
      }
      else if(red.left > 0){
        red.left--;
      }

      if (-s_green->right.value + s_red->right.value > GREEN_THRESHOLD){
        if (red.right < check_length){
          red.right++;
        }
      }
      else if(red.right > 0){
        red.right--;
      }
    #endif
  }
}
