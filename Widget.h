#pragma once

#include <SPI.h>
#include <RA8875/RA8875.h>
#include <vector>
#include "GfxUtils.h"

#include "GuiCommon.h"
#include "FontSettings.h"
#include "IDeviceContext.h"


#define MAX_TEXTLENGTH 255


class Widget;

class ITouchContext
{
public:
	virtual ~ITouchContext() = default;

	virtual bool Touched() const = 0;
	virtual Point GetTouchCoordinates() const = 0;
	virtual void TouchEnabled(bool value) = 0;
	virtual bool TouchEnabled() const = 0;
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
	explicit Widget(IDeviceContext& dc)
		: _topLeft(), _backgroundColor(0), _dc(dc)
	{
		
	}

	explicit Widget(IDeviceContext& tft, Point location)
		: _topLeft(location), _backgroundColor(0), _dc(tft)
	{
	}

	virtual ~Widget();

	virtual void Draw() = 0;
	
	virtual bool NeedsRedraw() const;

	inline void TopLeft(Point& location) { _topLeft = location; }
	inline Point& TopLeft() { return _topLeft; }

	inline void BackgroundColor(TColor color) { _backgroundColor = color; }
	inline TColor BackgroundColor() const { return _backgroundColor; }

protected:
	inline IDeviceContext& Dc() const { return _dc; }
	Point _topLeft;

private:
	TColor _backgroundColor;
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


	void Draw() override = 0;

	inline void Size(Dimensions& dims)
	{
		_dimensions = dims;
		_bottomRight = Point(_topLeft.X + _dimensions.Width, _topLeft.Y + _dimensions.Height);
	}
	inline ::Dimensions& Size() { return _dimensions; }
};

enum WidgetVerticalAlignment
{
	VA_TOP,
	VA_MIDDLE,
	VA_BOTTOM
};

enum WidgetHorizontalAlignment
{
	HA_LEFT,
	HA_CENTRE,
	HA_RIGHT
};

class Label : public BoundingBoxWidget
{
private:
	TColor _foregroundColor;
	char _text[MAX_TEXTLENGTH + 1] = { 0 };
	FontSettings _fontSettings;
	WidgetVerticalAlignment _vertical_alignment;
	WidgetHorizontalAlignment _horizontal_alignment;
public:
	explicit Label(IDeviceContext& dc, Point location, Dimensions dimensions)
		: BoundingBoxWidget(dc, location, dimensions), _foregroundColor(RA8875_WHITE), _fontSettings(dc),  _vertical_alignment(VA_MIDDLE), _horizontal_alignment(HA_CENTRE)
	{
	}
	
	explicit Label(IDeviceContext& dc)
		: BoundingBoxWidget(dc, Point(0,0), Dimensions(100,10)), _foregroundColor(RA8875_WHITE), _fontSettings(dc),  _vertical_alignment(VA_MIDDLE), _horizontal_alignment(HA_CENTRE)
	{
	}

	//explicit Label(IDeviceContext& dc, Point location)
	//	: BoundingBoxWidget(dc, location, Dimensions(UINT16_MAX)), _foregroundColor(RA8875_WHITE), _fontSettings(dc), _vertical_alignment(VA_TOP), _horizontal_alignment(HA_LEFT)
	//{
	//}	

	void Draw() override;

	inline FontSettings* Font() { return &_fontSettings; }

	inline void ForegroundColor(TColor color) { _foregroundColor = color; }
	inline TColor ForegroundColor() const { return _foregroundColor; }

	inline void VerticalAlignment(::WidgetVerticalAlignment value) { _vertical_alignment = value; }
	inline ::WidgetVerticalAlignment VerticalAlignment() const { return _vertical_alignment; }

	inline void HorizontalAlignment(WidgetHorizontalAlignment value) { _horizontal_alignment = value; }
	inline ::WidgetHorizontalAlignment HorizontalAlignment() const { return _horizontal_alignment; }

	inline const char* Text() const { return _text; }
	inline void Text(const char* text) 
	{
		if (text != nullptr)
		{
			auto ln = strlen(text);
			ets_strncpy(_text, text, ln < MAX_TEXTLENGTH ? ln : MAX_TEXTLENGTH);
		}
		else
		{
			memset(_text, 0, MAX_TEXTLENGTH);
		}
	}

	inline size_t TextLength() const { return strlen(_text); }
};

class Button : public BoundingBoxWidget
{
private:
	void DrawWorker(bool pressed);
	Label _label;

public:
	Button(IDeviceContext& dc, Point location, Dimensions dimensions)
		: BoundingBoxWidget(dc, location, dimensions), _label(dc, location, dimensions)
	{
		
	}	

	inline Label* Content() { return &_label; }

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
