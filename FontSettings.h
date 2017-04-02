// FontSettings.h

#ifndef _FONTSETTINGS_h
#define _FONTSETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>
#include <RA8875/RA8875.h>

#include "IDeviceContext.h"

class FontSettings
{
private:
	const tFont* _font;
	bool _fontApplied;
	IDeviceContext& _dc;

public:
	explicit FontSettings(IDeviceContext& dc)
		: _font(nullptr), _fontApplied(false), _dc(dc)
	{
	}

	inline void Font(const tFont* font) { _font = font; }
	inline const tFont* Font() const { return _font; }

	void ApplyFont();

	void ResetFont();
};

#endif
