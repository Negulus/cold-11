#include "scr.h"
#include "menu.h"
#include "data.h"
#include "memory.h"

void ScrMenu_Start(uint8_t scr_new, uint8_t line)
{
	screen = scr_new;

	scr_menu_init = false;

	scr_menu_sel = false;
	scr_menu_sel_1 = scr_menu_sel;

	scr_menu_data_tmp = 0;
	scr_menu_data_tmp_1 = scr_menu_data_tmp;

	scr_menu_line_start = 0;
	scr_menu_line_start_1 = scr_menu_line_start;

	scr_menu_line_sel = line;
	scr_menu_line_sel_1 = scr_menu_line_sel;

	menu_main_cfg = 0b111111;
	menu_count_set1 = 20;
	menu_count_set2 = 20;
}

void ScrMenu_Update()
{
	switch (screen)
	{
	case scr_menu_main:
		ScrMenu_Main_Update();
		break;
	case scr_menu_set1:
		ScrMenu_Set1_Update();
		break;
	case scr_menu_set2:
		ScrMenu_Set2_Update();
		break;
	case scr_menu_timer:
		ScrMenu_Timer_Update();
		break;
	case scr_menu_time:
		ScrMenu_Time_Update();
		break;
	case scr_menu_log:
		ScrMenu_Log_Update();
		break;
	case scr_menu_config:
		ScrMenu_Config_Update();
		break;
	}
}

//Exit
void ScrMenu_But0_Click()
{
	switch (screen)
	{
	case scr_menu_main:
		ScrMenu_Main_Exit();
		break;
	case scr_menu_set1:
		ScrMenu_Set1_Exit();
		break;
	case scr_menu_set2:
		ScrMenu_Set2_Exit();
		break;
	case scr_menu_log:
		ScrMenu_Log_Exit();
		break;
	case scr_menu_timer:
		ScrMenu_Timer_Exit();
		break;
	case scr_menu_time:
		ScrMenu_Time_Exit();
		break;
	case scr_menu_config:
		ScrMenu_Config_Exit();
		break;
	default:
		scr_menu_sel = false;
		break;
	}
}

//Reset
void ScrMenu_But0_ClickL()
{
	ScrMain_Start();
}

void ScrMenu_But1_Click()
{
}

void ScrMenu_But1_ClickL()
{
}

void ScrMenu_But2_Click()
{
}

void ScrMenu_But2_ClickL()
{
}

//Enter
void ScrMenu_But3_Click()
{
	switch (screen)
	{
	case scr_menu_main:
		ScrMenu_Main_Enter();
		break;
	case scr_menu_set1:
		ScrMenu_Set1_Enter();
		break;
	case scr_menu_set2:
		ScrMenu_Set2_Enter();
		break;
	case scr_menu_log:
		ScrMenu_Log_Enter();
		break;
	case scr_menu_timer:
		ScrMenu_Timer_Enter();
		break;
	case scr_menu_time:
		ScrMenu_Time_Enter();
		break;
	case scr_menu_config:
		ScrMenu_Config_Enter();
		break;
	}
}

void ScrMenu_But3_ClickL()
{
}

//Down
void ScrMenu_But4_Click()
{
	switch (screen)
	{
	case scr_menu_main:
		ScrMenu_Main_Down();
		break;
	case scr_menu_set1:
		ScrMenu_Set1_Down();
		break;
	case scr_menu_set2:
		ScrMenu_Set2_Down();
		break;
	case scr_menu_log:
		ScrMenu_Log_Down();
		break;
	case scr_menu_timer:
		ScrMenu_Timer_Down();
		break;
	case scr_menu_time:
		ScrMenu_Time_Down();
		break;
	case scr_menu_config:
		ScrMenu_Config_Down();
		break;
	}
}

void ScrMenu_But4_ClickL()
{
}


//Up
void ScrMenu_But5_Click()
{
	switch (screen)
	{
	case scr_menu_main:
		ScrMenu_Main_Up();
		break;
	case scr_menu_set1:
		ScrMenu_Set1_Up();
		break;
	case scr_menu_set2:
		ScrMenu_Set2_Up();
		break;
	case scr_menu_log:
		ScrMenu_Log_Up();
		break;
	case scr_menu_timer:
		ScrMenu_Timer_Up();
		break;
	case scr_menu_time:
		ScrMenu_Time_Up();
		break;
	case scr_menu_config:
		ScrMenu_Config_Up();
		break;
	}
}

void ScrMenu_But5_ClickL()
{
}
