#pragma once

#include "main.h"

#define MEM_PAGE_DATA		0
#define MEM_PAGE_TEXT		1
#define MEM_ADR_DATA		((uint32_t)0x0803F000)
#define MEM_ADR_TEXT		((uint32_t)0x0803F800)

#define MENU_MAX_TEXT_SET	20
#define MENU_MAX_TEXT_LOG	20
#define MENU_LINE_LENGTH	20

#define MEM_CNT_DATA		40
#define MEM_CNT_TEXT		(MENU_MAX_TEXT_SET + MENU_MAX_TEXT_SET + MENU_MAX_TEXT_LOG) * MENU_LINE_LENGTH / 2

#define MEM_MENU_ADR_SET1	0
#define MEM_MENU_ADR_SET2	MEM_MENU_ADR_SET1 + (MENU_MAX_TEXT_SET * MENU_LINE_LENGTH / 2)
#define MEM_MENU_ADR_LOG	MEM_MENU_ADR_SET2 + (MENU_MAX_TEXT_SET * MENU_LINE_LENGTH / 2)

#define FLASH_KEY1      	((uint32_t)0x45670123)
#define FLASH_KEY2      	((uint32_t)0xCDEF89AB)

uint16_t MemRead(uint8_t page, uint16_t adr);
void MemWrite(uint8_t page, uint16_t adr, uint16_t data);
void MemWriteText(uint8_t page, uint16_t adr, uint8_t *text, uint16_t cnt);
void MemReWrite(uint8_t page, uint16_t *data);
void MemErase(uint8_t page);
