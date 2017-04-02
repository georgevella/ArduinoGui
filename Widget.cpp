#include "Widget.h"
#include "GfxUtils.h"

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
	//_deviceContext.FillScreen(_windowBackground);

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

void Label::Draw()
{
	Serial.println("Label::Draw() >>");

	_fontSettings.ApplyFont();
	auto fontDimensions = Dc().GetFontDimensions();

	char log[200] = { 0 };

	size_t textLength = TextLength();
	uint16_t textWidth = 0;

	// calculate whole width of label
	if (fontDimensions.Width != 0)
	{
		sprintf(log, "Font Dimensions (%d, %d)", fontDimensions.Width, fontDimensions.Height);
		Serial.println(log);
		textWidth = textLength * fontDimensions.Width;
	}
	else
	{
		Serial.println("Calculation font dimensions ...");

		const tFont* font = _fontSettings.Font();
		if (font != nullptr)
		{
			size_t count = textLength;
			char* ptr = _text;

			do
			{				
				for (int i = 0; i< font->length;i++)
				{
					if (font->chars[i].char_code == *ptr)
					{
						textWidth += font->chars[i].image->image_width;
					}
				}
				ptr++;
			} while (--count > 0);
		}
	}

	sprintf(log, "Text Width (%d)", textWidth);
	Serial.println(log);

	uint16_t xLocation = 0, yLocation = 0;

	switch (_horizontal_alignment)
	{
	case HA_CENTRE:
		xLocation = (_topLeft.X + (_dimensions.Width / 2)) - (textWidth / 2);
		break;
	case HA_RIGHT:
		xLocation = (_topLeft.X + _dimensions.Width) - textWidth;
		break;
	case HA_LEFT:
		xLocation = _topLeft.X;
		break;
	}

	switch (_vertical_alignment)
	{
	case VA_TOP:
		yLocation = _topLeft.Y;
		break;
	case VA_MIDDLE:
		yLocation = (_topLeft.Y + (_dimensions.Height / 2)) - (fontDimensions.Height / 2);
		break;
	case VA_BOTTOM:
		yLocation = (_topLeft.Y + _dimensions.Height) - fontDimensions.Height;
		break;
	}

	auto location = Point(xLocation, yLocation);

	sprintf(log, "Drawing text '%s' @ [%d, %d] ...", _text, xLocation, yLocation);
	Serial.println(log);
	Dc().WriteText(location, _text, _foregroundColor);

	_fontSettings.ResetFont();

	Serial.println("Label::Draw() <<");
}

void Button::DrawWorker(bool pressed)
{
	Serial.println("Drawing button...");
	Dc().DrawRect(_topLeft, _dimensions, BackgroundColor());

	//auto color1 = (pressed) ? MakeColour(0x9) : MakeColour(0xf0);
	//auto color2 = (pressed) ? MakeColour(0xf0) : MakeColour(0x9);	

	_label.Draw();

	//auto bottomLeft = Point(_topLeft.X, _bottomRight.Y);
	//auto topRight = Point(_bottomRight.X, _topLeft.Y);

	//Dc().DrawLine(_topLeft, bottomLeft, color1);
	//Dc().DrawLine(_topLeft, topRight, color1);

	//Dc().DrawLine(bottomLeft, _bottomRight, color2);
	//Dc().DrawLine(topRight, _bottomRight, color2);
}

void Button::Draw()
{
	DrawWorker(false);
}
