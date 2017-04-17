#pragma once

#include "main.h"

#define	LED_GREEN	1
#define	LED_RED		2

void LedInit();
void LedOn(int led);
void LedOff(int led);
bool LedIsOn(int led);
