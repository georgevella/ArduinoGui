// GfxUtils.h

#ifndef _GFXUTILS_h
#define _GFXUTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>
#include <RA8875/RA8875.h>

uint16_t MakeColour(uint8_t r, uint8_t g, uint8_t b);
uint16_t MakeColour(uint8_t all);

typedef struct {
	const uint32_t	*imageData;
	uint16_t		width;
	uint16_t		height;
	uint16_t 		length;
} tIcon;

void drawMonochromeCompressed(RA8875& tft, const tIcon& icon, uint16_t x, uint16_t y, uint16_t color);
void drawMonochromeCompressed(RA8875& tft, const uint32_t * image, uint32_t isize, uint16_t iwidth, uint16_t x, uint16_t y);

#endif

