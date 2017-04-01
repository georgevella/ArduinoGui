// 
// 
// 

#include "GfxUtils.h"

uint16_t MakeColour(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0x1f) << 11) + ((g & 0x3f) << 5) + (b & 0x1f);
}

uint16_t MakeColour(uint8_t all)
{
	return MakeColour(all, all, all);
}

void drawMonochromeCompressed(RA8875& tft, const tIcon& icon, uint16_t x, uint16_t y, uint16_t color)
{
	char log[200] = { 0 };

	uint32_t bit = 0;
	uint16_t row = 0, col = 0, p = 0;
	size_t number_of_items_per_row = icon.length / icon.width;
	/*size_t number_of_rows = isize / number_of_items_per_row;*/

	sprintf(log, "Row length: %d; Number of rows: %d", number_of_items_per_row, icon.height);
	Serial.println(log);

	for (row = 0; row < icon.height; row++)
	{
		/*		sprintf(log, "Row index: %d", row);
		Serial.println(log);	*/

		p = 0;

		for (col = 0; col < number_of_items_per_row; col++) {
			//sprintf(log, "Col index: %d", col);
			//Serial.println(log);

			bit = 0x80000000;
			auto batch = icon.imageData[(row * number_of_items_per_row) + col];
			do
			{
				if (batch & bit)
				{
					//sprintf(log, "p: %d, bit: 0x%02X (x: %d, y: %d) ON", p, bit, x + p, y + row);
					//Serial.println(log);
					tft.drawPixel(x + p, y + row, color);
				}
				//else
				//{
				//	sprintf(log, "p: %d, bit: 0x%02X (x: %d, y: %d) OFF", p, bit, x + p, y + row);
				//	Serial.println(log);
				//}
				p++;
				bit >>= 1;
			} while (bit > 0 && p < icon.width);

			yield();
		}
	}
}

void drawMonochromeCompressed(RA8875& tft, const uint32_t * image, uint32_t isize, uint16_t iwidth, uint16_t x, uint16_t y)
{
	char log[200] = { 0 };

	uint32_t bit = 0;
	uint16_t row = 0, col = 0, p = 0;
	size_t number_of_items_per_row = isize / iwidth;
	size_t number_of_rows = isize / number_of_items_per_row;

	sprintf(log, "Row length: %d; Number of rows: %d", number_of_items_per_row, number_of_rows);
	Serial.println(log);

	for (row = 0; row < number_of_rows; row++)
	{
		/*		sprintf(log, "Row index: %d", row);
		Serial.println(log);	*/

		p = 0;

		for (col = 0; col < isize / iwidth; col++) {
			//sprintf(log, "Col index: %d", col);
			//Serial.println(log);

			bit = 0x80000000;
			auto batch = image[(row * number_of_items_per_row) + col];
			do
			{
				if (batch & bit)
				{
					//sprintf(log, "p: %d, bit: 0x%02X (x: %d, y: %d) ON", p, bit, x + p, y + row);
					//Serial.println(log);
					tft.drawPixel(x + p, y + row, RA8875_WHITE);
				}
				//else
				//{
				//	sprintf(log, "p: %d, bit: 0x%02X (x: %d, y: %d) OFF", p, bit, x + p, y + row);
				//	Serial.println(log);
				//}
				p++;
				bit >>= 1;
			} while (bit > 0 && p < iwidth);

			yield();
		}
	}
}

//void drawMonochrome(const uint8_t * image, uint32_t imageBufferSize, uint16_t imageWidth, uint16_t x, uint16_t y)
//{
//	char log[200] = { 0 };
//
//	size_t number_of_rows = imageBufferSize / imageWidth;
//
//	for (size_t row = 0; row < number_of_rows; row++)
//	{
//		for (size_t col = 0; col < imageWidth; col++)
//		{
//			auto bit = image[(row * imageWidth) + col];
//			if (bit)
//			{
//				//sprintf(log, "bit: 0x%02X (x: %d, y: %d) ON", bit, x + col, y + row);
//				Serial.println(log);
//				tft.drawPixel(x + col, y + row, RA8875_WHITE);
//			}
//		}
//	}
//
//}
