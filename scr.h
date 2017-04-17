#pragma once

#include "main.h"

uint8_t screen;
uint8_t screen_last;

bool scr_menu_init;

bool scr_menu_sel;
bool scr_menu_sel_1;

uint16_t scr_menu_data_tmp;
uint16_t scr_menu_data_tmp_1;

uint8_t scr_menu_line_start;
uint8_t scr_menu_line_start_1;

uint8_t scr_menu_line_sel;
uint8_t scr_menu_line_sel_1;

enum
{
	scr_logo = 0,
	scr_main_sens,
	scr_main_out,
	scr_main_set,
	scr_menu_main,
	scr_menu_set1,
	scr_menu_set2,
	scr_menu_timer,
	scr_menu_time,
	scr_menu_log,
	scr_menu_config
};

void ScrInit();
void ScrUpdate();
void ScrImage(uint8_t img, uint8_t img_bg, uint16_t x, uint8_t y, bool en, bool *save);

void ScrLogo_Update();

void ScrMain_Start();
void ScrMain_Update();
void ScrMain_But0_Click();
void ScrMain_But0_ClickL();
void ScrMain_But1_Click();
void ScrMain_But1_ClickL();
void ScrMain_But2_Click();
void ScrMain_But2_ClickL();
void ScrMain_But3_Click();
void ScrMain_But3_ClickL();
void ScrMain_But4_Click();
void ScrMain_But4_ClickL();
void ScrMain_But5_Click();
void ScrMain_But5_ClickL();

void ScrMenu_Start(uint8_t scr_new, uint8_t line);
void ScrMenu_Update();
void ScrMenu_But0_Click();
void ScrMenu_But0_ClickL();
void ScrMenu_But1_Click();
void ScrMenu_But1_ClickL();
void ScrMenu_But2_Click();
void ScrMenu_But2_ClickL();
void ScrMenu_But3_Click();
void ScrMenu_But3_ClickL();
void ScrMenu_But4_Click();
void ScrMenu_But4_ClickL();
void ScrMenu_But5_Click();
void ScrMenu_But5_ClickL();

void ScrMenu_Main_Update();
void ScrMenu_Main_Enter();
void ScrMenu_Main_Exit();
void ScrMenu_Main_Clear();
void ScrMenu_Main_Up();
void ScrMenu_Main_Down();

void ScrMenu_Set1_Update();
void ScrMenu_Set1_Enter();
void ScrMenu_Set1_Exit();
void ScrMenu_Set1_Clear();
void ScrMenu_Set1_Up();
void ScrMenu_Set1_Down();

void ScrMenu_Set2_Update();
void ScrMenu_Set2_Enter();
void ScrMenu_Set2_Exit();
void ScrMenu_Set2_Clear();
void ScrMenu_Set2_Up();
void ScrMenu_Set2_Down();

void ScrMenu_Log_Update();
void ScrMenu_Log_Enter();
void ScrMenu_Log_Exit();
void ScrMenu_Log_Clear();
void ScrMenu_Log_Up();
void ScrMenu_Log_Down();

void ScrMenu_Timer_Update();
void ScrMenu_Timer_Enter();
void ScrMenu_Timer_Exit();
void ScrMenu_Timer_Clear();
void ScrMenu_Timer_Up();
void ScrMenu_Timer_Down();

void ScrMenu_Time_Update();
void ScrMenu_Time_Enter();
void ScrMenu_Time_Exit();
void ScrMenu_Time_Clear();
void ScrMenu_Time_Up();
void ScrMenu_Time_Down();

void ScrMenu_Config_Update();
void ScrMenu_Config_Enter();
void ScrMenu_Config_Exit();
void ScrMenu_Config_Clear();
void ScrMenu_Config_Up();
void ScrMenu_Config_Down();
