#pragma once

#include "main.h"

uint8_t lcd_spi_buf[8];

void LCDSPI_Init();
void LCDSPI_Write(uint8_t reg, uint8_t data);
void LCDSPICSEn();
void LCDSPICSDis();
