#pragma once

#include "main.h"

#define COLOR_GREEN		0x07E0
#define COLOR_RED		0xF800
#define COLOR_BLUE		0x001F
#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000

#define ASCII_BIG_W	12
#define ASCII_BIG_H	24
#define ASCII_W		10
#define ASCII_H		18

#define DRAW_TEXT_COLOR	1
#define DRAW_TEXT_ALPHA	2
#define DRAW_TEXT_BG 	3

void DrawImage(uint8_t img, uint16_t x, uint8_t y);
void DrawImageErase(uint8_t img, uint8_t img_bg, uint16_t x, uint8_t y);
void DrawImageEraseWH(uint8_t w, uint8_t h, uint8_t img_bg, uint16_t x, uint8_t y);
void DrawText(char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint8_t type, uint16_t x, uint8_t y);
void DrawTextL(char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint8_t type, uint16_t x, uint8_t y);
void DrawNum(long num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint16_t color_bg, uint8_t type, bool large, uint16_t x, uint8_t y);

enum
{
	logo = 0,
	zt_main_bg = 1,
	zt_mode_vent = 2,
	zt_mode_heat = 3,
	zt_mode_cool = 4,
	zt_mode_hum = 5,
	zt_mode_dhum = 6,
};
