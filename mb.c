#include "mb.h"
#include "usart.h"
#include "data.h"
#include "zt033.h"
#include "memory.h"

void MBInit()
{
	MB_Data.er_adr = false;
	MB_Data.adr = 1;

	//Инициализация массива регистров
	MB_Data.Holding_Reg[0] = &MB_Data.Reg.reg_100;
	MB_Data.Holding_Reg[1] = &MB_Data.Reg.reg_101;
	MB_Data.Holding_Reg[2] = &MB_Data.Reg.reg_102;
	MB_Data.Holding_Reg[3] = &MB_Data.Reg.reg_103;
	MB_Data.Holding_Reg[4] = &MB_Data.Reg.reg_104;
	MB_Data.Holding_Reg[5] = &MB_Data.Reg.reg_105;
	MB_Data.Holding_Reg[6] = &MB_Data.Reg.reg_106;
	MB_Data.Holding_Reg[7] = &MB_Data.Reg.reg_107;
	MB_Data.Holding_Reg[8] = &MB_Data.Reg.reg_108;
	MB_Data.Holding_Reg[9] = &MB_Data.Reg.reg_109;
	MB_Data.Holding_Reg[10] = &MB_Data.Reg.reg_10A;
	MB_Data.Holding_Reg[11] = &MB_Data.Reg.reg_10B;
	MB_Data.Holding_Reg[12] = &MB_Data.Reg.reg_10C;
	MB_Data.Holding_Reg[13] = &MB_Data.Reg.reg_10D;
	MB_Data.Holding_Reg[14] = &MB_Data.Reg.reg_10E;
	MB_Data.Holding_Reg[15] = &MB_Data.Reg.reg_10F;
	MB_Data.Holding_Reg[16] = &MB_Data.Reg.reg_110;
	MB_Data.Holding_Reg[17] = &MB_Data.Reg.reg_111;
	MB_Data.Holding_Reg[18] = &MB_Data.Reg.reg_112;
	MB_Data.Holding_Reg[19] = &MB_Data.Reg.reg_113;
	MB_Data.Holding_Reg[20] = &MB_Data.Reg.reg_114;
	MB_Data.Holding_Reg[21] = &MB_Data.Reg.reg_115;
	MB_Data.Holding_Reg[22] = &MB_Data.Reg.reg_116;
	MB_Data.Holding_Reg[23] = &MB_Data.Reg.reg_117;
	MB_Data.Holding_Reg[24] = &MB_Data.Reg.reg_118;
	MB_Data.Holding_Reg[25] = &MB_Data.Reg.reg_119;
	MB_Data.Holding_Reg[26] = &MB_Data.Reg.reg_11A;
	MB_Data.Holding_Reg[27] = &MB_Data.Reg.reg_11D;
	MB_Data.Holding_Reg[28] = &MB_Data.Reg.reg_11C;
	MB_Data.Holding_Reg[29] = &MB_Data.Reg.reg_11D;

	//Инициализация регистров
	MB_Data.Reg.reg_100 = 1;
	MB_Data.Reg.reg_101 = 0;
	MB_Data.Reg.reg_102 = 0;
	MB_Data.Reg.reg_103 = 0;
	MB_Data.Reg.reg_104 = 0;
	MB_Data.Reg.reg_105 = 0;
	MB_Data.Reg.reg_106 = 0;
	MB_Data.Reg.reg_107 = 0;
	MB_Data.Reg.reg_108 = 0;
	MB_Data.Reg.reg_109 = 0;
	MB_Data.Reg.reg_10A = 0;
	MB_Data.Reg.reg_10B = 0;
	MB_Data.Reg.reg_10C = 0;
	MB_Data.Reg.reg_10D = 0;
	MB_Data.Reg.reg_10E = 0;
	MB_Data.Reg.reg_10F = 0;
	MB_Data.Reg.reg_110 = 0;
	MB_Data.Reg.reg_111 = 0;
	MB_Data.Reg.reg_112 = (4 << 0) | PANEL_FANSPEED | PANEL_AUTO | PANEL_MODE_VENT | PANEL_MODE_HEAT | PANEL_TEMP_AIR | PANEL_TEMP_ROOM | PANEL_HUM_AIR;
	MB_Data.Reg.reg_113 = (10 << 0) | (35 << 6);
	MB_Data.Reg.reg_114 = (0 << 0) | (1 << 1) | PANEL_AUTO | PANEL_MODE_VENT | PANEL_TEMP_ROOM;
	MB_Data.Reg.reg_115 = 25;
	MB_Data.Reg.reg_116 = 60;
	MB_Data.Reg.reg_117 = 245;
	MB_Data.Reg.reg_118 = 576;
	MB_Data.Reg.reg_119 = 0;
	MB_Data.Reg.reg_11A = 0;
	MB_Data.Reg.reg_11D = 0;
	MB_Data.Reg.reg_11C = PANEL_SYS_OFF;
	MB_Data.Reg.reg_11D = (12 << 0) | (0 << 5) | (5 << 11);
}

bool MBReceive()
{
	if (USART_Data.ctu_rx == 1)
	{
		if (USART_Data.data_rx[0] != MB_Data.adr)
			MB_Data.er_adr = true;
		else
			MB_Data.er_adr = false;
	}

	if (!MB_Data.er_adr)
		return true;
	else
		return false;
}

bool MBEnd()
{
	if (MB_Data.er_adr)
		return true;

	switch (USART_Data.data_rx[1])
	{
	case MB_READ_C:
		if (USARTCheckCRC16()) MBReadC();
		break;
	case MB_READ_DI:
		if (USARTCheckCRC16()) MBReadDI();
		break;
	case MB_READ_HR:
		if (USARTCheckCRC16()) MBReadHR();
		break;
	case MB_READ_IR:
		if (USARTCheckCRC16()) MBReadIR();
		break;
	case MB_WRITE_SC:
		if (USARTCheckCRC16()) MBWriteSC();
		break;
	case MB_WRITE_SR:
		if (USARTCheckCRC16()) MBWriteSR();
		break;
	case MB_WRITE_MC:
		if (USARTCheckCRC16()) MBWriteMC();
		break;
	case MB_WRITE_MR:
		if (USARTCheckCRC16()) MBWriteMR();
		break;
	case MB_DATA_CMD:
		DataUSARTRx();
		break;
	case MB_READ_TEXT:
		MBReadText();
		break;
	case MB_WRITE_TEXT:
		MBWriteText();
		break;
	default:
		return true;
		break;
	}

	return false;
}

//0x01 Read Coils
void MBReadC()
{
	//Отправить ошибку 01
	USART_Data.data_tx[0] = MB_Data.adr;
	USART_Data.data_tx[1] = 0x81;
	USART_Data.data_tx[2] = 0x01;
	USARTStartTx(3, USART_CRC);
}

//0x02 Read Discrete Inputs
void MBReadDI()
{
	//Отправить ошибку 01
	USART_Data.data_tx[0] = MB_Data.adr;
	USART_Data.data_tx[1] = 0x82;
	USART_Data.data_tx[2] = 0x01;
	USARTStartTx(3, USART_CRC);
}

//0x03 Read Holding Register
void MBReadHR()
{
	USART_Data.data_tx[0] = MB_Data.adr;

	uint16_t reg_start = (uint16_t)((USART_Data.data_rx[2] << 8) | USART_Data.data_rx[3]);
	uint16_t reg_count = (uint16_t)((USART_Data.data_rx[4] << 8) | USART_Data.data_rx[5]);
	reg_start -= 0x0100;
	//Если диапазон адресов вне допустимого
	if ((reg_start + reg_count) > MB_COUNT_HR)
	{
		//Отправить ошибку 02
		USART_Data.data_tx[1] = 0x83;
		USART_Data.data_tx[2] = 0x02;
		USARTStartTx(3, USART_CRC);
	}
	else if (USART_Data.ctu_rx < 8)
	{
		//Отправить ошибку 03
		USART_Data.data_tx[1] = 0x83;
		USART_Data.data_tx[2] = 0x03;
		USARTStartTx(3, USART_CRC);
	}
	else
	{
		//Подготовить и отправить ответ
		USART_Data.data_tx[1] = 0x03;
		USART_Data.data_tx[2] = reg_count * 2;

		uint8_t i;
		for (i = 0; i < reg_count; i++)
		{
			USART_Data.data_tx[3 + (i * 2)] = *MB_Data.Holding_Reg[reg_start + i] >> 8;
			USART_Data.data_tx[4 + (i * 2)] = *MB_Data.Holding_Reg[reg_start + i] & 0xff;
		}

		USARTStartTx(3 + USART_Data.data_tx[2], USART_CRC);
	}
}

//0x04 Read Input Registers
void MBReadIR()
{
	//Отправить ошибку 01
	USART_Data.data_tx[0] = MB_Data.adr;
	USART_Data.data_tx[1] = 0x84;
	USART_Data.data_tx[2] = 0x01;
	USARTStartTx(3, USART_CRC);
}

//0x05 Write Single Coil
void MBWriteSC()
{
	//Отправить ошибку 01
	USART_Data.data_tx[0] = MB_Data.adr;
	USART_Data.data_tx[1] = 0x85;
	USART_Data.data_tx[2] = 0x01;
	USARTStartTx(3, USART_CRC);
}

//0x06 Write Single Register
void MBWriteSR()
{
	USART_Data.data_tx[0] = MB_Data.adr;

	uint16_t reg_adr = (uint16_t)((USART_Data.data_rx[2] << 8) | USART_Data.data_rx[3]);
	reg_adr -= 0x0100;
	//Если диапазон адресов вне допустимого
	if (reg_adr > MB_COUNT_HR)
	{
		//Отправить ошибку 02
		USART_Data.data_tx[1] = 0x86;
		USART_Data.data_tx[2] = 0x02;
		USARTStartTx(3, USART_CRC);
	}
	else if (USART_Data.ctu_rx < 8)
	{
		//Отправить ошибку 03
		USART_Data.data_tx[1] = 0x86;
		USART_Data.data_tx[2] = 0x03;
		USARTStartTx(3, USART_CRC);
	}
	else
	{
		//Записать полученые данные
		*MB_Data.Holding_Reg[reg_adr] = (uint16_t)((USART_Data.data_rx[4] << 8) | USART_Data.data_rx[5]);

		//Подготовить и отправить ответ
		USART_Data.data_tx[1] = 0x06;
		USART_Data.data_tx[2] = USART_Data.data_rx[2];
		USART_Data.data_tx[3] = USART_Data.data_rx[3];
		USART_Data.data_tx[4] = USART_Data.data_rx[4];
		USART_Data.data_tx[5] = USART_Data.data_rx[5];
		USARTStartTx(6, USART_CRC);
	}
}

//0x0F Write Multiple Coils
void MBWriteMC()
{
	//Отправить ошибку 01
	USART_Data.data_tx[0] = MB_Data.adr;
	USART_Data.data_tx[1] = 0x8F;
	USART_Data.data_tx[2] = 0x01;
	USARTStartTx(3, USART_CRC);
}

//0x10 Write Multiple Registers
void MBWriteMR()
{
	USART_Data.data_tx[0] = MB_Data.adr;

	uint16_t reg_start = (uint16_t)((USART_Data.data_rx[2] << 8) | USART_Data.data_rx[3]);
	uint16_t reg_count = (uint16_t)((USART_Data.data_rx[4] << 8) | USART_Data.data_rx[5]);
	reg_start -= 0x0100;

	//Если диапазон адресов вне допустимого
	if ((reg_start + reg_count) > MB_COUNT_HR)
	{
		//Отправить ошибку 02
		USART_Data.data_tx[1] = 0x90;
		USART_Data.data_tx[2] = 0x02;
		USARTStartTx(3, USART_CRC);
	}
	else if (USART_Data.ctu_rx < (9 + (reg_count * 2)))
	{
		//Отправить ошибку 03
		USART_Data.data_tx[1] = 0x90;
		USART_Data.data_tx[2] = 0x03;
		USARTStartTx(3, USART_CRC);
	}
	else
	{
		uint8_t i;
		//Записать полученые данные
		for (i = 0; i < reg_count; i++)
		{
			*MB_Data.Holding_Reg[reg_start + i] = (uint16_t)((USART_Data.data_rx[7 + (i * 2)] << 8) | USART_Data.data_rx[8 + (i * 2)]);
		}

		//Подготовить и отправить ответ
		USART_Data.data_tx[1] = 0x10;
		USART_Data.data_tx[2] = USART_Data.data_rx[2];
		USART_Data.data_tx[3] = USART_Data.data_rx[3];
		USART_Data.data_tx[4] = USART_Data.data_rx[4];
		USART_Data.data_tx[5] = USART_Data.data_rx[5];
		USARTStartTx(6, USART_CRC);
	}
}

void MBReadText()
{
	USART_Data.data_tx[0] = MB_Data.adr;

	uint16_t reg_start = (uint16_t)((USART_Data.data_rx[2] << 8) | USART_Data.data_rx[3]);
	uint16_t reg_count = (uint16_t)((USART_Data.data_rx[4] << 8) | USART_Data.data_rx[5]);

	//Если диапазон адресов вне допустимого
	if (reg_start >= (MENU_MAX_TEXT_SET + MENU_MAX_TEXT_SET + MENU_MAX_TEXT_LOG))
	{
		//Отправить ошибку 02
		USART_Data.data_tx[1] = 0xB0;
		USART_Data.data_tx[2] = 0x02;
		USARTStartTx(3, USART_CRC);
	}
	else if (reg_count != MENU_LINE_LENGTH / 2)
	{
		//Отправить ошибку 03
		USART_Data.data_tx[1] = 0xB0;
		USART_Data.data_tx[2] = 0x03;
		USARTStartTx(3, USART_CRC);
	}
	else
	{
		//Подготовить и отправить ответ
		USART_Data.data_tx[1] = 0x30;
		USART_Data.data_tx[2] = reg_count * 2;

		uint8_t i;
		uint16_t tmp;
		for (i = 0; i < reg_count; i++)
		{
			tmp = MemRead(MEM_PAGE_TEXT, (reg_start * MENU_LINE_LENGTH / 2) + i);
			USART_Data.data_tx[3 + (i * 2)] = tmp >> 8;
			USART_Data.data_tx[4 + (i * 2)] = tmp & 0xff;
		}

		USARTStartTx(3 + USART_Data.data_tx[2], USART_CRC);
	}
}

void MBWriteText()
{
	USART_Data.data_tx[0] = MB_Data.adr;

	uint16_t reg_start = (uint16_t)((USART_Data.data_rx[2] << 8) | USART_Data.data_rx[3]);
	uint8_t byte_count = USART_Data.data_rx[6];

	//Если диапазон адресов вне допустимого
	if (reg_start >= (MENU_MAX_TEXT_SET + MENU_MAX_TEXT_SET + MENU_MAX_TEXT_LOG))
	{
		//Отправить ошибку 02
		USART_Data.data_tx[1] = 0xB1;
		USART_Data.data_tx[2] = 0x02;
		USARTStartTx(3, USART_CRC);
	}
	else if (byte_count != MENU_LINE_LENGTH)
	{
		//Отправить ошибку 03
		USART_Data.data_tx[1] = 0xB1;
		USART_Data.data_tx[2] = 0x03;
		USARTStartTx(3, USART_CRC);
	}
	else
	{
		MemWriteText(MEM_PAGE_TEXT, reg_start * MENU_LINE_LENGTH / 2, &USART_Data.data_rx[7], MENU_LINE_LENGTH);

		USART_Data.data_tx[1] = 0xB1;
		USART_Data.data_tx[2] = USART_Data.data_rx[2];
		USART_Data.data_tx[3] = USART_Data.data_rx[3];
		USART_Data.data_tx[4] = USART_Data.data_rx[4];
		USART_Data.data_tx[5] = USART_Data.data_rx[5];
		USARTStartTx(6, USART_CRC);
	}
}
