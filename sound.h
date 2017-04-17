#pragma once

#include "main.h"

typedef struct
{
	bool		en;
	uint16_t	t;
} SoundData;

SoundData Sound_Data;

void SoundInit();
void SoundUpdate();
void SoundS();
void SoundL();
