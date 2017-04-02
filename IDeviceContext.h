#pragma once

#ifndef _I_DEVICE_CONTEXT
#define _I_DEVICE_CONTEXT

#include "GuiCommon.h"
#include <RA8875/_settings/font.h>

///0[180-270°],1[270-0°],2[0-90°],3[90-180°]
enum DC_DRAWCURVE_QUADRANT
{
	DRAWCURVEQUAD_90 = 2,
	DRAWCURVEQUAD_180 = 3,
	DRAWCURVEQUAD_270 = 0,
	DRAWCURVEQUAD_360 = 1,
};

class IDeviceContext
{
public:
	virtual ~IDeviceContext() = default;
	virtual void WriteText(Point location, const char* text, TColor textColor) const = 0;
	virtual Dimensions GetFontDimensions() const = 0;
	virtual Dimensions GetScreenDimensions() const = 0;

	virtual void DrawBorder(Point location, Dimensions dimensions, TColor borderColor) const = 0;
	virtual void DrawRect(Point location, Dimensions dimensions, TColor fillColor) const = 0;
	virtual void DrawLine(Point startLocation, Point endLocation, TColor lineColor) const = 0;

	/// Draws a curve around the point defined by center.
	virtual void DrawCurve(Point center, Dimensions axis, DC_DRAWCURVE_QUADRANT quadrant, TColor borderColor) = 0;
	virtual void DrawQuadrant(Point center, Dimensions axis, DC_DRAWCURVE_QUADRANT quadrant, TColor borderColor, TColor fillColor) = 0;


	virtual void SetFont(const tFont* font) const = 0;
	virtual void ResetFont() const = 0;
	virtual void FillScreen(TColor fillColor) const = 0;
};

#endif