#include "led.h"

void LedInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOG, GPIO_Pin_8 | GPIO_Pin_15);
}

void LedOn(int led)
{
	switch (led)
	{
	case LED_GREEN:
		GPIO_SetBits(GPIOG, GPIO_Pin_15);
		break;
	case LED_RED:
		GPIO_SetBits(GPIOG, GPIO_Pin_8);
		break;
	}
}

void LedOff(int led)
{
	switch (led)
	{
	case LED_GREEN:
		GPIO_ResetBits(GPIOG, GPIO_Pin_15);
		break;
	case LED_RED:
		GPIO_ResetBits(GPIOG, GPIO_Pin_8);
		break;
	}
}

bool LedIsOn(int led)
{
	switch (led)
	{
	case LED_GREEN:
		return GPIOG->ODR & (1<<15);
		break;
	case LED_RED:
		return GPIOG->ODR & (1<<8);
		break;
	}
	return false;
}
