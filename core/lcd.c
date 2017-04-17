#include "lcd.h"
#include "ram.h"

void LCDInit()
{
	LCD_Data.nodata = 0xffff;
	LCD_Data.stat = 0;

	LCD_Data.t_led = 0;
	LCD_Data.t_led_set = 500;
	LCD_Data.power = 0;
	LCD_Data.power_set = 50;

	//-----------------------------------------
	//----------------GPIO Init----------------
	//-----------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_14 | GPIO_Pin_15;
	LCD_Data.GPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	LCD_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &LCD_Data.GPIO_Init);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &LCD_Data.GPIO_Init);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOG, &LCD_Data.GPIO_Init);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &LCD_Data.GPIO_Init);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6;
	LCD_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &LCD_Data.GPIO_Init);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &LCD_Data.GPIO_Init);

	LCD_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOG, &LCD_Data.GPIO_Init);

	LCDDis();

	LCDSetV();
	LCDSetH();

	//PWM
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseStructInit(&LCD_Data.TIM_Init);
    LCD_Data.TIM_Init.TIM_Prescaler = 120;
    LCD_Data.TIM_Init.TIM_Period = 100;
    TIM_TimeBaseInit(TIM5, &LCD_Data.TIM_Init);

    TIM_OCStructInit(&LCD_Data.PWM_Init);
    LCD_Data.PWM_Init.TIM_Pulse = 0;
    LCD_Data.PWM_Init.TIM_OCMode = TIM_OCMode_PWM1;
    LCD_Data.PWM_Init.TIM_OutputState = TIM_OutputState_Enable;
    LCD_Data.PWM_Init.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM5, &LCD_Data.PWM_Init);

    TIM_Cmd(TIM5, ENABLE);

	//-----------------------------------------
	//----------------FSMC Init----------------
	//-----------------------------------------
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	LCD_Data.FSMC_TimingInit.FSMC_AccessMode = FSMC_AccessMode_A;
	LCD_Data.FSMC_TimingInit.FSMC_AddressHoldTime = 1;
	LCD_Data.FSMC_TimingInit.FSMC_AddressSetupTime = 0;
	LCD_Data.FSMC_TimingInit.FSMC_BusTurnAroundDuration = 0;
	LCD_Data.FSMC_TimingInit.FSMC_CLKDivision = 1;
	LCD_Data.FSMC_TimingInit.FSMC_DataLatency = 0;
	LCD_Data.FSMC_TimingInit.FSMC_DataSetupTime = 1;

	LCD_Data.FSMC_Init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	LCD_Data.FSMC_Init.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	LCD_Data.FSMC_Init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	LCD_Data.FSMC_Init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	LCD_Data.FSMC_Init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	LCD_Data.FSMC_Init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	LCD_Data.FSMC_Init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	LCD_Data.FSMC_Init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	LCD_Data.FSMC_Init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	LCD_Data.FSMC_Init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	LCD_Data.FSMC_Init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	LCD_Data.FSMC_Init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	LCD_Data.FSMC_Init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	LCD_Data.FSMC_Init.FSMC_ReadWriteTimingStruct = &LCD_Data.FSMC_TimingInit;
	LCD_Data.FSMC_Init.FSMC_WriteTimingStruct = &LCD_Data.FSMC_TimingInit;
	FSMC_NORSRAMInit(&LCD_Data.FSMC_Init);

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);

    //Reset LCD
    LCDResetR();
    Delay(0x0FFFFF);
    LCDSetR();
    Delay(0x0FFFFF);

	//DMA
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    //DMA Общее
    LCD_Data.DMA_Init.DMA_DIR = DMA_DIR_PeripheralSRC;
    LCD_Data.DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    LCD_Data.DMA_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    LCD_Data.DMA_Init.DMA_Mode = DMA_Mode_Normal;
    LCD_Data.DMA_Init.DMA_Priority = DMA_Priority_High;
    LCD_Data.DMA_Init.DMA_M2M = DMA_M2M_Enable;

	//DMA: LCD
	LCD_Data.DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&LCD_Data.nodata);
	LCD_Data.DMA_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	LCD_Data.DMA_Init.DMA_MemoryBaseAddr = MEM_LCD;
	LCD_Data.DMA_Init.DMA_MemoryInc = DMA_MemoryInc_Disable;
	LCD_Data.DMA_Init.DMA_BufferSize = LCD_H_PULSE;
	DMA_Init(DMA2_Channel3, &LCD_Data.DMA_Init);

	//DMA: CPU -> LCD
	LCD_Data.DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&mem_buf);
	LCD_Data.DMA_Init.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	LCD_Data.DMA_Init.DMA_MemoryBaseAddr = MEM_LCD;
	LCD_Data.DMA_Init.DMA_MemoryInc = DMA_MemoryInc_Disable;
	LCD_Data.DMA_Init.DMA_BufferSize = LCD_H_WIDTH;
	DMA_Init(DMA2_Channel4, &LCD_Data.DMA_Init);

	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
	NVIC_Init(&NVIC_InitStruct);

//	NVIC_EnableIRQ(DMA2_Channel3_IRQn);
//	NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
}


void LCDDraw()
{
	mem_cnt = LCD_HEIGHT + LCD_V_BACK + LCD_V_FRONT;
	DMA2_Channel4->CPAR = (uint32_t)(&LCD_Data.nodata);
	DMA2_Channel4->CCR &= ~DMA_CCR1_PINC;
	LCD_Data.stat = LCD_PULSE;
	DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_PULSE);
	DMA_Cmd(DMA2_Channel3, ENABLE);
}

//DMA: LCD
void DMA2_Channel3_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_IT_TC3);
	DMA_Cmd(DMA2_Channel3, DISABLE);

	switch (LCD_Data.stat)
	{
	case LCD_PULSE:
		LCDResetH();

		if (mem_cnt == 0)
			LCDResetV();
		else if (mem_cnt == 1)
			LCDSetV();

		LCD_Data.stat = LCD_BACK;
		DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_BACK);
		DMA_Cmd(DMA2_Channel3, ENABLE);
		break;
	case LCD_BACK:
		LCDSetH();
		LCD_Data.stat = LCD_FRONT;
		DMA_SetCurrDataCounter(DMA2_Channel4, LCD_H_WIDTH);
		DMA_Cmd(DMA2_Channel4, ENABLE);
		break;
	case LCD_FRONT:
		if (mem_cnt > LCD_HEIGHT + LCD_V_FRONT)
		{
			LCD_Data.stat = LCD_PULSE;
		    DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_PULSE);
		    DMA_Cmd(DMA2_Channel3, ENABLE);
			mem_cnt--;
		}
		else if (mem_cnt > LCD_V_FRONT)
		{
			if (mem_cnt == LCD_HEIGHT + LCD_V_FRONT)
			{
				DMA2_Channel4->CPAR = (uint32_t)(&mem_buf);
				DMA2_Channel4->CCR |= DMA_CCR1_PINC;
			}
			DMA2_Channel2->CPAR = MEM_RAM + ((mem_cnt - LCD_V_FRONT) * LCD_H_WIDTH * 2);
			DMA2_Channel2->CMAR = (uint32_t)(&mem_buf);
			LCD_Data.stat = LCD_PULSE;
			DMA_SetCurrDataCounter(DMA2_Channel2, LCD_H_WIDTH);
			DMA_Cmd(DMA2_Channel2, ENABLE);

//			uint16_t i;
//			for (i = 0; i < 320; i++)
//				mem_buf[i] = 0xF800;
//	        DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_PULSE);
//	        DMA_Cmd(DMA2_Channel3, ENABLE);

			mem_cnt--;
		}
		else if (mem_cnt > 0)
		{
			if (mem_cnt == LCD_HEIGHT)
			{
				DMA2_Channel4->CPAR = (uint32_t)(&LCD_Data.nodata);
				DMA2_Channel4->CCR &= ~DMA_CCR1_PINC;
			}
			LCD_Data.stat = LCD_PULSE;
		    DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_PULSE);
		    DMA_Cmd(DMA2_Channel3, ENABLE);
			mem_cnt--;
		}
		else
		{
			Data_Data.busy = false;
		}
		break;
	}
}

//DMA: CPU -> LCD
void DMA2_Channel4_5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_IT_TC4);
    DMA_Cmd(DMA2_Channel4, DISABLE);

	DMA_SetCurrDataCounter(DMA2_Channel3, LCD_H_FRONT);
	DMA_Cmd(DMA2_Channel3, ENABLE);
}

void LCDSetH()
{
	GPIOC->ODR |= (1<<6);
}

void LCDResetH()
{
	GPIOC->ODR &= ~(1<<6);
}

void LCDSetV()
{
	GPIOA->ODR |= (1<<8);
}

void LCDResetV()
{
	GPIOA->ODR &= ~(1<<8);
}

void LCDSetR()
{
	GPIOC->ODR |= (1<<1);
}

void LCDResetR()
{
	GPIOC->ODR &= ~(1<<1);
}

void LCDEn()
{
	GPIO_SetBits(GPIOG, GPIO_Pin_10);
}

void LCDDis()
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_10);
}

void LCDLedUpdate()
{
	if (LCD_Data.t_led == LCD_Data.t_led_set)
	{
		LCD_Data.power_set = 2;
		LCD_Data.t_led++;
	}
	else
	{
		LCD_Data.t_led++;
	}

	if (LCD_Data.power > LCD_Data.power_set)
	{
		LCD_Data.power--;
		TIM5->CCR1 = LCD_Data.power;
	}
	else if (LCD_Data.power < LCD_Data.power_set)
	{
		LCD_Data.power++;
		TIM5->CCR1 = LCD_Data.power;
	}
}

void LCDLedReset()
{
	LCD_Data.power_set = 50;
	LCD_Data.t_led = 0;
}
