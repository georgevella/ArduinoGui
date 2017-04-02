#include "LCARSUI.h"

inline void LCarsCorner::Draw()
{
	auto maxHeight = _outer_quadrant_dims.Height + _inner_quadrant_dims.Height;
	auto maxWidth = _sidebar_width + _inner_quadrant_dims.Width;
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

