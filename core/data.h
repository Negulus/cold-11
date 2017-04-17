#pragma once

#include "main.h"

#define DATA_RAM_LCD		1
#define DATA_IMG_RAM		2
#define DATA_IMG_ERASE_RAM	3
#define DATA_NUM1_C_RAM		4
#define DATA_NUM1_I_RAM		5
#define DATA_NUM2_C_RAM		6
#define DATA_NUM2_I_RAM		7
#define DATA_TEXT1_C_RAM	8
#define DATA_TEXT1_I_RAM	9
#define DATA_TEXT2_C_RAM	10
#define DATA_TEXT2_I_RAM	11
#define DATA_LCD_ON			12
#define DATA_LCD_OFF		13

#define DATA_CMD_HOLD			0x25
#define DATA_CMD_ERASE_SECTOR	0x35
#define DATA_CMD_WRITE			0x45
#define DATA_CMD_RELEASE		0x55
#define DATA_CMD_ERASE_FULL		0x65

typedef struct
{
	uint8_t		stat;
	uint16_t	x;
	uint8_t		y;
	uint8_t		length;
	uint16_t	color;
	uint16_t	color_bg;
	char		text[20];
} DataLine;

typedef struct
{
	bool		busy;
	bool		hold;
	uint8_t		usart_stat;
	uint8_t		usart_cnt;
	DataLine	Line[16];
	uint16_t	cnt;
	uint8_t		step;
} DataData;
DataData	Data_Data;

void DataInit();
void DataUpdate();
void DataLineUpdate();

void DataRAMLCD();
void DataImgLCD(uint8_t img, uint16_t x, uint8_t y);
void DataImgEraseLCD(uint8_t img, uint8_t img_bg, uint16_t x, uint8_t y);
void DataNumLCD_C(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y);
void DataNumLCD_I(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint8_t img, uint16_t x, uint8_t y);
void DataNumLCD_C_L(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y);
void DataNumLCD_I_L(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint8_t img, uint16_t x, uint8_t y);
void DataTextLCD_C(const char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y);
void DataTextLCD_I(const char *text, uint8_t length, uint16_t color, uint8_t img, uint16_t x, uint8_t y);
void DataTextLCD_C_L(const char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y);
void DataTextLCD_I_L(const char *text, uint8_t length, uint16_t color, uint8_t img, uint16_t x, uint8_t y);

void DataUSARTRx();
