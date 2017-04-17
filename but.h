#pragma once

#include "main.h"

//50 мс!!!
#define BUT_SHORT	1
#define	BUT_LONG	20

typedef struct
{
	bool stat;
	uint8_t t;
} Button;

typedef struct
{
	EXTI_InitTypeDef 		EXTI_Init;
	GPIO_InitTypeDef 		GPIO_Init;
	NVIC_InitTypeDef 		NVIC_Init;

	Button					but[6];
} ButData;
ButData	But_Data;

void ButInit();
void ButUpdate();

void ButClick_0();
void ButClickL_0();
void ButClick_1();
void ButClickL_1();
void ButClick_2();
void ButClickL_2();
void ButClick_3();
void ButClickL_3();
void ButClick_4();
void ButClickL_4();
void ButClick_5();
void ButClickL_5();
