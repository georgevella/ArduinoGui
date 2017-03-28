#include "Widget.h"
#include "GfxUtils.h"

Widget::~Widget()
{
}

void Button::DrawWorker(bool pressed)
{
	Dc().DrawRect(_topLeft, _dimensions, MakeColour(0xea));

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
