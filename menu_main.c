#include "scr.h"
#include "draw.h"
#include "menu.h"
#include "data.h"

void MenuMain_Up()
{
	BitNumPrev(&scr_line_sel, menu_main_line_cfg);
}

void MenuMain_Down()
{

}

void MenuMain_Enter()
{
	switch (scr_line_sel)
	{
	case MENU_MAIN_SETPOINT:
		MenuSetpoint_Start(0);
		break;
	case MENU_MAIN_SET:
		MenuSet_Start(0);
		break;
	case MENU_MAIN_CONFIG:
		MenuConfig_Start(0);
		break;
	}
}

void MenuMain_Exit()
{
	ScrMain_Start();
}
