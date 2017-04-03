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
//#define MAX_WIDTH			(SIDEBAR_WIDTH + INNERQUAD_WIDTH)
//#define MAX_HEIGHT			(TITLEBAR_HEIGHT+INNERQUAD_HEIGHT)
//#define QUADRANT_WIDTH		25
//#define QUADRANT_HEIGHT		25

#define LCARS_LIGHTYELLOW	0xFCC6
#define LCARS_BARELYRED		0xDB28
#define LCARS_BLUE			0x009D
#define LCARS_LIGHTBLUE		0x54DF
#define LCARS_DARKCYAN		0x3339
#define LCARS_LIGHTPINK		0xCCD9

enum LCarsCornerType
{
	LCARS_CORNER_TOPLEFT,
	LCARS_CORNER_TOPRIGHT,
	LCARS_CORNER_BOTTOMLEFT,
	LCARS_CORNER_BOTTOMRIGHT
};

struct TLCarsSettings
{
	uint8_t sideBarWidth;
	uint8_t titleBarHeight;

	uint8_t widgetGap;

	uint8_t topLeftCornerHeight;
	uint8_t topLeftCornerExtension;
	uint8_t bottomLeftCornerExtension;
	uint8_t bottomLeftCornerHeight;

	TLCarsSettings()
	{
		widgetGap = 5;
		sideBarWidth = SIDEBAR_WIDTH;
		titleBarHeight = TITLEBAR_HEIGHT;

		bottomLeftCornerHeight = topLeftCornerHeight = TITLEBAR_HEIGHT + INNERQUAD_HEIGHT;
		topLeftCornerExtension = INNERQUAD_WIDTH;
		bottomLeftCornerExtension = INNERQUAD_WIDTH;
	}
};

extern TLCarsSettings LCarsSettings;

class LCarsCorner : public Widget
{
public:
	LCarsCorner(IDeviceContext& tft, LCarsCornerType cornerType)
		: Widget(tft),
		  _corner_type(cornerType),
		  _borderColor(LCARS_LIGHTYELLOW),
		  _fillColor(LCARS_LIGHTYELLOW)

	{
	}

	void Draw() override;

private:
	LCarsCornerType _corner_type;
	TColor _borderColor;
	TColor _fillColor;
};

class LCarsTitleBar : public Widget
{
private:
	TColor _borderColor;
	TColor _fillColor;
	Label _label;
public:
	explicit LCarsTitleBar(IDeviceContext& dc)
		: Widget(dc), _borderColor(LCARS_LIGHTPINK), _fillColor(LCARS_LIGHTPINK), _label(dc)
	{
		_label.ForegroundColor(0);
	}

	inline void BorderColor(TColor color) { _borderColor = color; }
	inline void FillColor(TColor color) { _fillColor = color; }
	inline TColor BorderColor() const { return _borderColor; }
	inline TColor FillColor() const { return _fillColor; }
	void Draw() override;

	inline Label* Caption() { return &_label; }
};
class LCarsFooter : public Widget
{
private:
	TColor _borderColor;
	TColor _fillColor;
	Label _label;
public:
	explicit LCarsFooter(IDeviceContext& dc)
		: Widget(dc), _borderColor(LCARS_BLUE), _fillColor(LCARS_BLUE), _label(dc)
	{
		_label.ForegroundColor(0);
	}

	inline void BorderColor(TColor color) { _borderColor = color; }
	inline void FillColor(TColor color) { _fillColor = color; }
	inline TColor BorderColor() const { return _borderColor; }
	inline TColor FillColor() const { return _fillColor; }
	void Draw() override;

	inline Label* Caption() { return &_label; }
};

#endif
