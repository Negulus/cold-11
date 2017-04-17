#include "data.h"
#include "usart.h"
#include "draw.h"
#include "lcd.h"
#include "rom.h"
#include "scr.h"
#include "lcd_spi.h"

void DataInit()
{
	Data_Data.busy = false;
	Data_Data.step = 1;
	Data_Data.cnt = 0;

	Data_Data.hold = false;
	Data_Data.usart_stat = 0;
	Data_Data.usart_cnt = 0;

	DataLineUpdate();
}

void DataUpdate()
{
	if (Data_Data.busy)
		return;

	Data_Data.busy = true;

	//ќчередь команд
	if (!Data_Data.hold)
	{
		switch(Data_Data.Line[0].stat)
		{
		case DATA_IMG_RAM:
			DrawImage(Data_Data.Line[0].text[0], Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_IMG_ERASE_RAM:
			DrawImageErase(Data_Data.Line[0].text[0], Data_Data.Line[0].text[1], Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_TEXT1_C_RAM:
			DrawText(Data_Data.Line[0].text, Data_Data.Line[0].length, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_COLOR, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_TEXT1_I_RAM:
			DrawText(Data_Data.Line[0].text, Data_Data.Line[0].length, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_BG, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_TEXT2_C_RAM:
			DrawTextL(Data_Data.Line[0].text, Data_Data.Line[0].length, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_COLOR, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_TEXT2_I_RAM:
			DrawTextL(Data_Data.Line[0].text, Data_Data.Line[0].length, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_BG, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_NUM1_C_RAM:
			DrawNum((Data_Data.Line[0].text[0] << 16) | (Data_Data.Line[0].text[1] << 8) | Data_Data.Line[0].text[2], Data_Data.Line[0].length, Data_Data.Line[0].text[5], Data_Data.Line[0].text[3] == 1, Data_Data.Line[0].text[4] == 1, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_COLOR, false, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_NUM1_I_RAM:
			DrawNum((Data_Data.Line[0].text[0] << 16) | (Data_Data.Line[0].text[1] << 8) | Data_Data.Line[0].text[2], Data_Data.Line[0].length, Data_Data.Line[0].text[5], Data_Data.Line[0].text[3] == 1, Data_Data.Line[0].text[4] == 1, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_BG, false, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_NUM2_C_RAM:
			DrawNum((Data_Data.Line[0].text[0] << 16) | (Data_Data.Line[0].text[1] << 8) | Data_Data.Line[0].text[2], Data_Data.Line[0].length, Data_Data.Line[0].text[5], Data_Data.Line[0].text[3] == 1, Data_Data.Line[0].text[4] == 1, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_COLOR, true, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_NUM2_I_RAM:
			DrawNum((Data_Data.Line[0].text[0] << 16) | (Data_Data.Line[0].text[1] << 8) | Data_Data.Line[0].text[2], Data_Data.Line[0].length, Data_Data.Line[0].text[5], Data_Data.Line[0].text[3] == 1, Data_Data.Line[0].text[4] == 1, Data_Data.Line[0].color, Data_Data.Line[0].color_bg, DRAW_TEXT_BG, true, Data_Data.Line[0].x, Data_Data.Line[0].y);
			while (Data_Data.busy);
			break;
		case DATA_RAM_LCD:
			LCDEn();
			LCDDraw();
			while (Data_Data.busy);
			LCDDis();
			break;
		case DATA_LCD_OFF:
			LCDSPI_Write(0x06, 0b10101110);
			while (Data_Data.busy);
			break;
		case DATA_LCD_ON:
			LCDSPI_Write(0x06, 0b10101111);
			while (Data_Data.busy);
			break;
		}
		DataLineUpdate();
	}
	//”правление по USART
	else
	{
		switch(Data_Data.usart_stat)
		{
		case DATA_CMD_HOLD:
			Data_Data.usart_stat = 0;
			USART_Data.data_tx[0] = DATA_CMD_HOLD;
			USART_Data.data_tx[1] = 0xFF;
			USARTStartTx(2, USART_ENDLINE);
		break;
		case DATA_CMD_ERASE_SECTOR:
			Data_Data.usart_stat = 0;
			if (Data_Data.hold)
			{
				if (Data_Data.usart_cnt == 8)
				{
					ROMWait();
					ROMWriteEn();
					while (Data_Data.busy);
					Data_Data.busy = true;
					uint32_t tmpl = ((uint32_t)USART_Data.data_rx[3] << 16) | ((uint32_t)USART_Data.data_rx[4] << 8) | (uint32_t)USART_Data.data_rx[5];
					ROMEraseSector(tmpl);
					while (Data_Data.busy);
					Data_Data.busy = true;
					ROMWait();
					USART_Data.data_tx[0] = DATA_CMD_ERASE_SECTOR;
					USART_Data.data_tx[1] = 0xFF;
					USARTStartTx(2, USART_ENDLINE);
				}
				else
				{
					USART_Data.data_tx[0] = DATA_CMD_ERASE_SECTOR;
					USART_Data.data_tx[1] = 0x02;
					USARTStartTx(2, USART_ENDLINE);
				}
			}
			else
			{
				USART_Data.data_tx[0] = DATA_CMD_ERASE_SECTOR;
				USART_Data.data_tx[1] = 0x01;
				USARTStartTx(2, USART_ENDLINE);
			}
		break;
		case DATA_CMD_WRITE:
			Data_Data.usart_stat = 0;
			if (Data_Data.hold)
			{
				if (Data_Data.usart_cnt == 40)
				{
					Data_Data.busy = true;
					ROMWriteEn();
					while (Data_Data.busy);
					Data_Data.busy = true;
					uint32_t tmpl = ((uint32_t)USART_Data.data_rx[3] << 16) | ((uint32_t)USART_Data.data_rx[4] << 8) | (uint32_t)USART_Data.data_rx[5];
					uint8_t i;
					for (i = 0; i < 32; i += 2)
						mem_buf[(i / 2) + 2] = (uint16_t)USART_Data.data_rx[i + 6] | (uint16_t)(USART_Data.data_rx[i + 7] << 8);
					ROMWrite(tmpl, 32);
					mem_cnt++;
					while (Data_Data.busy);
					Data_Data.busy = true;
					ROMWait();
					USART_Data.data_tx[0] = DATA_CMD_WRITE;
					USART_Data.data_tx[1] = 0xFF;
					USARTStartTx(2, USART_ENDLINE);
				}
				else
				{
					USART_Data.data_tx[0] = DATA_CMD_WRITE;
					USART_Data.data_tx[1] = 0x02;
					USARTStartTx(2, USART_ENDLINE);
				}
			}
			else
			{
				USART_Data.data_tx[0] = DATA_CMD_WRITE;
				USART_Data.data_tx[1] = 0x01;
				USARTStartTx(2, USART_ENDLINE);
			}
		break;
		case DATA_CMD_RELEASE:
			Data_Data.usart_stat = 0;
			Data_Data.hold = false;
			USART_Data.data_tx[0] = DATA_CMD_RELEASE;
			USART_Data.data_tx[1] = 0xFF;
			USARTStartTx(2, USART_ENDLINE);
		break;
		case DATA_CMD_ERASE_FULL:
			Data_Data.usart_stat = 0;
			if (Data_Data.hold)
			{
				ROMWriteEn();
				while (Data_Data.busy);
				Data_Data.busy = true;
				ROMEraseFull();
				while (Data_Data.busy);
				Data_Data.busy = true;
				ROMWait();
				USART_Data.data_tx[0] = DATA_CMD_ERASE_FULL;
				USART_Data.data_tx[1] = 0xFF;
				USARTStartTx(2, USART_ENDLINE);
			}
			else
			{
				USART_Data.data_tx[0] = DATA_CMD_ERASE_FULL;
				USART_Data.data_tx[1] = 0x01;
				USARTStartTx(2, USART_ENDLINE);
			}
		break;
		}
	}

	Data_Data.busy = false;
}

void DataRAMLCD()
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_RAM_LCD;
	Data_Data.step++;
}

void DataImgLCD(uint8_t img, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_IMG_RAM;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].text[0] = img;
	Data_Data.step++;
}

void DataImgEraseLCD(uint8_t img, uint8_t img_bg, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_IMG_ERASE_RAM;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].text[0] = img;
	Data_Data.Line[Data_Data.step].text[1] = img_bg;
	Data_Data.step++;
}
void DataTextLCD_C(const char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_TEXT1_C_RAM;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = color_bg;
	uint8_t i;
	for (i = 0; i < length; i++) Data_Data.Line[Data_Data.step].text[i] = text[i];
	Data_Data.step++;
}

void DataTextLCD_I(const char *text, uint8_t length, uint16_t color, uint8_t img, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_TEXT1_I_RAM;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = img;
	uint8_t i;
	for (i = 0; i < length; i++) Data_Data.Line[Data_Data.step].text[i] = text[i];
	Data_Data.step++;
}

void DataTextLCD_C_L(const char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_TEXT2_C_RAM;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = color_bg;
	uint8_t i;
	for (i = 0; i < length; i++) Data_Data.Line[Data_Data.step].text[i] = text[i];
	Data_Data.step++;
}

void DataTextLCD_I_L(const char *text, uint8_t length, uint16_t color, uint8_t img, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_TEXT2_I_RAM;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = img;
	uint8_t i;
	for (i = 0; i < length; i++) Data_Data.Line[Data_Data.step].text[i] = text[i];
	Data_Data.step++;
}

void DataNumLCD_C(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_NUM1_C_RAM;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = color_bg;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].text[0] = ((num >> 16) & 0xff);
	Data_Data.Line[Data_Data.step].text[1] = ((num >> 8) & 0xff);
	Data_Data.Line[Data_Data.step].text[2] = (num & 0xff);
	Data_Data.Line[Data_Data.step].text[3] = minus ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[4] = zero ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[5] = fract;
	Data_Data.step++;
}

void DataNumLCD_I(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint8_t img, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_NUM1_I_RAM;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = img;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].text[0] = ((num >> 16) & 0xff);
	Data_Data.Line[Data_Data.step].text[1] = ((num >> 8) & 0xff);
	Data_Data.Line[Data_Data.step].text[2] = (num & 0xff);
	Data_Data.Line[Data_Data.step].text[3] = minus ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[4] = zero ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[5] = fract;
	Data_Data.step++;
}

void DataNumLCD_C_L(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint16_t color_bg, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_NUM2_C_RAM;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = color_bg;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].text[0] = ((num >> 16) & 0xff);
	Data_Data.Line[Data_Data.step].text[1] = ((num >> 8) & 0xff);
	Data_Data.Line[Data_Data.step].text[2] = (num & 0xff);
	Data_Data.Line[Data_Data.step].text[3] = minus ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[4] = zero ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[5] = fract;
	Data_Data.step++;
}

void DataNumLCD_I_L(int32_t num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint8_t img, uint16_t x, uint8_t y)
{
	if (Data_Data.step > 15) return;
	Data_Data.Line[Data_Data.step].stat = DATA_NUM2_I_RAM;
	Data_Data.Line[Data_Data.step].color = color;
	Data_Data.Line[Data_Data.step].color_bg = img;
	Data_Data.Line[Data_Data.step].x = x;
	Data_Data.Line[Data_Data.step].y = y;
	Data_Data.Line[Data_Data.step].length = length;
	Data_Data.Line[Data_Data.step].text[0] = ((num >> 16) & 0xff);
	Data_Data.Line[Data_Data.step].text[1] = ((num >> 8) & 0xff);
	Data_Data.Line[Data_Data.step].text[2] = (num & 0xff);
	Data_Data.Line[Data_Data.step].text[3] = minus ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[4] = zero ? 1 : 0;
	Data_Data.Line[Data_Data.step].text[5] = fract;
	Data_Data.step++;
}

void DataLineUpdate()
{
	uint8_t i;
	for (i = 0; i < Data_Data.step - 1; i++)
		Data_Data.Line[i] = Data_Data.Line[i + 1];
	Data_Data.step--;

	if (Data_Data.step == 0)
	{
		ScrUpdate();
		DataRAMLCD();
	}
}

void DataUSARTRx()
{
	switch (USART_Data.data_rx[2])
	{
	case DATA_CMD_HOLD:
		Data_Data.usart_stat = DATA_CMD_HOLD;
		Data_Data.hold = true;
		break;
	case DATA_CMD_ERASE_FULL:
		Data_Data.usart_stat = DATA_CMD_ERASE_FULL;
		break;
	case DATA_CMD_ERASE_SECTOR:
		Data_Data.usart_stat = DATA_CMD_ERASE_SECTOR;
		Data_Data.usart_cnt = USART_Data.ctu_rx;
		break;
	case DATA_CMD_WRITE:
		Data_Data.usart_stat = DATA_CMD_WRITE;
		Data_Data.usart_cnt = USART_Data.ctu_rx;
		break;
	case DATA_CMD_RELEASE:
		Data_Data.usart_stat = DATA_CMD_RELEASE;
		break;
	default:
		USART_Data.data_tx[0] = USART_Data.data_rx[0];
		USART_Data.data_tx[1] = 0x03;
		USARTStartTx(2, USART_ENDLINE);
		break;
	}
}
