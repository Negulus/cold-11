#include "scr.h"
#include "menu.h"
#include "data.h"
#include "memory.h"

void ScrMenu_Log_Update()
{
	uint8_t i = 0;
	uint8_t y = 0;

	if (!scr_menu_init)
	{
		DataImgLCD(zt_main_bg, 0, 0);
		DataTextLCD_I_L(&menu_lines_main[MENU_MAIN_LOG * 20], 16, COLOR_WHITE, zt_main_bg, MENU_SCR_TITLE_X, MENU_SCR_TITLE_Y);

		for (i = 0; i < MENU_MAX_TEXT_LOG * MENU_LINE_LENGTH / 2; i++)
		{
			menu_lines_tmp[i * 2] = (MemRead(MEM_PAGE_TEXT, MEM_MENU_ADR_SET1 + i) >> 8) & 0xFF;
			menu_lines_tmp[i * 2 + 1] = MemRead(MEM_PAGE_TEXT, MEM_MENU_ADR_SET1 + i) & 0xFF;
		}
	}

	if (scr_menu_line_start != scr_menu_line_start_1 || !scr_menu_init)
	{
		y = MENU_SCR_LIST_Y;

		if (Menu_Log[scr_menu_line_start].id != 0)
		{
			//Номер события
			DataTextLCD_I_L("Событие №", 9, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X, y);
			DataNumLCD_I_L((scr_menu_line_start + 1), 2, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_BIG_W * 9), y);

			y += 30;

			//Событие
			DataTextLCD_I_L(&menu_lines_tmp[(Menu_Log[scr_menu_line_start].id - 1) * MENU_LINE_LENGTH], MENU_LINE_LENGTH, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X, y);

			y += 40;

			//Дата
			DataTextLCD_I("Дата:    .  .", 13, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X, y);
			DataNumLCD_I(Menu_Log[scr_menu_line_start].day, 2, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_W * 7), y);
			DataNumLCD_I(Menu_Log[scr_menu_line_start].month, 2, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_W * 10), y);
			DataNumLCD_I(Menu_Log[scr_menu_line_start].year, 4, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_W * 13), y);

			y += 25;

			//Время
			DataTextLCD_I("Время:   :  :", 13, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X, y);
			DataNumLCD_I(Menu_Log[scr_menu_line_start].h, 2, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_W * 7), y);
			DataNumLCD_I(Menu_Log[scr_menu_line_start].m, 2, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_W * 10), y);
			DataNumLCD_I(Menu_Log[scr_menu_line_start].s, 2, 0, false, true, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X + (ASCII_W * 13), y);
		}

		scr_menu_line_start = scr_menu_line_start_1;
	}

	scr_menu_init = true;
}

void ScrMenu_Log_Enter()
{

}

void ScrMenu_Log_Exit()
{

}

void ScrMenu_Log_Clear()
{

}

void ScrMenu_Log_Up()
{

}

void ScrMenu_Log_Down()
{

}
