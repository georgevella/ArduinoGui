// 
// 
// 

#include "GfxUtils.h"

uint16_t MakeColour(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0x1f) << 11) + ((g & 0x3f) << 5) + (b & 0x1f);
}

uint16_t MakeColour(uint8_t all)
{
	return MakeColour(all, all, all);
}

