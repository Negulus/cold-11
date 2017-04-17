#include "scr.h"
#include "data.h"
#include "menu.h"

void ScrInit()
{
	screen = 0;
	screen_last = 0;

	Menu_Init();
}

void ScrUpdate()
{
	switch (screen)
	{
	case scr_logo:
		ScrLogo_Update();
		break;
	case scr_main_sens:
		ScrMain_Update();
		break;
	case scr_main_out:
		break;
	case scr_main_set:
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_Update();
		break;
	}
}

void ScrImage(uint8_t img, uint8_t img_bg, uint16_t x, uint8_t y, bool en, bool *save)
{
	if (*save != en)
	{
		*save = en;
		if (en)
			DataImgLCD(img, x, y);
		else
			DataImgEraseLCD(img, img_bg, x, y);
	}
}
