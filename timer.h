#pragma once

#define	T_MS_AC		(1<<0)
#define	T_10MS_AC	(1<<1)
#define	T_50MS_AC	(1<<2)
#define	T_S_AC		(1<<3)
#define	T_M_AC		(1<<4)
#define	T_H_AC		(1<<5)

//Таймеры внутренние
uint16_t	t_ms;
uint8_t		t_10ms;
uint8_t		t_50ms;
uint8_t		t_s;
uint8_t		t_m;
uint8_t		t_stat;

void TimerInit();
void TimerMs();
void Timer10ms();
void Timer50ms();
void TimerS();
void TimerM();
void TimerH();
