//Include: https://github.com/adafruit/Adafruit_NeoPixel

#include <Adafruit_NeoPixel.h>
#include "pins.h"

// How many NeoPixels are attached?
#define NUMPIXELS 4

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

Adafruit_NeoPixel RGB(NUMPIXELS, RGB_LED, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("RoboCoreV3 - RGB LED Test");

  RGB.begin();           // INITIALIZE NeoPixel RGB object (REQUIRED)
  RGB.show();            // Turn OFF all pixels ASAP
  RGB.setBrightness(BRIGHTNESS);
}

void loop() {
  RGB.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    RGB.setPixelColor(i, RGB.Color(0, 150, 0));

    RGB.show();   // Send the updated pixel colors to the hardware.

    delay(500); // Pause before next pass through loop
  }
}
