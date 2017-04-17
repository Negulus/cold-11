#pragma once

#include "main.h"

#define	LCD_H_PULSE		1
#define LCD_H_BACK		68		//!!!
#define LCD_H_FRONT		8

#define LCD_HEIGHT		240

#define	LCD_V_BACK		15		//!!!!
#define	LCD_V_FRONT		10

#define LCD_PULSE		0
#define LCD_BACK		1
#define LCD_DATA		2
#define LCD_FRONT		3

void LCDInit();
void LCDDraw();
void LCDStart();
void LCDSetH();
void LCDResetH();
void LCDSetV();
void LCDResetV();
void LCDSetR();
void LCDResetR();
void LCDEn();
void LCDDis();
void LCDLedUpdate();
void LCDLedReset();

typedef struct
{
	uint8_t							stat;
	uint16_t 						nodata;
	uint16_t						t_led;
	uint16_t						t_led_set;
	uint8_t							power;
	uint8_t							power_set;

	FSMC_NORSRAMInitTypeDef 		FSMC_Init;
	FSMC_NORSRAMTimingInitTypeDef 	FSMC_TimingInit;
	GPIO_InitTypeDef 				GPIO_Init;
	DMA_InitTypeDef 				DMA_Init;
	TIM_TimeBaseInitTypeDef			TIM_Init;
	TIM_OCInitTypeDef 				PWM_Init;
} LCDData;
LCDData	LCD_Data;
