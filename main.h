#pragma once

#define CLC_DIV 1

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_flash.h"
#include "misc.h"
#include "stdbool.h"

#define	LED_GREEN	1
#define	LED_RED		2
#define	LED_BLUE	3
#define	LED_LCD		4

#define	MEM_RAM			((uint32_t)0x60000000)
#define	MEM_LCD			((uint32_t)0x6C000000)

#define LCD_H_WIDTH		320

#define FALSE 0
#define TRUE 1

uint16_t mem_buf[LCD_H_WIDTH + 4];
//uint16_t mem_buf2[LCD_H_WIDTH + 4];
uint16_t mem_cnt;

void LedInit();
void LedOn(int led);
void LedOff(int led);
bool LedIsOn(int led);

GPIO_InitTypeDef GPIO_InitStructure;

void Delay(uint32_t delayTime);
void BitNext(uint16_t *cur, uint16_t cfg);
void BitNumNext(uint8_t *cur, uint16_t cfg);
void BitPrev(uint16_t *cur, uint16_t cfg);
void BitNumPrev(uint8_t *cur, uint16_t cfg);
uint16_t adc_res;


uint8_t s_n;
bool s_en;
