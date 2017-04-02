// 
// 
// 

#include "FontSettings.h"

void FontSettings::ApplyFont()
{
	if (_font != nullptr && !_fontApplied)
	{
		_dc.SetFont(_font);
		_fontApplied = true;
	}
}

void FontSettings::ResetFont()
{
	if (_font != nullptr && _fontApplied)
	{
		_dc.ResetFont();
		_fontApplied = false;
	}
}
