#include <Arduino.h>
#include "icons.h"

/*
   Bitmap converter: https://javl.github.io/image2cpp/
   Settings:
   -Background color: Transparent
   -Invert image colors: True
   -Code output format: Adafruit GFXbitmapFont
   -128x64px
*/

const unsigned char logo_Bitmap [] PROGMEM = {
	// 'logo2, 128x64px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0x00, 0x00, 0x7f, 0x03, 0xfc, 0x15, 0x55, 0x55, 0x50, 0x3f, 0xc0, 0xfe, 0x00, 0x00, 0xff, 
	0xff, 0x80, 0x00, 0x7f, 0x03, 0xfe, 0x0a, 0xaa, 0xaa, 0xa0, 0x7f, 0xc0, 0xfe, 0x00, 0x01, 0xff, 
	0xff, 0xc0, 0x00, 0x7f, 0x03, 0xff, 0x05, 0x55, 0x55, 0x40, 0xff, 0xc0, 0xfe, 0x00, 0x03, 0xff, 
	0xff, 0xe0, 0x00, 0x7f, 0x03, 0xff, 0x82, 0xaa, 0xaa, 0x81, 0xff, 0xc0, 0xfe, 0x00, 0x07, 0xff, 
	0xff, 0xf0, 0x00, 0x7f, 0x03, 0xff, 0xc1, 0x55, 0x55, 0x03, 0xff, 0xc0, 0xfe, 0x00, 0x0f, 0xff, 
	0xff, 0xf8, 0x00, 0x7f, 0x03, 0xff, 0xe0, 0xaa, 0xaa, 0x07, 0xff, 0xc0, 0xfe, 0x00, 0x1f, 0xff, 
	0xff, 0xfc, 0x00, 0x7f, 0x03, 0xff, 0xf0, 0x55, 0x54, 0x0f, 0xff, 0xc0, 0xfe, 0x00, 0x3f, 0xff, 
	0xff, 0xfe, 0x00, 0x7f, 0x01, 0xff, 0xf8, 0x2a, 0xa8, 0x1f, 0xff, 0x80, 0xfe, 0x00, 0x7f, 0xff, 
	0xff, 0xff, 0x00, 0x7f, 0x00, 0xff, 0xfc, 0x15, 0x50, 0x3f, 0xff, 0x00, 0xfe, 0x00, 0xff, 0xff, 
	0xff, 0xff, 0x80, 0x7f, 0x00, 0x7f, 0xfe, 0x0a, 0xa0, 0x7f, 0xfe, 0x00, 0xfe, 0x01, 0xff, 0xff, 
	0xff, 0xff, 0xc0, 0x7f, 0x00, 0x3f, 0xff, 0x05, 0x40, 0xff, 0xfc, 0x00, 0xfe, 0x03, 0xff, 0xff, 
	0xff, 0xff, 0xe0, 0x7f, 0x00, 0x1f, 0xff, 0x82, 0x81, 0xff, 0xf8, 0x00, 0xfe, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xf0, 0x7f, 0x00, 0x0f, 0xff, 0xc1, 0x03, 0xff, 0xf0, 0x00, 0xfe, 0x0f, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0x7f, 0x00, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x00, 0xfe, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0x7f, 0x00, 0x03, 0xff, 0xf0, 0x0f, 0xff, 0xc0, 0x00, 0xfe, 0x3f, 0xff, 0xff, 
	0xfe, 0xff, 0xfe, 0x7f, 0x00, 0x01, 0xff, 0xf8, 0x1f, 0xff, 0x80, 0x00, 0xfe, 0x7f, 0xff, 0x7f, 
	0xfe, 0x7f, 0xff, 0x7f, 0x00, 0x00, 0xff, 0xfc, 0x3f, 0xff, 0x00, 0x00, 0xfe, 0xff, 0xfe, 0x7f, 
	0xfe, 0x3f, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xfc, 0x7f, 
	0xfe, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x7f, 
	0xfe, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x7f, 
	0xfe, 0x07, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x7f, 
	0xfe, 0x03, 0xff, 0xff, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x7f, 
	0xfe, 0x01, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xff, 0xff, 0x80, 0x7f, 
	0xfe, 0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0xff, 0xff, 0x00, 0x7f, 
	0xfe, 0x00, 0x7f, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xfe, 0x00, 0x7f, 
	0xfe, 0x00, 0x3f, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xfc, 0x00, 0x7f, 
	0xfe, 0x00, 0x1f, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xf8, 0x00, 0x7f, 
	0xfe, 0x00, 0x0f, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xf0, 0x00, 0x7f, 
	0xfe, 0x00, 0x07, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xe0, 0x00, 0x7f, 
	0xfe, 0x00, 0x03, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xc0, 0x00, 0x7f, 
	0xfe, 0x00, 0x01, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0x80, 0x00, 0x7f, 
	0xfe, 0x00, 0x00, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0x00, 0x00, 0x7f, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bluetooth [] PROGMEM = {
	// 'bluetooth, 7x9px
	0x18, 0x14, 0x52, 0x34, 0x18, 0x34, 0x52, 0x14, 0x18
};

/// 'calibrate', 32x32px
const unsigned char iconCalibrate [] PROGMEM = {
	0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
	0x00, 0xf9, 0x9f, 0x00, 0x01, 0xc1, 0x83, 0x80, 0x03, 0x81, 0x81, 0xc0, 0x07, 0x01, 0x80, 0xe0, 
	0x0e, 0x01, 0x80, 0x70, 0x1c, 0x01, 0x80, 0x38, 0x18, 0x01, 0x80, 0x18, 0x18, 0x03, 0xc0, 0x18, 
	0x38, 0x0f, 0x30, 0x1c, 0x30, 0x0f, 0x10, 0x0c, 0x30, 0x1f, 0x08, 0x0c, 0xff, 0xff, 0x1f, 0xff, 
	0xff, 0xf0, 0xff, 0xff, 0x30, 0x10, 0xf8, 0x0c, 0x30, 0x08, 0xf0, 0x0c, 0x38, 0x0c, 0xf0, 0x1c, 
	0x18, 0x03, 0xc0, 0x18, 0x18, 0x01, 0x80, 0x18, 0x1c, 0x01, 0x80, 0x38, 0x0e, 0x01, 0x80, 0x70, 
	0x07, 0x01, 0x80, 0xe0, 0x03, 0x81, 0x81, 0xc0, 0x01, 0xc1, 0x83, 0x80, 0x00, 0xf9, 0x9f, 0x00, 
	0x00, 0x7f, 0xfe, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00
};
// 'run', 32x32px
const unsigned char iconRun [] PROGMEM = {
	0x01, 0xf8, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x01, 0xdf, 0x00, 0x00, 
	0x01, 0xef, 0x80, 0x00, 0x01, 0xd7, 0xc0, 0x00, 0x01, 0xeb, 0xe0, 0x00, 0x01, 0xd5, 0xf0, 0x00, 
	0x01, 0xea, 0xf8, 0x00, 0x01, 0xd5, 0x7c, 0x00, 0x01, 0xea, 0xbe, 0x00, 0x01, 0xd5, 0x5f, 0x00, 
	0x01, 0xea, 0xaf, 0x80, 0x01, 0xd5, 0x57, 0xc0, 0x01, 0xea, 0xab, 0xe0, 0x01, 0xd5, 0x55, 0xf0, 
	0x01, 0xea, 0xab, 0xe0, 0x01, 0xd5, 0x57, 0xc0, 0x01, 0xea, 0xaf, 0x80, 0x01, 0xd5, 0x5f, 0x00, 
	0x01, 0xea, 0xbe, 0x00, 0x01, 0xd5, 0x7c, 0x00, 0x01, 0xea, 0xf8, 0x00, 0x01, 0xd5, 0xf0, 0x00, 
	0x01, 0xeb, 0xe0, 0x00, 0x01, 0xd7, 0xc0, 0x00, 0x01, 0xef, 0x80, 0x00, 0x01, 0xdf, 0x00, 0x00, 
	0x01, 0xfe, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00
};
