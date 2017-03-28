// GfxUtils.h

#ifndef _GFXUTILS_h
#define _GFXUTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

uint16_t MakeColour(uint8_t r, uint8_t g, uint8_t b);
uint16_t MakeColour(uint8_t all);

#endif

