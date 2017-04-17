#pragma once

#include "main.h"

void ROMInit();

#define ROM_READ 1
#define ROM_WRITE 2
#define ROM_WRITE_EN 3
#define ROM_WRITE_DIS 4
#define ROM_WRITE_EN2 5
#define ROM_WRITE_ER 6

void ROMWrite(uint32_t adr, uint16_t cnt);
void ROMRead(uint32_t adr, uint16_t cnt);
void ROMWriteEn();
void ROMWriteDis();
void ROMEraseSector(uint32_t ard);
void ROMEraseFull();
void ROMStat1Read();
void ROMWait();
void ROMCSEn();
void ROMCSDis();
