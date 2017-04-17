#include "menu.h"

void Menu_Init()
{
	uint8_t i;
	for (i = 0; i < MENU_MAX_TEXT_LOG; i++)
	{
		Menu_Log[i].id = 0;
		Menu_Log[i].year = 0;
		Menu_Log[i].month = 0;
		Menu_Log[i].day = 0;
		Menu_Log[i].h = 0;
		Menu_Log[i].m = 0;
		Menu_Log[i].s = 0;
	}

	Menu_Log[0].id = 1;
	Menu_Log[0].year = 2015;
	Menu_Log[0].month = 5;
	Menu_Log[0].day = 24;
	Menu_Log[0].h = 12;
	Menu_Log[0].m = 54;
	Menu_Log[0].s = 31;
}
