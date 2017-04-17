#include "main.h"
#include "timer.h"
#include "lcd.h"
#include "data.h"
#include "but.h"
#include "usart.h"
#include "draw.h"
#include "scr.h"
#include "sound.h"

uint16_t t_250ms;

void TimerInit()
{
	t_ms = 0;
	t_10ms = 0;
	t_50ms = 0;
	t_s = 0;
	t_m = 0;
	t_stat = 0;

	t_250ms = 0;
	s_n = 0;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Period = (120 * CLC_DIV) - 1;
	TIM_InitStructure.TIM_Prescaler = 1000 - 1;
	TIM_TimeBaseInit(TIM6, &TIM_InitStructure);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	NVIC_EnableIRQ(TIM6_IRQn);
}

void TimerMs()
{
	USARTTimerRx();
	USARTTimerTx();
	USARTTimerIdle();
}

void Timer10ms()
{
	SoundUpdate();
	LCDLedUpdate();
}


void Timer50ms()
{
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
/*
	uint16_t tmpui = lcd_data[LCD_H_WIDTH - 1];

	for (i = LCD_H_WIDTH - 1; i > 0; i--)
		lcd_data[i] = lcd_data[i - 1];

	lcd_data[0] = tmpui;
*/
	ButUpdate();
}

void TimerS()
{
	if (screen == scr_logo && t_s == 2)
	{
		USART_Data.en = true;
		USARTStartRx();
		ScrMain_Start();
	}
}

void TimerM()
{

}

void TimerH()
{

}
