#include "Widget.h"
#include "GfxUtils.h"


void Ra8875DeviceContext::WriteText(Point& location, const char* text, TColor textColor) const
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

void Ra8875DeviceContext::DrawBorder(Point& location, Dimensions& dimensions, TColor borderColor) const
{
	_ra8875.drawRect(location.X, location.Y, dimensions.Width, dimensions.Height, borderColor);
}

void Ra8875DeviceContext::DrawRect(Point& location, Dimensions& dimensions, TColor fillColor) const
{
	_ra8875.fillRect(location.X, location.Y, dimensions.Width, dimensions.Height, fillColor);
}

void Ra8875DeviceContext::DrawLine(Point& startLocation, Point& endLocation, TColor lineColor) const
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

void Ra8875TouchContext::Begin() const
{
	_ra8875.useINT(_touch_interupt_pin);
	_ra8875.touchBegin();
}

bool Ra8875TouchContext::Touched() const
{
	return _ra8875.touched();	
}

Point Ra8875TouchContext::GetTouchCoordinates() const
{
	uint16_t tx = 0, ty = 0;
	_ra8875.touchReadPixel(&tx, &ty);

	return Point(tx, ty);
}

void Ra8875TouchContext::TouchEnabled(bool value)
{
	_touchEnabled = value;
	_ra8875.touchEnable(value);
}

bool Ra8875TouchContext::TouchEnabled() const
{
	return  _touchEnabled;
}

void Window::AddWidget(Widget* widget)
{
	_widgets.push_back(widget);
}

void Window::RemoveWidget(Widget& widget)
{
}

void Window::Draw() const
{
	_deviceContext.FillScreen(_windowBackground);

	if (_showTitleBar)
	{
		Point origin = Point(0, 0);
		Dimensions titleBarDims = Dimensions(480, 16);
		_deviceContext.DrawRect(origin, titleBarDims, _titlebarBackgroundColor);
		_deviceContext.WriteText(origin, "Test Titlebar", _titlebarForgroundColor);		
	}

	std::vector<Widget*>::const_iterator it = _widgets.begin();
	for (; it != _widgets.end(); it++)
	{
		Widget* w = *it;
		w->Draw();
	}
}

void Window::CheckForTouchEvent()
{
}

void Window::WindowDrawLoop()
{
}

Widget::~Widget()
{
}

bool Widget::NeedsRedraw() const
{
	return false;
}

Caption::Caption(IDeviceContext& dc, Point location, const char* text)
	: Widget(dc, location), _font(nullptr), _foregroundColor(MakeColour(0xff))
{
	if (text != nullptr)
	{
		auto ln = strlen(text);
		ets_strncpy(_text, text, ln < MAX_TEXTLENGTH ? ln : MAX_TEXTLENGTH);
	}
}

void Caption::Draw()
{
	if (_font != nullptr)
	{
		Serial.println("Setting font ...");
		Dc().SetFont(_font);
	}

	Serial.println("Drawing text ...");
	Dc().WriteText(TopLeft(), _text, _foregroundColor);

	if (_font != nullptr)
	{
		Serial.println("Resetting font ...");
		Dc().ResetFont();
	}
}

void Label::Draw()
{
	auto fontDimensions = Dc().GetFontDimensions();

	char log[100] = { 0 };
	sprintf(log, "Font Dimensions (%d, %d)", fontDimensions.Width, fontDimensions.Height);
	Serial.println(log);

	// calculate whole width of label
	uint8_t textWidth = _caption.TextLength() * fontDimensions.Width;

	sprintf(log, "Text Width (%d)", textWidth);
	Serial.println(log);

	Point location;

	if (_centered)
	{
		location = Point(
			(_topLeft.X + (_dimensions.Width / 2)) - (textWidth / 2),
			(_topLeft.Y + (_dimensions.Height / 2)) - (fontDimensions.Height / 2)
		);
	}
	else
	{
		location = _topLeft;
	}

	_caption.TopLeft(location);
	_caption.Draw();
}

void Button::DrawWorker(bool pressed)
{
	Serial.println("Drawing button...");
	Dc().DrawRect(_topLeft, _dimensions, BackgroundColor());

	auto color1 = (pressed) ? MakeColour(0x9) : MakeColour(0xf0);
	auto color2 = (pressed) ? MakeColour(0xf0) : MakeColour(0x9);	

	_label.Draw();

	auto bottomLeft = Point(_topLeft.X, _bottomRight.Y);
	auto topRight = Point(_bottomRight.X, _topLeft.Y);

	Dc().DrawLine(_topLeft, bottomLeft, color1);
	Dc().DrawLine(_topLeft, topRight, color1);

	Dc().DrawLine(bottomLeft, _bottomRight, color2);
	Dc().DrawLine(topRight, _bottomRight, color2);
}

void Button::Draw()
{
	DrawWorker(false);
}
