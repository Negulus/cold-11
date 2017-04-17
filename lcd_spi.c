#include "lcd_spi.h"
#include "data.h"

void LCDSPI_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	LCDSPICSDis();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CRCPolynomial = 0;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//SPI TX
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)(&lcd_spi_buf);
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_BufferSize = 8;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(DMA1_Channel3, &DMA_InitStruct);

	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
	NVIC_EnableIRQ(SPI1_IRQn);

	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}

//SPI TX
void DMA1_Channel3_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	LCDSPICSDis();
	LedOff(LED_RED);

	Data_Data.busy = false;
}

void SPI1_IRQHandler()
{
	if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		return;

	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) != RESET)
	{
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
		Data_Data.busy = false;
	}
}

void LCDSPI_Write(uint8_t reg, uint8_t data)
{
	LCDSPICSEn();
	LedOn(LED_RED);

	lcd_spi_buf[0] = reg;
	lcd_spi_buf[1] = data;

//	DMA_SetCurrDataCounter(DMA1_Channel3, 2);
//    DMA_Cmd(DMA1_Channel3, ENABLE);

	Data_Data.busy = true;
	SPI_I2S_SendData(SPI1, (uint16_t)((reg << 8) | data));
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
	while (Data_Data.busy);
	LedOff(LED_RED);
	LCDSPICSDis();
}

void LCDSPICSEn()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void LCDSPICSDis()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}
