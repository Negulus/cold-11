#pragma once

//Режимы панели
#define PANEL_MODE_VENT		(1<<5)
#define PANEL_MODE_HEAT		(1<<6)
#define PANEL_MODE_COOL		(1<<7)
#define PANEL_MODE_HUM		(1<<8)
#define PANEL_MODE_DHUM		(1<<9)

#define PANEL_FANSPEED		(1<<3)
#define PANEL_AUTO			(1<<4)

#define PANEL_RUN			(1<<0)

//Точки температур
#define PANEL_TEMP_ROOM		(1<<10)
#define PANEL_TEMP_AIR		(1<<11)
#define PANEL_TEMP_FLOOR	(1<<12)

//Точки влажности
#define PANEL_HUM_OUT		(1<<13)
#define PANEL_HUM_ROOM		(1<<14)
#define PANEL_HUM_AIR		(1<<15)

//Надпись System
#define PANEL_SYS_ON		(1<<9)
#define PANEL_SYS_OFF		(1<<10)
#define PANEL_SYS_BLINK		(1<<11)
