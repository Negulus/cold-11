#include "scr.h"
#include "menu.h"
#include "data.h"
#include "memory.h"

void ScrMenu_Main_Update()
{
	uint8_t i = 0;
	uint8_t y = 0;

	if (!scr_menu_init)
	{
		DataImgLCD(zt_main_bg, 0, 0);
		DataTextLCD_I_L(&menu_lines_main[MENU_MAIN_MAIN * 20], MENU_LINE_LENGTH, COLOR_WHITE, zt_main_bg, MENU_SCR_TITLE_X, MENU_SCR_TITLE_Y);
	}

	if (scr_menu_line_sel != scr_menu_line_sel_1 || !scr_menu_init)
	{
		y = MENU_SCR_LIST_Y;

		for (i = 0; i < MENU_COUNT_MAIN; i++)
		{
			if (i == scr_menu_line_sel)
				DataTextLCD_C(&menu_lines_main[(i + 1) * MENU_LINE_LENGTH], MENU_LINE_LENGTH, COLOR_WHITE, COLOR_BLACK, MENU_SCR_LIST_X, y);
			else
				DataTextLCD_I(&menu_lines_main[(i + 1) * MENU_LINE_LENGTH], MENU_LINE_LENGTH, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X, y);

			y += MENU_SCR_LIST_D;
		}

		scr_menu_line_sel_1 = scr_menu_line_sel;
	}

	scr_menu_init = true;
}

void ScrMenu_Main_Enter()
{
	switch ((scr_menu_line_sel + 1))
	{
	case MENU_MAIN_SET1:
		ScrMenu_Start(scr_menu_set1, 0);
		break;
	case MENU_MAIN_SET2:
		ScrMenu_Start(scr_menu_set2, 0);
		break;
	case MENU_MAIN_LOG:
		ScrMenu_Start(scr_menu_log, 0);
		break;
	case MENU_MAIN_TIMER:
		ScrMenu_Start(scr_menu_timer, 0);
		break;
	case MENU_MAIN_TIME:
		ScrMenu_Start(scr_menu_time, 0);
		break;
	case MENU_MAIN_CONFIG:
		ScrMenu_Start(scr_menu_config, 0);
		break;
	}
}

void ScrMenu_Main_Exit()
{
	ScrMain_Start();
}

void ScrMenu_Main_Clear()
{

}

void ScrMenu_Main_Up()
{
	BitNumPrev(&scr_menu_line_sel, menu_main_cfg);
}

void ScrMenu_Main_Down()
{
	BitNumNext(&scr_menu_line_sel, menu_main_cfg);
}
