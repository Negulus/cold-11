#include "rom.h"
#include "data.h"

void ROMInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//SPI TX
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)(&mem_buf);
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_BufferSize = 8;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(DMA1_Channel5, &DMA_InitStruct);

	//SPI RX
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)(&mem_buf);
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);

	SPI_Cmd(SPI2, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_Init(&NVIC_InitStruct);

//	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
//	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

//SPI RX
void DMA1_Channel4_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	ROMCSDis();
	LedOff(LED_RED);

	if (Data_Data.hold)
	{
		Data_Data.busy = false;
	}
	else
	{
		switch (Data_Data.Line[0].stat)
		{
		case DATA_IMG_RAM:
		case DATA_IMG_ERASE_RAM:
			DMA2_Channel1->CPAR = (uint32_t)(&mem_buf) + 4;
			DMA2_Channel1->CMAR = MEM_RAM + ((((Data_Data.Line[0].y + mem_cnt) * LCD_H_WIDTH) + Data_Data.Line[0].x) * 2);
			DMA_SetCurrDataCounter(DMA2_Channel1, Data_Data.cnt / 2);
			DMA_Cmd(DMA2_Channel1, ENABLE);
			break;
		default:
			Data_Data.busy = false;
		}
	}
}

//SPI TX
void DMA1_Channel5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC5);
	DMA_Cmd(DMA1_Channel5, DISABLE);
}

void ROMWriteEn()
{
	ROMCSEn();
	LedOn(LED_RED);

	mem_buf[0] = 0x06;

	DMA_SetCurrDataCounter(DMA1_Channel5, 1);
	DMA_SetCurrDataCounter(DMA1_Channel4, 1);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

void ROMWriteDis()
{
	ROMCSEn();
	LedOn(LED_RED);

	mem_buf[0] = 0x04;

	DMA_SetCurrDataCounter(DMA1_Channel5, 1);
	DMA_SetCurrDataCounter(DMA1_Channel4, 1);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

void ROMWrite(uint32_t adr, uint16_t cnt)
{
	ROMCSEn();
	LedOn(LED_RED);

	adr &= 0xffffff;

	mem_buf[0] = (((adr >> 16) & 0xff) << 8) | 0x02;
	mem_buf[1] = (uint16_t)(((adr >> 8) & 0xff) | ((adr & 0xff) << 8));

	DMA_SetCurrDataCounter(DMA1_Channel5, cnt + 4);
	DMA_SetCurrDataCounter(DMA1_Channel4, cnt + 4);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

void ROMRead(uint32_t adr, uint16_t cnt)
{
	ROMCSEn();
	LedOn(LED_RED);

	adr &= 0xffffff;

	mem_buf[0] = (((adr >> 16) & 0xff) << 8) | 0x03;
	mem_buf[1] = (uint16_t)(((adr >> 8) & 0xff) | ((adr & 0xff) << 8));
	mem_buf[2] = 0;

	Data_Data.cnt = cnt;
	DMA_SetCurrDataCounter(DMA1_Channel5, cnt + 4);
	DMA_SetCurrDataCounter(DMA1_Channel4, cnt + 4);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

void ROMEraseSector(uint32_t adr)
{
	ROMCSEn();

	mem_buf[0] = (((adr >> 16) & 0xff) << 8) | 0x20;
	mem_buf[1] = (uint16_t)(((adr >> 8) & 0xff) | ((adr & 0xff) << 8));

	DMA_SetCurrDataCounter(DMA1_Channel5, 4);
	DMA_SetCurrDataCounter(DMA1_Channel4, 4);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
}
void ROMEraseFull()
{
	ROMCSEn();

	mem_buf[0] = 0xc7;

	DMA_SetCurrDataCounter(DMA1_Channel5, 1);
	DMA_SetCurrDataCounter(DMA1_Channel4, 1);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void ROMStat1Read()
{
	ROMCSEn();
	LedOn(LED_RED);

	mem_buf[0] = 0x05;
	mem_buf[1] = 0;

	DMA_SetCurrDataCounter(DMA1_Channel5, 2);
	DMA_SetCurrDataCounter(DMA1_Channel4, 2);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}


void ROMWait()
{
	do {
		ROMStat1Read();
		while (Data_Data.busy);
		Data_Data.busy = true;
	} while (mem_buf[0] & 0x0100);
}

void ROMCSEn()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}
void ROMCSDis()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
