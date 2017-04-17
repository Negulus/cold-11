#include "usart.h"
#include "data.h"
#include "mb.h"

uint8_t rrr = 0;

void USARTInit()
{
	USART_Data.en = false;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	USART_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_9;
	USART_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	USART_Data.GPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &USART_Data.GPIO_Init);

	USART_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_10;
	USART_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &USART_Data.GPIO_Init);

	USART_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_11;
	USART_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &USART_Data.GPIO_Init);

	USART_Data.USART_Init.USART_BaudRate = 9600 / 3;//115200 / 3;
	USART_Data.USART_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Data.USART_Init.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Data.USART_Init.USART_Parity = USART_Parity_No;
	USART_Data.USART_Init.USART_StopBits = USART_StopBits_2;
	USART_Data.USART_Init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_Data.USART_Init);

	USART_Cmd(USART1, ENABLE);

	USARTTimerRxSet(0);
	USARTTimerTxSet(0);
	USART_Data.t_idle = 0;

	USART_ITConfig(USART1, USART_IT_PE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_TimeBaseStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_Period = 10;
	TIM_InitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInit(TIM7, &TIM_InitStructure);
	TIM_Cmd(TIM7, DISABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
	NVIC_EnableIRQ(TIM7_IRQn);
}

void USART1_IRQHandler()
{
	if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		USARTTimerTxSet(USART_TIMEOUT);
		if (USART_Data.ctu_tx < USART_Data.cnt_tx)
		{
			USART_SendData(USART1, USART_Data.data_tx[USART_Data.ctu_tx]);
			USART_Data.ctu_tx++;
		}
		else
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
		}
	}
	else if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USARTStopTx();
	}
	else if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		rrr = 10;
		if (USART_Data.state & USART_RX)
		{
			rrr = 11;
			USARTTimerRxSet(USART_END_TIMEOUT);

			if (USART_Data.ctu_rx >= USART_RX_MAX)
				USART_Data.state |= USART_RX_OF;

			if (USART_Data.state & (USART_RX_PE | USART_RX_OF))
			{
				rrr = 12;
				USART_ReceiveData(USART1);
				return;
			}

			USART_Data.data_rx[USART_Data.ctu_rx++] = USART_ReceiveData(USART1);

			if (MBReceive())
			{
				rrr = 13;
				//Окончание посылки по символу конца строки для системных данных
				if (USART_Data.ctu_rx > 1)
				{
					if (USART_Data.data_rx[1] == MB_DATA_CMD)
					{
						if (USART_Data.data_rx[USART_Data.ctu_rx-2] == 0x0D && USART_Data.data_rx[USART_Data.ctu_rx-1] == 0x0A)
						{
							rrr = 14;
							USARTTimerRxSet(0);
							USARTStopRx();
						}
					}
				}
			}
		}
		else
		{
			rrr = 15;
			USART_ReceiveData(USART1);
		}
	}
	else if (USART_GetITStatus(USART1, USART_IT_PE) == SET)
	{
		USARTTimerRxSet(USART_END_TIMEOUT);
		USART_Data.state |= USART_RX_PE;
	}
	else if (USART_GetITStatus(USART1, USART_IT_ORE) == SET)
	{
		USARTTimerRxSet(USART_END_TIMEOUT);
		USART_ReceiveData(USART1);
	}
	else
	{
		rrr = 16;
		USARTTimerRxSet(USART_END_TIMEOUT);
		USART_ReceiveData(USART1);
	}
}

void USARTStartTx(uint8_t cnt, uint8_t type)
{
	rrr = 7;
	USART_Data.cnt_tx = cnt;

	//Рассчёт CRC при необходимости
	if (type & USART_CRC)
	{
		uint16_t tmpi = CRC16(USART_Data.data_tx, cnt);
		USART_Data.data_tx[USART_Data.cnt_tx++] = tmpi & 0xff;
		USART_Data.data_tx[USART_Data.cnt_tx++] = tmpi >> 8;
	}

	//Добавление символов конца строки при необходимости
	if (type & USART_ENDLINE)
	{
		USART_Data.data_tx[USART_Data.cnt_tx++] = 0x0D;
		USART_Data.data_tx[USART_Data.cnt_tx++] = 0x0A;
	}

	//Настройка на передачу
	USART_Data.state = USART_TX;

	TIM_Cmd(TIM7, ENABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

void TIM7_IRQHandler()
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

	TIM_Cmd(TIM7, DISABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);

	GPIOA->ODR |= GPIO_Pin_11;
	USART_Data.ctu_tx = 0;

	USARTTimerTxSet(USART_TIMEOUT);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

void USARTStopTx()
{
	rrr = 8;
	//Остановка передачи
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	GPIOA->ODR &= ~GPIO_Pin_11;
	USART_Data.state = 0;

	USARTTimerTxSet(0);

	USARTStartRx();
}

void USARTStartRx()
{
	//Настройка на приём
	USART_Data.state = USART_RX;
	USART_Data.ctu_rx = 0;

	//Начало приёма
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USARTStopRx()
{
	rrr = 2;
	if (USART_Data.state & USART_RX)
	{
		rrr = 3;
		bool tmpb = true;
		//Остановка приёма
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		USART_Data.state &= ~USART_RX;
		if (!(USART_Data.state & (USART_RX_PE | USART_RX_OF)))
		{
			rrr = 4;
			tmpb = MBEnd();
		}
		else
		{
			rrr = 5;
			USART_Data.state &= ~USART_RX_PE;
			USART_Data.state &= ~USART_RX_OF;
		}

		USART_Data.state = 0;

		if (tmpb)
		{
			rrr = 6;
			USARTStartRx();
		}
	}
}

bool USARTCheckCRC16()
{
	uint16_t tmpi = CRC16(USART_Data.data_rx, USART_Data.ctu_rx - 2);
	if (USART_Data.data_rx[USART_Data.ctu_rx - 2] == (tmpi & 0xff))
		if (USART_Data.data_rx[USART_Data.ctu_rx - 1] == (tmpi >> 8))
			return true;
	return false;
}

void USARTTimerTxSet(uint16_t set)
{
	USART_Data.t_tx = 0;
	USART_Data.t_tx_set = set;
}

void USARTTimerTx()
{
	if (USART_Data.t_tx_set > 0)
	{
		USART_Data.t_tx++;
		if (USART_Data.t_tx >= USART_Data.t_tx_set)
		{
			USART_Data.t_tx = 0;
			USART_Data.t_tx_set = 0;
			USARTStopTx();
		}
	}
}

void USARTTimerRxSet(uint16_t set)
{
	USART_Data.t_rx = 0;
	USART_Data.t_rx_set = set;
}

void USARTTimerRx()
{
	if (USART_Data.t_rx_set > 0)
	{
		USART_Data.t_rx++;
		if (USART_Data.t_rx >= USART_Data.t_rx_set)
		{
			USART_Data.t_rx = 0;
			USART_Data.t_rx_set = 0;
			USARTStopRx();
		}
	}
}

void USARTTimerIdle()
{
	return;
	if (!USART_Data.en) return;
	if (!(USART_Data.state & (USART_RX | USART_TX)))
	{
		if (USART_Data.t_idle < USART_IDLE_TIMEOUT)
		{
			USART_Data.t_idle++;
		}
		else
		{
			USART_Data.t_idle = 0;
			USARTStartRx();
		}
	}
	else
	{
		USART_Data.t_idle = 0;
	}
}
