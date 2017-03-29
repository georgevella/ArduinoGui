#pragma once

#include <SPI.h>
#include <RA8875/RA8875.h>
#include <vector>
#include "GfxUtils.h"

class Widget;
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

class ITouchContext
{
public:
	virtual ~ITouchContext() = default;

	virtual bool Touched() const = 0;
	virtual Point GetTouchCoordinates() const = 0;
	virtual void TouchEnabled(bool value) = 0;
	virtual bool TouchEnabled() const = 0;
};

class Ra8875DeviceContext 
	: public IDeviceContext
{
public:
	explicit Ra8875DeviceContext(RA8875& ra8875): _ra8875(ra8875) {
		
	}

	void WriteText(Point& location, const char* text, TColor textColor) const override;

	Dimensions GetFontDimensions() const override;

	void DrawBorder(Point& location, Dimensions& dimensions, TColor borderColor) const override;

	void DrawRect(Point& location, Dimensions& dimensions, TColor fillColor) const override;

	void DrawLine(Point& startLocation, Point& endLocation, TColor lineColor) const override;

	void SetFont(const tFont* font) const override;
	void ResetFont() const override;
	void FillScreen(TColor fillColor) const override;
private:
	RA8875& _ra8875;
};

class Ra8875TouchContext 
	: public ITouchContext
{
public:
	explicit Ra8875TouchContext(RA8875& ra8875, uint8_t touchInteruptPin): _ra8875(ra8875), _touchEnabled(true), _touch_interupt_pin(touchInteruptPin)
	{

	}

public:
	void Begin() const;

	bool Touched() const override;
	Point GetTouchCoordinates() const override;
	void TouchEnabled(bool value) override;
	bool TouchEnabled() const override;

private:	
	RA8875& _ra8875;
	bool _touchEnabled;
	uint8_t _touch_interupt_pin;
};

class Window
{
private:
	bool _showTitleBar;
	IDeviceContext& _deviceContext;
	ITouchContext& _touch_context;
	TColor _windowBackground;
	TColor _titlebarForgroundColor;
	TColor _titlebarBackgroundColor;
	char* _titlebarText = "";
	std::vector<Widget*> _widgets;
public:
	explicit Window(IDeviceContext& deviceContext, ITouchContext& touchContext) :
		_showTitleBar(false),
		_deviceContext(deviceContext),
		_touch_context(touchContext),
		_windowBackground(MakeColour(0x1, 0x1, 0x1)),
		_titlebarForgroundColor(RA8875_RED), 
		_titlebarBackgroundColor(MakeColour(0xf0, 0xf0, 0))
	{
	}

	~Window()
	{
		
	}

	inline void TitlebarVisible(bool value) { _showTitleBar = value; }
	inline bool TitlebarVisible() const { return _showTitleBar; }

	void AddWidget(Widget* widget);
	void RemoveWidget(Widget& widget);

	void Draw() const;

	void CheckForTouchEvent();
	void WindowDrawLoop();
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
	virtual bool NeedsRedraw() const;
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
	const tFont* _font;
	bool _centered;
	TColor _foregroundColor;
public:
	explicit Label(IDeviceContext& dc, Point location, Dimensions dimensions, const char* label)
		: BoundingBoxWidget(dc, location, dimensions), _label(label), _font(nullptr), _centered(true), _foregroundColor(MakeColour(0xff))
	{
	}

	explicit Label(IDeviceContext& dc, Point location, const char* label)
		: BoundingBoxWidget(dc, location, Dimensions(UINT16_MAX)), _label(label), _font(nullptr), _centered(false), _foregroundColor(MakeColour(0xff))
	{
	}

	void Draw() override;

	inline void SetFont(const tFont* font)
	{
		_font = font;
	}

	inline void Color(TColor color) { _foregroundColor = color; }
	inline TColor Color() const { return _foregroundColor; }

	inline void DrawCentered(bool centered)
	{
		_centered = centered;
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
