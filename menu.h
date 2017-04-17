#pragma once

#include "main.h"
#include "draw.h"
#include "memory.h"

#define MENU_MAIN_MAIN			0
#define MENU_MAIN_SET1			1
#define MENU_MAIN_SET2			2
#define MENU_MAIN_LOG			3
#define MENU_MAIN_TIMER			4
#define MENU_MAIN_TIME			5
#define MENU_MAIN_CONFIG		6

#define MENU_COUNT_MAIN			6
#define MENU_COUNT_CONFIG		5
#define MENU_COUNT_TIMER		7

#define MENU_SCR_TITLE_X		10
#define MENU_SCR_TITLE_Y		6
#define MENU_SCR_LIST_X			60
#define MENU_SCR_LIST_Y			40
#define MENU_SCR_LIST_D			20
#define MENU_SCR_VALUE_X		MENU_SCR_LIST_X + MENU_LINE_LENGTH * ASCII_W

extern const char menu_lines_main[];
extern const char menu_lines_config[];
extern const char menu_lines_timer[];

char menu_lines_tmp[MENU_MAX_TEXT_SET * MENU_LINE_LENGTH];

typedef struct
{
	uint8_t		id;

	uint16_t	year;
	uint8_t		month;
	uint8_t		day;
	uint8_t		h;
	uint8_t		m;
	uint8_t		s;
} MenuLog;
MenuLog Menu_Log[20];

uint16_t menu_main_cfg;
uint16_t menu_count_set1;
uint16_t menu_count_set2;

void Menu_Init();
