#include "scr.h"
#include "menu.h"
#include "data.h"
#include "memory.h"

void ScrMenu_Set2_Update()
{
	uint8_t i = 0;
	uint8_t y = 0;

	if (!scr_menu_init)
	{
		DataImgLCD(zt_main_bg, 0, 0);
		DataTextLCD_I_L(&menu_lines_main[MENU_MAIN_SET2 * 20], 16, COLOR_WHITE, zt_main_bg, MENU_SCR_TITLE_X, MENU_SCR_TITLE_Y);

		for (i = 0; i < menu_count_set1 * MENU_LINE_LENGTH / 2; i++)
		{
			menu_lines_tmp[i * 2] = (MemRead(MEM_PAGE_TEXT, MEM_MENU_ADR_SET2 + i) >> 8) & 0xFF;
			menu_lines_tmp[i * 2 + 1] = MemRead(MEM_PAGE_TEXT, MEM_MENU_ADR_SET2 + i) & 0xFF;
		}
	}

	if (scr_menu_line_sel != scr_menu_line_sel_1 || !scr_menu_init)
	{
		if (scr_menu_line_sel >= (scr_menu_line_start + 7))
			scr_menu_line_start = scr_menu_line_sel - 7 + 1;
		else if (scr_menu_line_sel < scr_menu_line_start)
			scr_menu_line_start = scr_menu_line_sel;

		y = MENU_SCR_LIST_Y;

		for (i = scr_menu_line_start; i < (((scr_menu_line_start + 7) <= menu_count_set2) ? (scr_menu_line_start + 7) : menu_count_set2); i++)
		{
			if (i == scr_menu_line_sel)
				DataTextLCD_C(&menu_lines_tmp[i * MENU_LINE_LENGTH], MENU_LINE_LENGTH, COLOR_WHITE, COLOR_BLACK, MENU_SCR_LIST_X, y);
			else
				DataTextLCD_I(&menu_lines_tmp[i * MENU_LINE_LENGTH], MENU_LINE_LENGTH, COLOR_BLACK, zt_main_bg, MENU_SCR_LIST_X, y);
			y += MENU_SCR_LIST_D;
		}

		scr_menu_line_sel_1 = scr_menu_line_sel;
	}

	scr_menu_init = true;
}

void ScrMenu_Set2_Enter()
{

}

void ScrMenu_Set2_Exit()
{
	ScrMenu_Start(scr_menu_main, MENU_MAIN_SET2 - 1);
}

void ScrMenu_Set2_Clear()
{

}

void ScrMenu_Set2_Up()
{
	if (scr_menu_line_sel > 0)
		scr_menu_line_sel--;
}

void ScrMenu_Set2_Down()
{
	if (scr_menu_line_sel < menu_count_set2 - 1)
		scr_menu_line_sel++;
}
