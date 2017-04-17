#include "draw.h"
#include "ascii.h"
#include "data.h"
#include "rom.h"
#include "ram.h"
#include "lcd.h"

void DrawText(char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint8_t type, uint16_t x, uint8_t y)
{
	uint8_t text_b = ASCII_W / 2;

	uint32_t adr;
	int8_t h;
	int8_t c;
	int8_t w;
	uint32_t m;
	int8_t tmp;

	uint16_t l = ASCII_W * length;

	float a;
	float r, g, b;
	float r1 = (float)((color & 0b1111100000000000) >> 11) / (float)0b11111;
	float g1 = (float)((color & 0b0000011111100000) >> 5) / (float)0b111111;
	float b1 = (float)((color & 0b0000000000011111)) / (float)0b11111;

	if (type == DRAW_TEXT_BG)
		color_bg *= 3;

	//Проход по каждой строке
	for (h = 0; h < ASCII_H; h++)
	{
		adr = ((y + h) * 640) + ((320 - x - l)) * 2;

		//Получение фона
		switch (type)
		{
		case DRAW_TEXT_COLOR:
			for (m = 0; m < l; m++)
				mem_buf[m] = color_bg;
			break;
		case DRAW_TEXT_ALPHA:
			Data_Data.busy = true;
			RAMRead(adr, length * text_b);
			while (Data_Data.busy);
			break;
		case DRAW_TEXT_BG:
			Data_Data.busy = true;
			ROMRead(images[color_bg] + (y + h) * images[color_bg + 1] * 2 + (images[color_bg + 1] - x - l) * 2, l * 2);
			while (Data_Data.busy);
			break;
		}

		m = (type == DRAW_TEXT_BG) ? 2 : 0;

		//Проход по каждому символу
		for (c = length - 1; c >= 0 ; c--)
		{
			//Проход по каждому байту символа
			for (w = text_b - 1; w >= 0; w--)
			{
				tmp = ascii[((text[c] - 32) * text_b * ASCII_H) + (h * text_b) + w] & 0x0f;
				a = (float)tmp / (float)0x0f;
				r = (float)((mem_buf[m] & 0b1111100000000000) >> 11) /  (float)0b11111;
				g = (float)((mem_buf[m] & 0b0000011111100000) >> 5) /  (float)0b111111;
				b = (float)((mem_buf[m] & 0b0000000000011111)) /  (float)0b11111;
				r = ((r * a) + (r1 * (1 - a))) * 0b11111;
				g = ((g * a) + (g1 * (1 - a))) * 0b111111;
				b = ((b * a) + (b1 * (1 - a))) * 0b11111;
				mem_buf[m++] = ((uint16_t)r << 11) | ((uint16_t)g << 5) | (uint16_t)b;

				tmp = ascii[((text[c] - 32) * text_b * ASCII_H) + (h * text_b) + w] >> 4;
				a = (float)tmp / (float)0x0f;
				r = (float)((mem_buf[m] & 0b1111100000000000) >> 11) /  (float)0b11111;
				g = (float)((mem_buf[m] & 0b0000011111100000) >> 5) /  (float)0b111111;
				b = (float)((mem_buf[m] & 0b0000000000011111)) /  (float)0b11111;
				r = ((r * a) + (r1 * (1 - a))) * 0b11111;
				g = ((g * a) + (g1 * (1 - a))) * 0b111111;
				b = ((b * a) + (b1 * (1 - a))) * 0b11111;
				mem_buf[m++] = ((uint16_t)r << 11) | ((uint16_t)g << 5) | (uint16_t)b;
			}
		}
		RAMWriteFull(adr, length * text_b, (type == DRAW_TEXT_BG) ? 4 : 0);
		while (Data_Data.busy);
	}
}

void DrawTextL(char *text, uint8_t length, uint16_t color, uint16_t color_bg, uint8_t type, uint16_t x, uint8_t y)
{
	uint8_t text_b = ASCII_BIG_W / 2;

	uint32_t adr;
	int8_t h;
	int8_t c;
	int8_t w;
	uint32_t m;
	int8_t tmp;

	uint16_t l = ASCII_BIG_W * length;

	float a;
	float r, g, b;
	float r1 = (float)((color & 0b1111100000000000) >> 11) / (float)0b11111;
	float g1 = (float)((color & 0b0000011111100000) >> 5) / (float)0b111111;
	float b1 = (float)((color & 0b0000000000011111)) / (float)0b11111;

	if (type == DRAW_TEXT_BG)
		color_bg *= 3;

	//Проход по каждой строке
	for (h = 0; h < ASCII_BIG_H; h++)
	{
		adr = ((y + h) * 640) + ((320 - x - l)) * 2;

		//Получение фона
		switch (type)
		{
		case DRAW_TEXT_COLOR:
			for (m = 0; m < l; m++)
				mem_buf[m] = color_bg;
			break;
		case DRAW_TEXT_ALPHA:
			Data_Data.busy = true;
			RAMRead(adr, length * text_b);
			while (Data_Data.busy);
			break;
		case DRAW_TEXT_BG:
			Data_Data.busy = true;
			ROMRead(images[color_bg] + (y + h) * images[color_bg + 1] * 2 + (images[color_bg + 1] - x - l) * 2, l * 2);
			while (Data_Data.busy);
			break;
		}

		m = (type == DRAW_TEXT_BG) ? 2 : 0;
		//Проход по каждому символу
		for (c = length - 1; c >= 0 ; c--)
		{
			//Проход по каждому байту символа
			for (w = text_b - 1; w >= 0; w--)
			{
				tmp = ascii_big[((text[c] - 32) * text_b * ASCII_BIG_H) + (h * text_b) + w] & 0x0f;
				a = (float)tmp / (float)0x0f;
				r = (float)((mem_buf[m] & 0b1111100000000000) >> 11) /  (float)0b11111;
				g = (float)((mem_buf[m] & 0b0000011111100000) >> 5) /  (float)0b111111;
				b = (float)((mem_buf[m] & 0b0000000000011111)) /  (float)0b11111;
				r = ((r * a) + (r1 * (1 - a))) * 0b11111;
				g = ((g * a) + (g1 * (1 - a))) * 0b111111;
				b = ((b * a) + (b1 * (1 - a))) * 0b11111;
				mem_buf[m++] = ((uint16_t)r << 11) | ((uint16_t)g << 5) | (uint16_t)b;

				tmp = ascii_big[((text[c] - 32) * text_b * ASCII_BIG_H) + (h * text_b) + w] >> 4;
				a = (float)tmp / (float)0x0f;
				r = (float)((mem_buf[m] & 0b1111100000000000) >> 11) /  (float)0b11111;
				g = (float)((mem_buf[m] & 0b0000011111100000) >> 5) /  (float)0b111111;
				b = (float)((mem_buf[m] & 0b0000000000011111)) /  (float)0b11111;
				r = ((r * a) + (r1 * (1 - a))) * 0b11111;
				g = ((g * a) + (g1 * (1 - a))) * 0b111111;
				b = ((b * a) + (b1 * (1 - a))) * 0b11111;
				mem_buf[m++] = ((uint16_t)r << 11) | ((uint16_t)g << 5) | (uint16_t)b;
			}
		}

		RAMWriteFull(adr, length * text_b, (type == DRAW_TEXT_BG) ? 4 : 0);
		while (Data_Data.busy);
	}
}

void DrawImage(uint8_t img, uint16_t x, uint8_t y)
{
	img *= 3;
	uint16_t w = images[img + 1];
	uint8_t h = images[img + 2];
	Data_Data.Line[0].x = 320 - x - w;
	Data_Data.Line[0].y = y;
	uint8_t i;
	for (i = 0; i < h; i++)
	{
		Data_Data.busy = true;
		mem_cnt = i;
		ROMRead(images[img] + (i * w * 2), w * 2);
		while (Data_Data.busy);
	}
}

void DrawImageErase(uint8_t img, uint8_t img_bg, uint16_t x, uint8_t y)
{
	img *= 3;
	img_bg *= 3;
	uint16_t w = images[img + 1];
	uint8_t h = images[img + 2];
	Data_Data.Line[0].x = 320 - x - w;
	Data_Data.Line[0].y = y;
	uint8_t i;
	for (i = 0; i < h; i++)
	{
		Data_Data.busy = true;
		mem_cnt = i;
		ROMRead(images[img_bg] + (((y + i) * 320) + Data_Data.Line[0].x) * 2, w * 2);
		while (Data_Data.busy);
	}
}


void DrawImageEraseWH(uint8_t w, uint8_t h, uint8_t img_bg, uint16_t x, uint8_t y)
{
	img_bg *= 3;
	Data_Data.Line[0].x = 320 - x - w;
	Data_Data.Line[0].y = y;
	uint8_t i;
	for (i = 0; i < h; i++)
	{
		Data_Data.busy = true;
		mem_cnt = i;
		ROMRead(images[img_bg] + (((y + i) * 320) + Data_Data.Line[0].x) * 2, w * 2);
		while (Data_Data.busy);
	}

	Data_Data.Line[0].x = x;
	Data_Data.Line[0].y = y;
}

void DrawNum(long num, uint8_t length, uint8_t fract, bool minus, bool zero, uint16_t color, uint16_t color_bg, uint8_t type, bool large, uint16_t x, uint8_t y)
{
	int8_t i;
	long tmp;

	if (num < 0)
		num *= -1;
	else
		minus = false;

	tmp = num;
	i = 0;
	while (tmp > 0 && i < 16)
	{
		tmp /= 10;
		i++;
	}

	if (fract > 0) i++;

	if (length <= i)
	{
		length = i;
		if (minus) length += 1;
	}

	char text[(length < 16) ? length + 1 : length];
	if (length < 16) text[length] = '^';
	tmp = num;
	bool num_draw = false;
	for (i = length - 1; i >= 0; i--)
	{
		if (tmp == 0 && num_draw)
		{
			if (zero)
			{
				if (i == 0 && minus)
					text[i] = '-';
				else
					text[i] = '0';
			}
			else
			{
				if (minus)
				{
					text[i] = '-';
					minus = false;
				}
				else
					text[i] = ' ';
			}
		}
		else
		{
			if (fract > 0 && i == (length - fract - 1))
			{
				text[i] = '.';
			}
			else
			{
				text[i] = (tmp % 10) + 48;
				tmp /= 10;
			}
			num_draw = true;
		}
	}

	if (large)
		DrawTextL(text, length, color, color_bg, type, x, y);
	else
		DrawText(text, length, color, color_bg, type, x, y);
}
