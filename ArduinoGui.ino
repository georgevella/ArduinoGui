#include "GfxUtils.h"
#define ESP8266
#define SUMOTOY_LIB

#include <SPI.h>

#ifdef defined(BDLIB)
//#include <BuyDisplay_RA8875/BD_RA8875.h>
#define touched				touchDetect
#elif defined(ADAFRUIT_LIB)
//#include <Adafruit_RA8875/Adafruit_RA8875.h>
//#include <Adafruit_GFX/Fonts/FreeMono12pt7b.h >
#define touchReadPixel		touchRead
#elif defined(SUMOTOY_LIB)
#include <RA8875/RA8875.h>
#include "RA8875/fonts/minipixel_24.c"//minipixel
#include "RA8875/fonts/squarefont_14.c"//minipixel
#include "RA8875/fonts/squarefuture_20.c"//minipixel
#include "RA8875/fonts/squareone_14.c"//minipixel
#include "RA8875/fonts/squareone_24.c"//minipixel
#include "RA8875/fonts/rubic_36.c"//minipixel

#define fillScreen			fillWindow
#endif

#include "GfxUtils.h"
#include "Widget.h"


#define RA8875_INT	D2
#define RA8875_CS	D0 

#define RA8875_RESET D1

// setup instance of RA8875
#if ADAFRUIT_LIB
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
#else
RA8875 tft = RA8875(RA8875_CS, RA8875_RESET);
#endif 

Ra8875DeviceContext dc = Ra8875DeviceContext(tft);
Ra8875TouchContext tc = Ra8875TouchContext(tft, RA8875_INT);
Window window = Window(dc, tc);

uint16_t tx, ty;
bool touched = false;
bool pendingTouchAction = false;
uint16_t pendingTouchActionDelay = 0;

// UI items
Label lblMinipixel(dc, Point(150, 30), "minipixel24");
Label lblSquareFont(dc, Point(150, 60), "squarefont14");
Label lblSquareFuture(dc, Point(150, 90), "squareFuture20");
Label lblSquareOne14(dc, Point(300, 30), "squareOne14");
Label lblSquareOne24(dc, Point(300, 60), "squareOne24");
Label lblRubic36(dc, Point(300, 90), "rubic36");
Button btn1(dc, Point(20, 40), Dimensions(100, 60), "Btn1");

//
//void drawButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* label, bool pressed)
//{
//	tft.fillRect(x, y, w, h, MakeColour(0xea));
//
//	uint16_t color1 = (pressed) ? MakeColour(0x9) : MakeColour(0xf0);
//	uint16_t color2 = (pressed) ? MakeColour(0xf0) : MakeColour(0x9);
//
//	uint8_t textWidth = strlen(label) * tft.getFontWidth();
//
//	tft.setCursor(
//		(x + (h / 2)) - (tft.getFontHeight() / 2),
//		(y + (w / 2)) - (textWidth / 2)
//	);
//	tft.println(label);
//
//	tft.drawLine(x, y, x, y + h, color1);
//	tft.drawLine(x, y, x + w, y, color1);
//
//	tft.drawLine(x, y + h, x + w, y + h, color2);
//	tft.drawLine(x + w, y, x + w, y + h, color2);	
//}

void drawColourPalette()
{
	int x = 20;
	int y = 40;

	for (int r = 0; r < 0x1f; r++)
	{
		for (int g = 0; g < 0x3f; g++)
		{
			for (int b = 0; b < 0x1f; b++)
			{
				tft.drawPixel(x, y, MakeColour(r, g, b));
				x++;

				if (x >= 450)
				{
					x = 20;
					y++;
				}
			}
			yield();
		}

		y++;
		tft.drawFastHLine(0, y, 480, RA8875_BLACK);
		y++;
	}
}

void setup()
{
	Serial.begin(38400);
	long unsigned debug_start = millis();
	while (!Serial && ((millis() - debug_start) <= 5000));

	Serial.println("Hello");

	tft.begin(RA8875_480x272);

	// start polling for touch
	tc.Begin();

//	tft.fillScreen(RA8875_BLACK);
//	// fill screen with a grayish tint
//	tft.fillScreen(MakeColour(0x1, 0x1, 0x1));
//
//	// draw titlebar
//	tft.fillRect(0, 0, 480, 16, MakeColour(0xf0, 0xf0, 0));
//
//
//
//	tft.setTextColor(RA8875_RED);
//	tft.setCursor(0, 0);
//
//#if defined(BDLIB)
//	tft.changeMode(TEXT);
//	tft.print("Test Titlebar");
//	tft.changeMode(GRAPHIC);
//#endif
//
//#if defined(ADAFRUIT_LIB)
//	tft.setFont(&FreeMono12pt7b);
//	tft.write("Test Titlebar", strlen("Test Titlebar"));
//#endif
//
//#if defined(SUMOTOY_LIB)
//	tft.print("Test Titlebar");
//#endif

	// setup widgets
	lblMinipixel.SetFont(&minipixel_24);
	lblSquareFont.SetFont(&squarefont_14);
	lblSquareFuture.SetFont(&squarefuture_20);
	lblSquareOne14.SetFont(&SquareOne_14);
	lblSquareOne24.SetFont(&SquareOne_24);
	lblRubic36.SetFont(&Rubic_36);

	window.AddWidget(&lblMinipixel);
	window.AddWidget(&lblSquareFont);
	window.AddWidget(&lblSquareFuture);
	window.AddWidget(&lblSquareOne14);
	window.AddWidget(&lblSquareOne24);
	window.AddWidget(&lblRubic36);
	window.AddWidget(&btn1);

	window.Draw();

	tft.brightness(128);

	//tft.touched();
#if defined(SUMOTOY_LIB)
	tft.enableISR();
#endif
}

bool firstTime = true;
char hitpointLocationStr[50] = { 0 };


void loop()
{	
	delay(50);

	if (firstTime)
	{
		firstTime = !firstTime;
		Serial.write("looping...");
	}

	if (!touched)
	{		
		touched = tft.touched();
		if (touched)
		{
			Serial.println("!touched -> touched");
		}
		tft.touchReadPixel(&tx, &ty);
		tx = tx; ty = ty;

		pendingTouchAction = false;
	}
	else
	{		
		bool stillDown = tft.touched();
		if (!stillDown)
		{
			Serial.println("touched -> !touched");
			sprintf(hitpointLocationStr, "Loc: X[%d]Y[%d]", tx, ty);
			Serial.println(hitpointLocationStr);

			touched = false;
			pendingTouchAction = true;
			Point hitPoint(tx, ty);
			if (btn1.IsHit(hitPoint))
			{
				btn1.DrawPressed();
			}
		}
		else {
			tft.touchReadPixel(&tx, &ty);
			tx = tx; ty = ty;
			sprintf(hitpointLocationStr, "Loc: X[%d]Y[%d]", tx, ty);
			Serial.println(hitpointLocationStr);
		}
	}
}
