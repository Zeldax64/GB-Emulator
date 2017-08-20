#ifndef _DMA
#define _DMA

#include "mmu.h"

/*----- Defines -----*/
#define DMA_ADDR 0xFF46

/*----- Function Prototypes -----*/
void DMA_doDMA(uint16_t addr);

#endif