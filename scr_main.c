#include "scr.h"
#include "draw.h"
#include "data.h"
#include "mb.h"
#include "zt033.h"

bool scr_main_init = false;
bool mode_vent = false;
bool mode_heat = false;
bool mode_cool = false;
bool mode_hum = false;
bool mode_dhum = false;
bool sys_off = false;
bool sys_on = false;
bool sys_blink = false;
bool sys_filter = false;
bool sys_error = false;
bool sys_run = false;
bool set_t_sel = false;
bool set_h_sel = false;
bool temp_room_sel = false;
bool temp_air_sel = false;
bool temp_floor_sel = false;
bool hum_out_sel = false;
bool hum_room_sel = false;
bool hum_air_sel = false;
uint8_t	sens_sel = 0b000001;
uint8_t	sens_sel_new = 0b000001;
uint8_t error = 0;
uint8_t set_t = 0;
uint8_t set_h = 0;
uint8_t fan_speed = 0;
uint16_t temp_room = 0;
uint16_t temp_air = 0;
uint16_t temp_floor = 0;
uint16_t hum_out = 0;
uint16_t hum_room = 0;
uint16_t hum_air = 0;

void ScrMain_Start()
{
	screen = scr_main_sens;

	scr_main_init = false;
	mode_vent = false;
	mode_heat = false;
	mode_cool = false;
	mode_hum = false;
	mode_dhum = false;
	sys_off = false;
	sys_on = false;
	sys_blink = false;
	sys_filter = false;
	sys_error = false;
	sys_run = false;
	set_t_sel = false;
	set_h_sel = false;
	temp_room_sel = false;
	temp_air_sel = false;
	temp_floor_sel = false;
	hum_out_sel = false;
	hum_room_sel = false;
	hum_air_sel = false;
	sens_sel = 0b000001;
	sens_sel_new = 0b000001;
	error = 0;
	set_t = 0;
	set_h = 0;
	fan_speed = 0;
	temp_room = 0;
	temp_air = 0;
	temp_floor = 0;
	hum_out = 0;
	hum_room = 0;
	hum_air = 0;
}

void ScrMain_Update()
{
	//Инициализация дисплея
	//Вывод фона и неменяемых надписей
	if (!scr_main_init)
	{
		scr_main_init = true;
		DataImgLCD(zt_main_bg, 0, 0);
		DataTextLCD_I_L("Система:", 8, COLOR_WHITE, zt_main_bg, 46, 5);
		DataTextLCD_I_L("Скорость:", 8, COLOR_BLACK, zt_main_bg, 58, 48);
		DataTextLCD_I_L("Уставка:", 8, COLOR_BLACK, zt_main_bg, 58, 82);
	}

	ScrImage(zt_mode_vent, zt_main_bg, 6, 32, MB_Data.Reg.reg_114 & PANEL_MODE_VENT, &mode_vent);
	ScrImage(zt_mode_heat, zt_main_bg, 6, 68, MB_Data.Reg.reg_114 & PANEL_MODE_HEAT, &mode_heat);
	ScrImage(zt_mode_cool, zt_main_bg, 6, 104, MB_Data.Reg.reg_114 & PANEL_MODE_COOL, &mode_cool);
	ScrImage(zt_mode_hum, zt_main_bg, 6, 140, MB_Data.Reg.reg_114 & PANEL_MODE_HUM, &mode_hum);
	ScrImage(zt_mode_dhum, zt_main_bg, 6, 176, MB_Data.Reg.reg_114 & PANEL_MODE_DHUM, &mode_dhum);

	//Состояния системы
	if (sys_off != (bool)(MB_Data.Reg.reg_11C & PANEL_SYS_OFF))
	{
		sys_off = (bool)(MB_Data.Reg.reg_11C & PANEL_SYS_OFF);
		if (sys_off) DataTextLCD_I_L(" ОСТАНОВ", 8, COLOR_WHITE, zt_main_bg, 204, 5);
	}

	if (sys_on != (bool)(MB_Data.Reg.reg_11C & PANEL_SYS_ON))
	{
		sys_on = (bool)(MB_Data.Reg.reg_11C & PANEL_SYS_ON);
		if (sys_on) DataTextLCD_I_L("  РАБОТА", 8, COLOR_GREEN, zt_main_bg, 204, 5);
	}

	if (sys_blink != (bool)(MB_Data.Reg.reg_11C & PANEL_SYS_BLINK))
	{
		sys_blink = (bool)(MB_Data.Reg.reg_11C & PANEL_SYS_BLINK);
		if (sys_blink) DataTextLCD_I_L("ОЖИДАНИЕ", 8, COLOR_WHITE, zt_main_bg, 204, 5);
	}

	if (sys_error != (bool)(MB_Data.Reg.reg_11C & 0b111111))
	{
		sys_error = (bool)(MB_Data.Reg.reg_11C & 0b111111);
		if (sys_error) DataTextLCD_I_L("  АВАРИЯ", 8, COLOR_RED, zt_main_bg, 204, 5);
	}

	if (sys_run != (bool)(MB_Data.Reg.reg_114 & PANEL_RUN))
	{
		sys_run = (bool)(MB_Data.Reg.reg_114 & PANEL_RUN);
		if (sys_run)
			LedOn(LED_GREEN);
		else
			LedOff(LED_GREEN);
	}

	//Скорость
	if (fan_speed != ((MB_Data.Reg.reg_114 >> 1) & 0b111))
	{
		fan_speed = ((MB_Data.Reg.reg_114  >> 1) & 0b111);
		DataNumLCD_I_L(fan_speed, 3, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 48);
	}

	//Температуры
	if (temp_room_sel != (bool)(sens_sel & 0b000001))
	{
		temp_room_sel = (bool)(sens_sel & 0b000001);
		if (temp_room_sel)
		{
			DataTextLCD_I("t в помещении:", 14, COLOR_BLACK, zt_main_bg, 58, 154);
			DataNumLCD_I(temp_room, 4, 1, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
		}
	}

	if (temp_room_sel && temp_room != 20)
	{
		temp_room = 20;
		DataNumLCD_I(temp_room, 4, 1, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
	}

	if (temp_air_sel != (bool)(sens_sel & 0b000010))
	{
		temp_air_sel = (bool)(sens_sel & 0b000010);
		if (temp_air_sel)
		{
			DataTextLCD_I("t в канале:   ", 14, COLOR_BLACK, zt_main_bg, 58, 154);
			DataNumLCD_I(temp_air, 4, 1, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
		}
	}

	if (temp_air_sel && temp_air != MB_Data.Reg.reg_117)
	{
		temp_air = MB_Data.Reg.reg_117;
		DataNumLCD_I(temp_air, 4, 1, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
	}

	if (temp_floor_sel != (bool)(sens_sel & 0b000100))
	{
		temp_floor_sel = (bool)(sens_sel & 0b000100);
		if (temp_floor_sel)
		{
			DataTextLCD_I("t обр. воды:  ", 14, COLOR_BLACK, zt_main_bg, 58, 154);
			DataNumLCD_I(temp_floor, 4, 1, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
		}
	}

	if (temp_floor_sel && temp_floor != MB_Data.Reg.reg_118)
	{
		temp_floor = MB_Data.Reg.reg_118;
		DataNumLCD_I(temp_floor, 4, 1, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
	}

	//Влажность
	if (hum_out_sel != (bool)(sens_sel & 0b001000))
	{
		hum_out_sel = (bool)(sens_sel & 0b001000);
		if (hum_out_sel)
		{
			DataTextLCD_I("h наружная:   ", 14, COLOR_BLACK, zt_main_bg, 58, 154);
			DataNumLCD_I(hum_out, 4, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
		}
	}

	if (hum_out_sel && hum_out != MB_Data.Reg.reg_119)
	{
		hum_out = MB_Data.Reg.reg_119;
		DataNumLCD_I(hum_out, 4, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
	}

	if (hum_air_sel != (bool)(sens_sel & 0b100000))
	{
		hum_air_sel = (bool)(sens_sel & 0b100000);
		if (hum_air_sel)
		{
			DataTextLCD_I("h в канале:   ", 14, COLOR_BLACK, zt_main_bg, 58, 154);
			DataNumLCD_I(hum_air, 4, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
		}
	}

	if (hum_air_sel && hum_air != MB_Data.Reg.reg_11B)
	{
		hum_air = MB_Data.Reg.reg_11B;
		DataNumLCD_I(hum_air, 4, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
	}

	if (hum_room_sel != (bool)(sens_sel & 0b010000))
	{
		hum_room_sel = (bool)(sens_sel & 0b010000);
		if (hum_room_sel)
		{
			DataTextLCD_I("h в помещении:", 14, COLOR_BLACK, zt_main_bg, 58, 154);
			DataNumLCD_I(hum_room, 4, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
		}
	}

	if (hum_room_sel && hum_room != MB_Data.Reg.reg_11A)
	{
		hum_room = MB_Data.Reg.reg_11A;
		DataNumLCD_I(hum_room, 4, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 154);
	}

	//Уставки
	if (set_t_sel != (bool)(sens_sel & 0b000111))
	{
		set_t_sel = (bool)(sens_sel & 0b000111);
		if (set_t_sel)
		{
			DataTextLCD_I_L("Температура:", 12, COLOR_BLACK, zt_main_bg, 58, 118);
			DataNumLCD_I_L(set_t, 3, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 82);
		}
	}

	if (set_t_sel && set_t != MB_Data.Reg.reg_115)
	{
		set_t = MB_Data.Reg.reg_115;
		DataNumLCD_I_L(set_t, 3, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 82);
	}

	if (set_h_sel != (bool)(sens_sel & 0b111000))
	{
		set_h_sel = (bool)(sens_sel & 0b111000);
		if (set_h_sel)
		{
			DataTextLCD_I_L("Влажность:  ", 12, COLOR_BLACK, zt_main_bg, 58, 118);
			DataNumLCD_I_L(set_h, 3, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 82);
		}
	}

	if (set_h_sel && set_h != MB_Data.Reg.reg_116)
	{
		set_h = MB_Data.Reg.reg_116;
		DataNumLCD_I_L(set_h, 3, 0, false, false, COLOR_BLACK, zt_main_bg, 246, 82);
	}
}

//Mode
void ScrMain_But0_Click()
{
	uint8_t tmp1 = (MB_Data.Reg.reg_112 >> 5) & 0b11111;
	uint8_t tmp2 = (MB_Data.Reg.reg_114 >> 5) & 0b11111;
	MB_Data.Reg.reg_114 &= ~(0b11111 << 5);

	if (tmp1 == 0) return;

	tmp2 <<= 1;
	while ((tmp2 & tmp1) == 0)
	{
		tmp2 <<= 1;
		if (tmp2 == 0) tmp2 = 1;
	}

	MB_Data.Reg.reg_114 |= (tmp2 << 5);
}

void ScrMain_But0_ClickL()
{
	ScrMenu_Start(scr_menu_main, 0);
}

//Type
void ScrMain_But1_Click()
{
	uint8_t tmp1 = (MB_Data.Reg.reg_112 >> 10) & 0b111111;

	if (tmp1 == 0)
	{
		sens_sel = 1;
	}
	else
	{
		sens_sel <<= 1;
		while ((sens_sel & tmp1) == 0)
		{
			sens_sel <<= 1;
			if (sens_sel == 0) sens_sel = 1;
		}
	}
}

void ScrMain_But1_ClickL()
{
}

//Time
void ScrMain_But2_Click()
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_LCD_OFF;
	Data_Data.step++;
}

void ScrMain_But2_ClickL()
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_LCD_ON;
	Data_Data.step++;
}

//Fan
void ScrMain_But3_Click()
{
	if (MB_Data.Reg.reg_11C & 0b111111) return;

	if (MB_Data.Reg.reg_114 & PANEL_RUN)
	{
		uint8_t tmp = (MB_Data.Reg.reg_114 >> 1) & 0b111;
		MB_Data.Reg.reg_114 &= ~(0b111 << 1);

		if (tmp < (MB_Data.Reg.reg_112 & 0b111))
			tmp++;
		else
			tmp = 1;
		MB_Data.Reg.reg_114 |=  tmp << 1;
	}
	else
		MB_Data.Reg.reg_114 |= PANEL_RUN;
}

void ScrMain_But3_ClickL()
{
	if (MB_Data.Reg.reg_11C & 0b111111) return;

	if (MB_Data.Reg.reg_114 & PANEL_RUN)
		MB_Data.Reg.reg_114 &= ~PANEL_RUN;
}

void ScrMain_But4_Click()
{
	if (sens_sel & 0b111000)
	{
		if (MB_Data.Reg.reg_116 > 0)
			MB_Data.Reg.reg_116--;
	} else
	{
		if (MB_Data.Reg.reg_115 > (MB_Data.Reg.reg_113 & 0b111111))
			MB_Data.Reg.reg_115--;
	}
}

void ScrMain_But4_ClickL()
{
}

void ScrMain_But5_Click()
{
	if (sens_sel & 0b111000)
	{
		if (MB_Data.Reg.reg_116 < 100)
			MB_Data.Reg.reg_116++;
	} else
	{
		if (MB_Data.Reg.reg_115 < ((MB_Data.Reg.reg_113 & 0b111111000000) >> 6))
			MB_Data.Reg.reg_115++;
	}
}

void ScrMain_But5_ClickL()
{
}
