#include <cstdint>
#include <sys/_stdint.h>
#include <numeric>

#include "linefollower.h"

#include <Arduino.h>
#include "motor.h"
#include "shared.h"
#include "color.h"

////// LINE FOLLOWING //////
void lf(){
  static int16_t last = 0;
  #ifdef LF_USE_BLACK_INTRO
    static int black_timestamp;
  #endif
  
  #ifdef LF_ACTIVE
    #define diff_outer_factor 4 // Factor for the outer light 
    #define mul 2
    
    int16_t mot_diff;
    #ifdef LF_USE_BACK
      int16_t diff_back = back.left.value - back.right.value;
    #endif
    int16_t diff = (white.left.value - white.center.value) - (white.right.value - white.center.value);
    int16_t diff_green = (green.left.value-red.left.value)-(green.right.value-red.right.value); // difference to ignore green value
    int16_t diff_outer = white.left_outer.value - white.right_outer.value;
    mot_diff = ((diff + diff_green*2)*2 + diff_outer*diff_outer_factor) * mul;  // calculate inner to outer mult
    diff_interchange = mot_diff;
    #ifdef LF_USE_BLACK_INTRO
      //shift_register::write(SR_LED_R_BLUE, LOW, true);
      bool right = color::on_black(RIGHT);
      bool left  = color::on_black(LEFT);
      if (left || right){
        black_timestamp = millis();
        //mot_diff += -int(right)*150 + int(left)*150;
      }
      else{
        int t = millis();
        if (black_timestamp + 400 < t && black_timestamp + 2000 > t){
          shift_register::write(SR_LED_R_BLUE, !left, true);
          mot_diff += int(right)*150 - int(left)*150;
        }
      }

    #endif

    #ifndef NOMOTORS
      int16_t v = min(max(V2, int(VZ-mot_diff*1.5)), VZ); // scale bas speed based on difference

      #ifdef LF_USE_BACK
        float scale = 1/(max(abs(mot_diff), 1)*0.5); // only use back LS if no difference

        int16_t vback_a = min(0, int(diff_back)) * scale;
        int16_t vback_b = max(0, int(diff_back)) * scale;
      #else
        #define vback_a 0
        #define vback_b 0
      #endif

      // start motors
      motor::fwd(A, ( v + (mot_diff*2+last)/3) + vback_a);
      motor::fwd(B, ( v - (mot_diff*2+last)/3) + vback_b);
      last = mot_diff;
    #endif
    diff_interchange = mot_diff;
  #endif
      
}



