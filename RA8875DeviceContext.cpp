#include "RA8875DeviceContext.h"


void Ra8875DeviceContext::WriteText(Point location, const char* text, TColor textColor) const
{
	char log[100] = { 0 };
	sprintf(log, "WriteText@ (%d, %d)", location.X, location.Y);
	Serial.println(log);

	_ra8875.setTextColor(textColor);

	_ra8875.setCursor(
		location.X,
		location.Y
	);
	_ra8875.println(text);
}

Dimensions Ra8875DeviceContext::GetFontDimensions() const
{
	Dimensions dims = Dimensions(_ra8875.getFontWidth(), _ra8875.getFontHeight());
	return dims;
}

void Ra8875DeviceContext::DrawBorder(Point location, Dimensions dimensions, TColor borderColor) const
{
	_ra8875.drawRect(location.X, location.Y, dimensions.Width, dimensions.Height, borderColor);
}

void Ra8875DeviceContext::DrawRect(Point location, Dimensions dimensions, TColor fillColor) const
{
	_ra8875.fillRect(location.X, location.Y, dimensions.Width, dimensions.Height, fillColor);
}

void Ra8875DeviceContext::DrawLine(Point startLocation, Point endLocation, TColor lineColor) const
{
	_ra8875.drawLine(startLocation.X, startLocation.Y, endLocation.X, endLocation.Y, lineColor);
}

void Ra8875DeviceContext::SetFont(const tFont* font) const
{
	if (font == nullptr)
	{
		Serial.println("No font specified");
		return;
	}
	_ra8875.setFont(font);
}

void Ra8875DeviceContext::ResetFont() const
{
	_ra8875.setFont(INT);
}

void Ra8875DeviceContext::FillScreen(TColor fillColor) const
{
	_ra8875.fillWindow(fillColor);
}

void Ra8875DeviceContext::DrawCurve(Point center, Dimensions axis, DC_DRAWCURVE_QUADRANT quadrant, TColor borderColor)
{
	_ra8875.drawCurve(center.X, center.Y, axis.Width, axis.Height, quadrant, borderColor);
}

void Ra8875DeviceContext::DrawQuadrant(Point center, Dimensions axis, DC_DRAWCURVE_QUADRANT quadrant, TColor borderColor, TColor fillColor)
{
	_ra8875.fillCurve(center.X, center.Y, axis.Width, axis.Height, quadrant, fillColor);
	if (borderColor != fillColor)
		_ra8875.drawCurve(center.X, center.Y, axis.Width, axis.Height, quadrant, borderColor);
}
