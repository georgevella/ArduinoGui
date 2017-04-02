#pragma once

#ifndef _I_DEVICE_CONTEXT
#define _I_DEVICE_CONTEXT

#include "GuiCommon.h"

class IDeviceContext
{
public:
	virtual ~IDeviceContext() = default;
	virtual void WriteText(Point& location, const char* text, TColor textColor) const = 0;
	virtual Dimensions GetFontDimensions() const = 0;

	virtual void DrawBorder(Point& location, Dimensions& dimensions, TColor borderColor) const = 0;
	virtual void DrawRect(Point& location, Dimensions& dimensions, TColor fillColor) const = 0;

	virtual void DrawLine(Point& startLocation, Point& endLocation, TColor lineColor) const = 0;
	virtual void SetFont(const tFont* font) const = 0;
	virtual void ResetFont() const = 0;
	virtual void FillScreen(TColor fillColor) const = 0;
};

#endif