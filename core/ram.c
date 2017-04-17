#include "ram.h"
#include "lcd.h"

void RAMInit()
{
	//-----------------------------------------
	//----------------GPIO Init----------------
	//-----------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	RAM_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	RAM_Data.GPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	RAM_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &RAM_Data.GPIO_Init);

	RAM_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &RAM_Data.GPIO_Init);

	RAM_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &RAM_Data.GPIO_Init);

	RAM_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &RAM_Data.GPIO_Init);

	//-----------------------------------------
	//----------------FSMC Init----------------
	//-----------------------------------------
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	RAM_Data.FSMC_TimingInit.FSMC_AccessMode = FSMC_AccessMode_A;
	RAM_Data.FSMC_TimingInit.FSMC_AddressHoldTime = 1;
	RAM_Data.FSMC_TimingInit.FSMC_AddressSetupTime = 1;
	RAM_Data.FSMC_TimingInit.FSMC_BusTurnAroundDuration = 0;
	RAM_Data.FSMC_TimingInit.FSMC_CLKDivision = 1;
	RAM_Data.FSMC_TimingInit.FSMC_DataLatency = 1;
	RAM_Data.FSMC_TimingInit.FSMC_DataSetupTime = 1;

	RAM_Data.FSMC_Init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	RAM_Data.FSMC_Init.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	RAM_Data.FSMC_Init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	RAM_Data.FSMC_Init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	RAM_Data.FSMC_Init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	RAM_Data.FSMC_Init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	RAM_Data.FSMC_Init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	RAM_Data.FSMC_Init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	RAM_Data.FSMC_Init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	RAM_Data.FSMC_Init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	RAM_Data.FSMC_Init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	RAM_Data.FSMC_Init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	RAM_Data.FSMC_Init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	RAM_Data.FSMC_Init.FSMC_ReadWriteTimingStruct = &RAM_Data.FSMC_TimingInit;
	RAM_Data.FSMC_Init.FSMC_WriteTimingStruct = &RAM_Data.FSMC_TimingInit;
	FSMC_NORSRAMInit(&RAM_Data.FSMC_Init);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

	//DMA
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    //DMA Общее
    RAM_Data.DMA_Init.DMA_DIR = DMA_DIR_PeripheralSRC;
    RAM_Data.DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    RAM_Data.DMA_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    RAM_Data.DMA_Init.DMA_Mode = DMA_Mode_Normal;
    RAM_Data.DMA_Init.DMA_Priority = DMA_Priority_High;
    RAM_Data.DMA_Init.DMA_M2M = DMA_M2M_Enable;

	//DMA: CPU -> RAM
    RAM_Data.DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&mem_buf);
    RAM_Data.DMA_Init.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    RAM_Data.DMA_Init.DMA_MemoryBaseAddr = MEM_RAM;
    RAM_Data.DMA_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;
    RAM_Data.DMA_Init.DMA_BufferSize = LCD_H_WIDTH;
	DMA_Init(DMA2_Channel1, &RAM_Data.DMA_Init);

	//DMA: RAM -> CPU
	RAM_Data.DMA_Init.DMA_PeripheralBaseAddr = MEM_RAM;
	RAM_Data.DMA_Init.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	RAM_Data.DMA_Init.DMA_MemoryBaseAddr = (uint32_t)(&mem_buf);
	RAM_Data.DMA_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	RAM_Data.DMA_Init.DMA_BufferSize = LCD_H_WIDTH;
	DMA_Init(DMA2_Channel2, &RAM_Data.DMA_Init);

	DMA_ITConfig(DMA2_Channel1, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA2_Channel2, DMA_IT_TC, ENABLE);

	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel1_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel2_IRQn;
	NVIC_Init(&NVIC_InitStruct);

//	NVIC_EnableIRQ(DMA2_Channel1_IRQn);
//	NVIC_EnableIRQ(DMA2_Channel2_IRQn);
}

void RAMWrite(uint32_t adr, uint16_t cnt)
{
	RAMWriteFull(adr, cnt, 0);
}

void RAMWriteFull(uint32_t adr, uint16_t cnt, uint8_t offset)
{
	Data_Data.busy = true;
	DMA2_Channel1->CPAR = (uint32_t)(&mem_buf) + offset;
	DMA2_Channel1->CMAR = MEM_RAM + adr;
	DMA_SetCurrDataCounter(DMA2_Channel1, cnt * 2);
	DMA_Cmd(DMA2_Channel1, ENABLE);
}

void RAMRead(uint32_t adr, uint16_t cnt)
{
	Data_Data.busy = true;
	DMA2_Channel2->CPAR = MEM_RAM + adr;
	DMA2_Channel2->CMAR = (uint32_t)(&mem_buf);
	DMA_SetCurrDataCounter(DMA2_Channel2, cnt * 2);
	DMA_Cmd(DMA2_Channel2, ENABLE);
}

//DMA: CPU -> RAM
void DMA2_Channel1_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_IT_TC1);
    DMA_Cmd(DMA2_Channel1, DISABLE);

	Data_Data.busy = false;
}

//DMA: RAM -> CPU
void DMA2_Channel2_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_IT_TC2);
    DMA_Cmd(DMA2_Channel2, DISABLE);

    switch(Data_Data.Line[0].stat)
    {
    case DATA_RAM_LCD:
        DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_PULSE);
        DMA_Cmd(DMA2_Channel3, ENABLE);
    	break;
    default:
        Data_Data.busy = false;
    	break;
    }
}
