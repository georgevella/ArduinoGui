#pragma once

#ifndef _GUI_COMMON_H
#define _GUI_COMMON_H

typedef uint16_t TColor;

struct Point
{
	uint16_t X;
	uint16_t Y;

	Point()
		: X(0), Y(0)
	{

	}

	explicit Point(uint16_t x)
		: X(x), Y(x)
	{

	}

	Point(uint16_t x, uint16_t y)
		: X(x), Y(y)
	{

	}

	inline void Change(uint16_t x, uint16_t y)
	{
		X = x;
		Y = y;
	}
};

struct Dimensions
{
	uint16_t Width;
	uint16_t Height;


	Dimensions()
		: Width(0), Height(0)
	{

	}

	explicit Dimensions(uint16_t square)
		: Width(square), Height(square)
	{

	}

	Dimensions(uint16_t width, uint16_t height)
		: Width(width), Height(height)
	{

	}
};

#endif