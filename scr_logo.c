#include "scr.h"
#include "draw.h"
#include "data.h"

bool scr_logo_init = false;

void ScrLogo_Update()
{
	if (!scr_logo_init)
	{
		scr_logo_init = true;
		DataImgLCD(logo, 0, 0);
	}
}
