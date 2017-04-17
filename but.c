#include "but.h"
#include "usart.h"
#include "data.h"
#include "lcd.h"
#include "draw.h"
#include "scr.h"
#include "mb.h"
#include "sound.h"

void ButInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	But_Data.GPIO_Init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	But_Data.GPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
	But_Data.GPIO_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &But_Data.GPIO_Init);

	uint8_t i;
	for (i = 0; i < 6; i++)
	{
		But_Data.but[i].stat = false;
		But_Data.but[i].t = 0;
	}
}

void ButClick_0()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But0_Click();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But0_Click();
		break;
	}
}

void ButClickL_0()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But0_ClickL();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But0_ClickL();
		break;
	}
}

void ButClick_1()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But1_Click();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But1_Click();
		break;
	}
}

void ButClickL_1()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But1_ClickL();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But1_ClickL();
		break;
	}
}

void ButClick_2()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But2_Click();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But2_Click();
		break;
	}

	USART_Data.data_rx[0] = 1;
	USART_Data.data_rx[1] = 0x31;
	USART_Data.data_rx[2] = 0x00;
	USART_Data.data_rx[3] = 0x01;
	USART_Data.data_rx[4] = 0x00;
	USART_Data.data_rx[5] = 0x0A;
	USART_Data.data_rx[6] = 0x14;
	USART_Data.data_rx[7] = 'Ï';
	USART_Data.data_rx[8] = 'ð';
	USART_Data.data_rx[9] = 'î';
	USART_Data.data_rx[10] = 'â';
	USART_Data.data_rx[11] = 'å';
	USART_Data.data_rx[12] = 'ð';
	USART_Data.data_rx[13] = 'ê';
	USART_Data.data_rx[14] = 'à';
	USART_Data.data_rx[15] = ' ';
	USART_Data.data_rx[16] = '2';
	USART_Data.data_rx[17] = ' ';
	USART_Data.data_rx[18] = ' ';
	USART_Data.data_rx[19] = ' ';
	USART_Data.data_rx[20] = ' ';
	USART_Data.data_rx[21] = ' ';
	USART_Data.data_rx[22] = ' ';
	USART_Data.data_rx[23] = ' ';
	USART_Data.data_rx[24] = ' ';
	USART_Data.data_rx[25] = ' ';
	USART_Data.data_rx[26] = ' ';
	USART_Data.data_rx[27] = 0;
	USART_Data.data_rx[28] = 0;
	MBWriteText();
/*
	USART_Data.data_rx[0] = 1;
	USART_Data.data_rx[1] = 0x30;
	USART_Data.data_rx[2] = 0x00;
	USART_Data.data_rx[3] = 0x00;
	USART_Data.data_rx[4] = 0x00;
	USART_Data.data_rx[5] = 0x0A;
	USART_Data.data_rx[6] = 0;
	USART_Data.data_rx[7] = 0;
	MBReadText();
*/
}

void ButClickL_2()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But2_ClickL();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But2_ClickL();
		break;
	}
}

void ButClick_3()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But3_Click();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But3_Click();
		break;
	}
}

void ButClickL_3()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But3_ClickL();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But3_ClickL();
		break;
	}
}

void ButClick_4()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But4_Click();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But4_Click();
		break;
	}
}

void ButClickL_4()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But4_ClickL();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But4_ClickL();
		break;
	}
}

void ButClick_5()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But5_Click();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But5_Click();
		break;
	}
}

void ButClickL_5()
{
	switch (screen)
	{
	case scr_main_sens:
	case scr_main_out:
	case scr_main_set:
		ScrMain_But5_ClickL();
		break;
	case scr_menu_main:
	case scr_menu_set1:
	case scr_menu_set2:
	case scr_menu_timer:
	case scr_menu_time:
	case scr_menu_log:
	case scr_menu_config:
		ScrMenu_But5_ClickL();
		break;
	}
}

void ButUpdate()
{
	But_Data.but[0].stat = (GPIOC->IDR & GPIO_Pin_10) == 0;
	But_Data.but[1].stat = (GPIOC->IDR & GPIO_Pin_11) == 0;
	But_Data.but[2].stat = (GPIOC->IDR & GPIO_Pin_12) == 0;
	But_Data.but[3].stat = (GPIOC->IDR & GPIO_Pin_13) == 0;
	But_Data.but[4].stat = (GPIOC->IDR & GPIO_Pin_15) == 0;
	But_Data.but[5].stat = (GPIOC->IDR & GPIO_Pin_14) == 0;

	uint8_t i;
	for (i = 0; i < 6; i++)
	{
		if (But_Data.but[i].stat)
		{
			if (But_Data.but[i].t < BUT_LONG)
			{
				But_Data.but[i].t++;
			}
			else if (But_Data.but[i].t == BUT_LONG)
			{
//				SoundL();
				LCDLedReset();
				switch (i)
				{
				case 0:
					ButClickL_0();
					break;
				case 1:
					ButClickL_1();
					break;
				case 2:
					ButClickL_2();
					break;
				case 3:
					ButClickL_3();
					break;
				case 4:
					ButClickL_4();
					break;
				case 5:
					ButClickL_5();
					break;
				}
				But_Data.but[i].t++;
			}
		}
		else
		{
			if (But_Data.but[i].t >= BUT_SHORT && But_Data.but[i].t < BUT_LONG)
			{
//				SoundS();
				LCDLedReset();
				switch (i)
				{
				case 0:
					ButClick_0();
					break;
				case 1:
					ButClick_1();
					break;
				case 2:
					ButClick_2();
					break;
				case 3:
					ButClick_3();
					break;
				case 4:
					ButClick_4();
					break;
				case 5:
					ButClick_5();
					break;
				}
			}
			But_Data.but[i].t = 0;
		}
	}
}
