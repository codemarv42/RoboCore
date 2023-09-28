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
/*
const GFXbitmapGlyph logo_Glyphs [] PROGMEM = {
	{ 0, 128, 64, 0, '0' }// 'logo2'
};

const GFXbitmapFont logo_Font PROGMEM = {
	(uint8_t *)logo_Bitmap,
	(GFXbitmapGlyph *)logo_Glyphs,
	1
};
*/