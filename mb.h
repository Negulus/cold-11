#pragma once

#include "main.h"

#define MB_READ_C		0x01
#define MB_READ_DI		0x02
#define MB_READ_HR		0x03
#define MB_READ_IR		0x04
#define MB_WRITE_SC		0x05
#define MB_WRITE_SR		0x06
#define MB_WRITE_MC		0x0F
#define MB_WRITE_MR		0x10
#define MB_DATA_CMD		0x20
#define MB_READ_TEXT	0x30
#define MB_WRITE_TEXT	0x31

#define MB_COUNT_HR		30		//!!!!!!!!!!!!!!!!!!!!!!

typedef struct
{
	uint16_t	reg_100;	//Параметры связи
	uint16_t	reg_101;	//Параметр 04
	uint16_t	reg_102;	//Параметр 05
	uint16_t	reg_103;	//Параметр 06
	uint16_t	reg_104;	//Параметр 07
	uint16_t	reg_105;	//Параметр 08
	uint16_t	reg_106;	//Параметр 09
	uint16_t	reg_107;	//Параметр 10
	uint16_t	reg_108;	//Параметр 11
	uint16_t	reg_109;	//Параметр 12
	uint16_t	reg_10A;	//Параметр 13
	uint16_t	reg_10B;	//Параметр 14
	uint16_t	reg_10C;	//Параметр 15
	uint16_t	reg_10D;	//Параметр 16
	uint16_t	reg_10E;	//Параметр 17
	uint16_t	reg_10F;	//Параметр 18
	uint16_t	reg_110;	//Параметр 19
	uint16_t	reg_111;	//Параметр 20
	uint16_t	reg_112;	//Общая конфигурация панели
	uint16_t	reg_113;	//Дополнительная конфигурация панели
	uint16_t	reg_114;	//Состояния панели
	uint16_t	reg_115;	//Уставка температуры
	uint16_t	reg_116;	//Уставка влажности
	uint16_t	reg_117;	//Температуры в канале
	uint16_t	reg_118;	//Температура воды
	uint16_t	reg_119;	//Наружная влажность
	uint16_t	reg_11A;	//Комнатная влажность
	uint16_t	reg_11B;	//Влажность в канале
	uint16_t	reg_11C;	//Ошибки
	uint16_t	reg_11D;	//Установка часов
} MBReg;

typedef struct
{
	bool		er_adr;
	uint8_t		adr;
	MBReg		Reg;

	uint16_t	*Holding_Reg[MB_COUNT_HR];
} MBData;

MBData MB_Data;

void MBInit();
bool MBReceive();
bool MBEnd();

void MBReadC();
void MBReadDI();
void MBReadHR();
void MBReadIR();
void MBWriteSC();
void MBWriteSR();
void MBWriteMC();
void MBWriteMR();
void MBReadText();
void MBWriteText();
