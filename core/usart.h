#pragma once

#include "main.h"

//Состояния UART
#define USART_RX		(1<<0)	//Приём
#define USART_RX_OF		(1<<1)	//Переполнение при приёме
#define USART_RX_PE		(1<<2)	//Ошибка чётности
#define USART_TX		(1<<3)	//Передача

#define USART_ENDLINE	(1<<0)
#define USART_CRC		(1<<1)

#define	USART_TX_MAX	40
#define	USART_RX_MAX	40

//Временные параметры USART
#define	USART_TIMEOUT		50
#define	USART_END_TIMEOUT	50
#define	USART_IDLE_TIMEOUT	500

typedef struct
{
	bool		en;
	bool		accept;
	uint8_t		state;

	uint8_t		ctu_tx;
	uint8_t		cnt_tx;
	uint8_t		ctu_rx;
	uint8_t		cnt_rx;
	uint8_t		data_tx[USART_TX_MAX];
	uint8_t		data_rx[USART_RX_MAX];

	uint16_t	t_rx;
	uint16_t	t_rx_set;
	uint16_t	t_tx;
	uint16_t	t_tx_set;
	uint16_t	t_idle;

	USART_InitTypeDef		USART_Init;
	GPIO_InitTypeDef		GPIO_Init;
} USARTData;
USARTData USART_Data;

void USARTInit();
void USARTStartTx(uint8_t cnt, uint8_t type);
void USARTStopTx();
void USARTStartRx();
void USARTStopRx();
bool USARTCheckCRC16();

uint16_t CRC16(uint8_t *puchMsg, uint8_t usDataLen);

void USARTTimerTxSet(uint16_t set);
void USARTTimerTx();
void USARTTimerRxSet(uint16_t set);
void USARTTimerRx();
void USARTTimerIdle();
