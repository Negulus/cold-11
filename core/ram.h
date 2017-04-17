#pragma once

#include "main.h"
#include "data.h"

typedef struct
{
	FSMC_NORSRAMInitTypeDef 		FSMC_Init;
	FSMC_NORSRAMTimingInitTypeDef 	FSMC_TimingInit;
	GPIO_InitTypeDef 				GPIO_Init;
	DMA_InitTypeDef 				DMA_Init;
} RAMData;
RAMData	RAM_Data;

void RAMInit();
void RAMWrite(uint32_t adr, uint16_t cnt);
void RAMWriteFull(uint32_t adr, uint16_t cnt, uint8_t offset);
void RAMRead(uint32_t adr, uint16_t cnt);

