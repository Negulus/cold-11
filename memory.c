#include "memory.h"

uint16_t MemRead(uint8_t page, uint16_t adr)
{
	uint32_t mem_adr = 0;
	switch (page)
	{
	case MEM_PAGE_DATA: mem_adr = MEM_ADR_DATA; break;
	case MEM_PAGE_TEXT: mem_adr = MEM_ADR_TEXT; break;
	default: return 0; break;
	}

	return *(__IO uint16_t*)(mem_adr + (adr * 2));
}

void MemWrite(uint8_t page, uint16_t adr, uint16_t data)
{
	uint16_t mem_cnt = 0;
	switch (page)
	{
	case MEM_PAGE_DATA: mem_cnt = MEM_CNT_DATA; break;
	case MEM_PAGE_TEXT: mem_cnt = MEM_CNT_TEXT; break;
	default: return; break;
	}

	uint16_t mem_tmp[mem_cnt];
	uint16_t i;
	for (i = 0; i < mem_cnt; i++)
	{
		if (i == adr)
			mem_tmp[i] = data;
		else
			mem_tmp[i] = MemRead(page, i);
	}
	MemReWrite(page, mem_tmp);
}

void MemWriteText(uint8_t page, uint16_t adr, uint8_t *text, uint16_t cnt)
{
	cnt /= 2;
	uint16_t mem_cnt = 0;
	switch (page)
	{
	case MEM_PAGE_DATA: mem_cnt = MEM_CNT_DATA; break;
	case MEM_PAGE_TEXT: mem_cnt = MEM_CNT_TEXT; break;
	default: return; break;
	}

	uint16_t mem_tmp[mem_cnt];
	uint16_t i;
	for (i = 0; i < mem_cnt; i++)
	{
		if (i >= adr && i < (adr + cnt))
		{
			mem_tmp[i] = (uint16_t)text[(i - adr) * 2] << 8;
			mem_tmp[i] |= (uint16_t)text[(i - adr) * 2 + 1];
		}
		else
			mem_tmp[i] = MemRead(page, i);
	}
	MemReWrite(page, mem_tmp);
}

void MemReWrite(uint8_t page, uint16_t *data)
{
	uint16_t i;
	uint16_t mem_cnt = 0;
	uint32_t mem_adr = 0;

	switch (page)
	{
	case MEM_PAGE_DATA: mem_adr = MEM_ADR_DATA; mem_cnt = MEM_CNT_DATA; break;
	case MEM_PAGE_TEXT: mem_adr = MEM_ADR_TEXT; mem_cnt = MEM_CNT_TEXT; break;
	default: return; break;
	}

	MemErase(page);
	FLASH->CR |= FLASH_CR_PG;
	for(i = 0; i < mem_cnt; i++)
	{
		while((FLASH->SR & FLASH_SR_BSY));
			*(__IO uint16_t*)(mem_adr + (i * 2)) = data[i];
	}

	FLASH->CR &= ~FLASH_CR_PG;
	FLASH->CR |= FLASH_CR_LOCK;
}

void MemErase(uint8_t page)
{
	//FLASH_Unlock
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;

	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR |= FLASH_CR_PER; //Page Erase Set
	switch (page)
	{
	case MEM_PAGE_DATA: FLASH->AR = MEM_ADR_DATA; mem_cnt = MEM_CNT_DATA; break;
	case MEM_PAGE_TEXT: FLASH->AR = MEM_ADR_TEXT; mem_cnt = MEM_CNT_TEXT; break;
	default: return; break;
	}
	FLASH->CR |= FLASH_CR_STRT; //Start Page Erase

	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear
}
