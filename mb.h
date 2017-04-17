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
	uint16_t	reg_100;	//��������� �����
	uint16_t	reg_101;	//�������� 04
	uint16_t	reg_102;	//�������� 05
	uint16_t	reg_103;	//�������� 06
	uint16_t	reg_104;	//�������� 07
	uint16_t	reg_105;	//�������� 08
	uint16_t	reg_106;	//�������� 09
	uint16_t	reg_107;	//�������� 10
	uint16_t	reg_108;	//�������� 11
	uint16_t	reg_109;	//�������� 12
	uint16_t	reg_10A;	//�������� 13
	uint16_t	reg_10B;	//�������� 14
	uint16_t	reg_10C;	//�������� 15
	uint16_t	reg_10D;	//�������� 16
	uint16_t	reg_10E;	//�������� 17
	uint16_t	reg_10F;	//�������� 18
	uint16_t	reg_110;	//�������� 19
	uint16_t	reg_111;	//�������� 20
	uint16_t	reg_112;	//����� ������������ ������
	uint16_t	reg_113;	//�������������� ������������ ������
	uint16_t	reg_114;	//��������� ������
	uint16_t	reg_115;	//������� �����������
	uint16_t	reg_116;	//������� ���������
	uint16_t	reg_117;	//����������� � ������
	uint16_t	reg_118;	//����������� ����
	uint16_t	reg_119;	//�������� ���������
	uint16_t	reg_11A;	//��������� ���������
	uint16_t	reg_11B;	//��������� � ������
	uint16_t	reg_11C;	//������
	uint16_t	reg_11D;	//��������� �����
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
