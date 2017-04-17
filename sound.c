#include "sound.h"

void SoundInit()
{
	Sound_Data.en = false;
	Sound_Data.t = 0;

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_OCInitTypeDef timerPWM;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseStructInit(&TIM_InitStructure);
    TIM_InitStructure.TIM_Prescaler = 1000;
    TIM_InitStructure.TIM_Period = 18;
    TIM_TimeBaseInit(TIM4, &TIM_InitStructure);
    TIM_OCStructInit(&timerPWM);
    timerPWM.TIM_Pulse = 9;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    timerPWM.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM4, &timerPWM);
    TIM_Cmd(TIM4, DISABLE);
}

void SoundUpdate()
{
	if (Sound_Data.t > 0)
	{
		if (!Sound_Data.en)
		{
			TIM_Cmd(TIM4, ENABLE);
			Sound_Data.en = true;
		}
		Sound_Data.t -= 10;

		if (Sound_Data.t == 0)
		{
			Sound_Data.en = false;
			TIM_Cmd(TIM4, DISABLE);
		}
	}
}

void SoundS()
{
	Sound_Data.t = 100;
}

void SoundL()
{
	Sound_Data.t = 400;
}
