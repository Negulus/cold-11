#include "main.h"
#include "timer.h"
#include "ram.h"
#include "rom.h"
#include "data.h"
#include "lcd.h"
#include "but.h"
#include "usart.h"
#include "draw.h"
#include "scr.h"
#include "mb.h"
#include "sound.h"
#include "memory.h"
#include "lcd_spi.h"

int main(void)
{
	LedInit();
	TimerInit();
	DataInit();
	RAMInit();
	LCDInit();
	ScrInit();
	ROMInit();
	ButInit();
	USARTInit();
	MBInit();
	SoundInit();
	LCDSPI_Init();

	s_en = false;
/*

	adc_res = 0;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_3);
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	NVIC_EnableIRQ(ADC1_2_IRQn);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);

	// калибровка АЦП
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
*/

    screen = scr_logo;
    while(1)
    {
    	DataUpdate();
    }
}

void ADC1_2_IRQHandler()
{
	if (ADC_GetITStatus(TIM6, ADC_IT_EOC) != RESET)
		ADC_ClearITPendingBit(TIM6, ADC_IT_EOC);

	adc_res = ADC_GetConversionValue(ADC1);

	if (adc_res < 2000)
	{
	    TIM_Cmd(TIM4, ENABLE);
	    TIM_PrescalerConfig(TIM4, adc_res, TIM_PSCReloadMode_Update);
	}
	else
	{
	    TIM_Cmd(TIM4, DISABLE);
	}
}

//ms
void TIM6_IRQHandler()
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

	t_ms++;
	t_10ms++;
	t_50ms++;

	if (!(t_stat & T_MS_AC))
	{
		t_stat |= T_MS_AC;
		TimerMs();
		t_stat &= ~T_MS_AC;
	}

	//10ms
	if (t_10ms >= 10)
	{
		t_10ms = 0;
		if (!(t_stat & T_10MS_AC))
		{
			t_stat |= T_10MS_AC;
			Timer10ms();
			t_stat &= ~T_10MS_AC;
		}
	}
	//50ms
	if (t_50ms >= 50)
	{
		t_50ms = 0;
		if (!(t_stat & T_50MS_AC))
		{
			t_stat |= T_50MS_AC;
			Timer50ms();
			t_stat &= ~T_50MS_AC;
		}
	}
	//s
	if (t_ms >= 1000)
	{
		t_ms = 0;
		t_s++;
		if (!(t_stat & T_S_AC))
		{
			t_stat |= T_S_AC;
			TimerS();
			t_stat &= ~T_S_AC;
		}
		//m
		if (t_s >= 60)
		{
			t_s = 0;
			t_m++;
			if (!(t_stat & T_M_AC))
			{
				t_stat |= T_M_AC;
				TimerM();
				t_stat &= ~T_M_AC;
			}
			//h
			if (t_m >= 60)
			{
				t_m = 0;
				if (!(t_stat & T_H_AC))
				{
					t_stat |= T_H_AC;
					TimerH();
					t_stat &= ~T_H_AC;
				}
			}
		}
	}
}

void Delay(uint32_t delayTime)
{
	uint32_t i;
	for(i = 0; i < delayTime; i++);
}

void BitNext(uint16_t *cur, uint16_t cfg)
{
	*cur <<= 1;
	while ((*cur & cfg) == 0)
	{
		*cur <<= 1;
		if (*cur == 0) *cur = 1;
	}
}

void BitNumNext(uint8_t *cur, uint16_t cfg)
{
	(*cur)++;
	while (((1 << *cur) & cfg) == 0)
	{
		(*cur)++;
		if (*cur > 15) *cur = 0;
	}
}

void BitPrev(uint16_t *cur, uint16_t cfg)
{
	*cur >>= 1;
	while ((*cur & cfg) == 0)
	{
		*cur >>= 1;
		if (*cur == 0) *cur = (1<<15);
	}
}

void BitNumPrev(uint8_t *cur, uint16_t cfg)
{
	if (*cur < 1) *cur = 15;
	else (*cur)--;
	while (((1 << *cur) & cfg) == 0)
	{
		if (*cur < 1) *cur = 15;
		else (*cur)--;
	}
}
