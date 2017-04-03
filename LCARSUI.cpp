#include "LCARSUI.h"

TLCarsSettings LCarsSettings;

inline void LCarsCorner::Draw()
{
	Dimensions _outer_quadrant_dims(LCarsSettings.titleBarHeight, LCarsSettings.titleBarHeight);
	Dimensions _inner_quadrant_dims;

	switch (_corner_type)
	{
	case LCARS_CORNER_TOPLEFT: 
		_inner_quadrant_dims = Dimensions(
			LCarsSettings.topLeftCornerExtension, 
			LCarsSettings.topLeftCornerHeight - LCarsSettings.titleBarHeight);
		break;
	case LCARS_CORNER_TOPRIGHT: break;
	case LCARS_CORNER_BOTTOMLEFT: 
		_inner_quadrant_dims = Dimensions(
			LCarsSettings.topLeftCornerExtension, 
			LCarsSettings.topLeftCornerHeight - LCarsSettings.titleBarHeight);
		break;
	case LCARS_CORNER_BOTTOMRIGHT: break;	
	}

	auto maxHeight = _outer_quadrant_dims.Height + _inner_quadrant_dims.Height;
	auto maxWidth = LCarsSettings.sideBarWidth + _inner_quadrant_dims.Width;
	Point outerQuadrantCenter;
	DC_DRAWCURVE_QUADRANT quadrantType = DRAWCURVEQUAD_270;
	Point a, b, innerQuadrantCenter;

	Dimensions screenDimensions = Dc().GetScreenDimensions();

	switch (_corner_type)
	{
	case LCARS_CORNER_TOPLEFT:
		outerQuadrantCenter = Point(_outer_quadrant_dims.Width, _outer_quadrant_dims.Height);
		a = Point(0, _outer_quadrant_dims.Height);
		b = Point(_outer_quadrant_dims.Width, 0);
		innerQuadrantCenter = Point(maxWidth, maxHeight);
		quadrantType = DRAWCURVEQUAD_360;
		break;
	case LCARS_CORNER_TOPRIGHT: 		
		quadrantType = DRAWCURVEQUAD_90;

		break;
	case LCARS_CORNER_BOTTOMLEFT: 
		outerQuadrantCenter = Point(_outer_quadrant_dims.Width, screenDimensions.Height - _outer_quadrant_dims.Height);
		innerQuadrantCenter = Point(maxWidth, screenDimensions.Height - maxHeight);
		a = Point(0, screenDimensions.Height - _outer_quadrant_dims.Height - _inner_quadrant_dims.Height);
		b = Point(_outer_quadrant_dims.Width, screenDimensions.Height - maxHeight);
		quadrantType = DRAWCURVEQUAD_270;
		break;
	case LCARS_CORNER_BOTTOMRIGHT: 
		quadrantType = DRAWCURVEQUAD_180;
		break;
	}

	Dc().DrawQuadrant(outerQuadrantCenter, _outer_quadrant_dims, quadrantType, _borderColor, _fillColor);
	Dc().DrawRect(a, Dimensions(_outer_quadrant_dims.Width, _inner_quadrant_dims.Height), _fillColor);
	Dc().DrawRect(b, Dimensions(maxWidth - _outer_quadrant_dims.Width, maxHeight), _fillColor);
	Dc().DrawQuadrant(innerQuadrantCenter, _inner_quadrant_dims, quadrantType, 0, 0);
}

void LCarsTitleBar::Draw()
{
	auto offset = LCarsSettings.sideBarWidth + LCarsSettings.topLeftCornerExtension + LCarsSettings.widgetGap;
	auto screenDims = Dc().GetScreenDimensions();
	auto titlebarWidth = screenDims.Width - ((offset * 2) + (LCarsSettings.widgetGap * 2));

	auto location = Point(offset, 0);
	auto dims = Dimensions(titlebarWidth, LCarsSettings.titleBarHeight);
	Dc().DrawRect(location, dims, _fillColor);

	Dc().DrawRect(Point(offset + titlebarWidth + LCarsSettings.widgetGap, location.Y), Dimensions(offset-1, dims.Height), _fillColor);

	_label.TopLeft(location);
	_label.Size(dims);
	_label.Draw();
}

void LCarsFooter::Draw()
{
	auto offset = LCarsSettings.sideBarWidth + LCarsSettings.bottomLeftCornerExtension + LCarsSettings.widgetGap;
	auto screenDims = Dc().GetScreenDimensions();
	auto titlebarWidth = screenDims.Width - ((offset * 2) + (LCarsSettings.widgetGap * 2));

	auto location = Point(offset, screenDims.Height - LCarsSettings.titleBarHeight);
	auto dims = Dimensions(titlebarWidth, LCarsSettings.titleBarHeight);
	Dc().DrawRect(location, dims, _fillColor);

	Dc().DrawRect(Point(offset + titlebarWidth + LCarsSettings.widgetGap, location.Y), Dimensions(offset-1, dims.Height), _fillColor);

	_label.TopLeft(location);
	_label.Size(dims);
	_label.Draw();
}

