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

	// setup widgets

#define X_GAP 15
#define Y_GAP 15

	Dimensions d(100, 60);

	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			Point p(10 + (x * (d.Width + X_GAP)), 10 + (y * (d.Height + Y_GAP)));

			auto btn = new Button(dc, p, d, "Button");
			btn->Content()->Font(&minipixel_24);
			btn->BackgroundColor(MakeColour(0xea));
			btn->ForegroundColor(MakeColour(0));
			window.AddWidget(btn);
		}

		auto timeWidget = new Label(dc, Point(240, 10), Dimensions(240, 60), "22:40");
		timeWidget->Content().Font(&squarefuture_20);
		window.AddWidget(timeWidget);
	}	

	window.Draw();

	tft.drawFastVLine(240, 10, 262, MakeColour(0xe0));	

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

	if (firstTime)
	{
		firstTime = !firstTime;
		Serial.write("looping...");
	}

	delay(50);
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
			//if (btn1.IsHit(hitPoint))
			//{
			//	btn1.DrawPressed();
			//}
		}
		else {
			tft.touchReadPixel(&tx, &ty);
			tx = tx; ty = ty;
			sprintf(hitpointLocationStr, "Loc: X[%d]Y[%d]", tx, ty);
			Serial.println(hitpointLocationStr);
		}
	}
}
