#pragma once
#include <SPI.h>
#include <RA8875/RA8875.h>

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

class IDeviceContext	
{
public:
	virtual ~IDeviceContext() = default;
	virtual void WriteText(Point& location, const char* text) const = 0;
	virtual Dimensions GetFontDimensions() const = 0;

	virtual void DrawBorder(Point& location, Dimensions& dimensions, TColor borderColor) const = 0;
	virtual void DrawRect(Point& location, Dimensions& dimensions, TColor fillColor) const = 0;

	virtual void DrawLine(Point& startLocation, Point& endLocation, TColor lineColor) const = 0;
};

class Ra8875DeviceContext 
	: public IDeviceContext
{
public:
	Ra8875DeviceContext(RA8875& ra8875): _ra8875(ra8875) {
		
	}

	void WriteText(Point& location, const char* text) const override
	{
		_ra8875.setCursor(
			location.X,
			location.Y
		);
		_ra8875.println(text);
	}

	Dimensions GetFontDimensions() const override
	{
		Dimensions dims = Dimensions(_ra8875.getFontWidth(), _ra8875.getFontHeight());
		return dims;
	}

	void DrawBorder(Point& location, Dimensions& dimensions, TColor borderColor) const override
	{
		_ra8875.drawRect(location.X, location.Y, dimensions.Width, dimensions.Height, borderColor);
	}
	void DrawRect(Point& location, Dimensions& dimensions, TColor fillColor) const override
	{
		_ra8875.fillRect(location.X, location.Y, dimensions.Width, dimensions.Height, fillColor);
	}
	void DrawLine(Point& startLocation, Point& endLocation, TColor lineColor) const override
	{
		_ra8875.drawLine(startLocation.X, startLocation.Y, endLocation.X, endLocation.Y, lineColor);
	}
private:
	RA8875& _ra8875;
};

class Widget
{
public:
	explicit Widget(IDeviceContext& tft, Point location)
		: _topLeft(location), _dc(tft)
	{
		
	}

	virtual ~Widget();

	virtual void Draw() = 0;
protected:
	inline IDeviceContext& Dc() const { return _dc; }
	Point _topLeft;

private:

	IDeviceContext& _dc;
};

class BoundingBoxWidget : public Widget
{
protected:
	Point _bottomRight;
	Dimensions _dimensions;
public:
	BoundingBoxWidget(IDeviceContext& tft, Point location, Dimensions dimensions)
		: Widget(tft, location), _dimensions(dimensions)
	{
		_bottomRight = Point(_topLeft.X + _dimensions.Width, _topLeft.Y + _dimensions.Height);
	}

};

class Label : public BoundingBoxWidget
{
private:
	const char* _label;
public:
	explicit Label(IDeviceContext& dc, Point location, Dimensions dimensions, const char* label)
		: BoundingBoxWidget(dc,  location, dimensions), _label(label)
	{
	}

	void Draw() override
	{
		auto fontDimensions = Dc().GetFontDimensions();

		// calculate whole width of label
		uint8_t textWidth = strlen(_label) * fontDimensions.Width;


		auto location = Point(
			(_topLeft.X + (_dimensions.Width / 2)) - (textWidth / 2),
			(_topLeft.Y + (_dimensions.Height / 2)) - (fontDimensions.Height / 2)
		);
		Dc().WriteText(location,_label);
	}	
};

class Button : public BoundingBoxWidget
{
private:
	void DrawWorker(bool pressed);
	Label _label;

public:
	Button(IDeviceContext& dc, Point location, Dimensions dimensions, const char* label)
		: BoundingBoxWidget(dc, location, dimensions), _label(dc, location, dimensions, label)
	{
		
	}

	void Draw() override;

	void DrawPressed()
	{
		DrawWorker(true);
	}

	bool IsHit(Point& hitpoint) const
	{
		return ((hitpoint.X >= _topLeft.X) && (hitpoint.X <= _bottomRight.X)) && ((hitpoint.Y >= _topLeft.Y) && (hitpoint.Y <= _bottomRight.Y));
	}
};
