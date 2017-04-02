#include "FontSettings.h"
#include <SPI.h>
#include <RA8875/RA8875.h>
#include "GfxUtils.h"
#include "Widget.h"

// fonts
#include "Comfortaa_16.c"
#include "Comfortaa_16b.c"
#include "Comfortaa_32b.c"

// icons
#include "Connectivity.c"
#include "Weather_Cloudy.c"


#define RA8875_INT	D2
#define RA8875_CS	D0 

#define RA8875_RESET D1

// setup instance of RA8875
RA8875 tft = RA8875(RA8875_CS, RA8875_RESET);

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
#define Y_GAP 4

#define SCREENBORDER_Y	10
#define SCREENBORDER_X	10

#define SECTION_TIME_HEIGHT 60
#define SECTION_TIME_BOTTOM SCREENBORDER_Y + SECTION_TIME_HEIGHT + SCREENBORDER_Y

#define SECTION_NOTIFICATIONS_TOP SECTION_TIME_BOTTOM + SCREENBORDER_Y

#define SIDEBAR_WIDTH		100
#define TITLEBAR_HEIGHT		25

#define INNERQUAD_WIDTH		11
#define INNERQUAD_HEIGHT	11
#define MAX_WIDTH			(SIDEBAR_WIDTH + INNERQUAD_WIDTH)
#define MAX_HEIGHT			(TITLEBAR_HEIGHT+INNERQUAD_HEIGHT)
#define QUADRANT_WIDTH		25
#define QUADRANT_HEIGHT		25

	Dimensions d(100, 45);

	for (int x = 0; x < 1; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			//0, MAX_HEIGHT
			Point p(x * (d.Width + X_GAP), (MAX_HEIGHT + Y_GAP) + (y * (d.Height + Y_GAP)));

			auto btn = new Button(dc, p, d);
			btn->Content()->Text("ButtonX");
			btn->Content()->Font()->Font(&Comfortaa_16b);
			btn->BackgroundColor(0x42B7);
			btn->Content()->ForegroundColor(MakeColour(0));
			window.AddWidget(btn);
		}
	}

	// find midpoint
	uint16_t mid_x = tft.width() / 2;
	uint16_t mid_h = tft.height() - (2 * SCREENBORDER_Y);
	
	auto timeWidget = new Label(dc, 
		Point(mid_x+ Weather_Cloudy.width+SCREENBORDER_X, SCREENBORDER_Y), 
		Dimensions(mid_x-Weather_Cloudy.width-SCREENBORDER_X, SECTION_TIME_HEIGHT));
	timeWidget->Text("22:40");
	timeWidget->Font()->Font(&Comfortaa_32b);
	window.AddWidget(timeWidget);	

	// draw borders
	//tft.drawFastVLine(mid_x, SCREENBORDER_Y, mid_h, 0xFFDF);

	//tft.drawFastHLine(mid_x + SCREENBORDER_X, SECTION_TIME_BOTTOM, mid_x - (2 * SCREENBORDER_X), 0xFFDF);

	//drawMonochromeCompressed(tft, image_data_Connectivity, 24, 24, mid_x + SCREENBORDER_X, 80);



	tft.fillCurve(QUADRANT_WIDTH, QUADRANT_HEIGHT, QUADRANT_WIDTH, QUADRANT_HEIGHT, 1, 0xFCE0);
	tft.fillRect(0, QUADRANT_HEIGHT, QUADRANT_WIDTH, MAX_HEIGHT - QUADRANT_HEIGHT, 0xfce0);
	tft.fillRect(QUADRANT_WIDTH, 0, MAX_WIDTH - QUADRANT_WIDTH, MAX_HEIGHT, 0xFCE0);
	tft.fillCurve(MAX_WIDTH, MAX_HEIGHT, INNERQUAD_WIDTH, INNERQUAD_HEIGHT, 1, 0);

	tft.fillCurve(QUADRANT_WIDTH, tft.height() - QUADRANT_HEIGHT, QUADRANT_WIDTH, QUADRANT_HEIGHT, 0, 0xFCE0);
	tft.fillRect(0, tft.height() - QUADRANT_HEIGHT - INNERQUAD_HEIGHT, QUADRANT_WIDTH, INNERQUAD_HEIGHT, 0xfce0);
	tft.fillRect(QUADRANT_WIDTH, tft.height() - QUADRANT_HEIGHT - INNERQUAD_HEIGHT, MAX_WIDTH - QUADRANT_WIDTH, MAX_HEIGHT, 0xFCE0);
	tft.fillCurve(MAX_WIDTH, tft.height() - MAX_HEIGHT, INNERQUAD_WIDTH, INNERQUAD_HEIGHT, 0, 0);

	window.Draw();
	//tft.fillRect(0, MAX_HEIGHT, SIDEBAR_WIDTH, tft.height() - (MAX_HEIGHT * 2), 0xFCE0);

	//tft.fillRect(MAX_WIDTH, 0, tft.width() - MAX_WIDTH, TITLEBAR_HEIGHT, 0xfce0);

	drawMonochromeCompressed(tft, Connectivity, mid_x + SCREENBORDER_X, SECTION_NOTIFICATIONS_TOP, RA8875_WHITE);
	drawMonochromeCompressed(tft, Weather_Cloudy, mid_x + SCREENBORDER_X, SCREENBORDER_Y + ((SECTION_TIME_HEIGHT / 2) - (Weather_Cloudy.height / 2)), RA8875_WHITE);

	tft.brightness(128);	

	//tft.touched();
	tft.enableISR();
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
