#pragma once

#ifndef _RA8875_DEVICE_CONTEXT_H
#define _RA8875_DEVICE_CONTEXT_H

#include <SPI.h>
#include <RA8875/RA8875.h>
#include "IDeviceContext.h"


class Ra8875DeviceContext
	: public IDeviceContext
{
public:
	explicit Ra8875DeviceContext(RA8875& ra8875) : _ra8875(ra8875) {

	}

	void WriteText(Point location, const char* text, TColor textColor) const override;

	Dimensions GetFontDimensions() const override;

	void DrawBorder(Point location, Dimensions dimensions, TColor borderColor) const override;

	void DrawRect(Point location, Dimensions dimensions, TColor fillColor) const override;

	void DrawLine(Point startLocation, Point endLocation, TColor lineColor) const override;

	void SetFont(const tFont* font) const override;
	void ResetFont() const override;
	void FillScreen(TColor fillColor) const override;

	virtual ~Ra8875DeviceContext() override
	{
	}

	void DrawCurve(Point center, Dimensions axis, DC_DRAWCURVE_QUADRANT quadrant, TColor borderColor) override;
	void DrawQuadrant(Point center, Dimensions axis, DC_DRAWCURVE_QUADRANT quadrant, TColor borderColor, TColor fillColor) override;
	Dimensions GetScreenDimensions() const override;
private:
	RA8875& _ra8875;
};

#endif