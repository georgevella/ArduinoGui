// LCARSUI.h

#ifndef _LCARSUI_h
#define _LCARSUI_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "IDeviceContext.h"
#include "Widget.h"

#define SIDEBAR_WIDTH		100
#define TITLEBAR_HEIGHT		25

#define INNERQUAD_WIDTH		11
#define INNERQUAD_HEIGHT	11
#define MAX_WIDTH			(SIDEBAR_WIDTH + INNERQUAD_WIDTH)
#define MAX_HEIGHT			(TITLEBAR_HEIGHT+INNERQUAD_HEIGHT)
#define QUADRANT_WIDTH		25
#define QUADRANT_HEIGHT		25

enum LCarsCornerType
{
	LCARS_CORNER_TOPLEFT,
	LCARS_CORNER_TOPRIGHT,
	LCARS_CORNER_BOTTOMLEFT,
	LCARS_CORNER_BOTTOMRIGHT
};

class LCarsCorner : public Widget
{
public:
	LCarsCorner(IDeviceContext& tft, LCarsCornerType cornerType)
		: Widget(tft), 
	_corner_type(cornerType), 
	_outer_quadrant_dims(QUADRANT_WIDTH, QUADRANT_HEIGHT),
	_inner_quadrant_dims(INNERQUAD_WIDTH, INNERQUAD_HEIGHT),
	_borderColor(0xFCE0),
	_fillColor(0xFCE0), _sidebar_width(100)

	{

	}

	void Draw() override;

private:
	LCarsCornerType _corner_type;
	Dimensions _outer_quadrant_dims;
	Dimensions _inner_quadrant_dims;
	TColor _borderColor;
	TColor _fillColor;
	uint16_t _sidebar_width;
};

#endif

