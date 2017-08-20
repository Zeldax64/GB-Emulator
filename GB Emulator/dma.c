#include "dma.h"

void DMA_doDMA(uint16_t addr) {
	addr <<= 8;

	uint8_t *src = MMU_getAddr(addr);
	uint8_t *dst = MMU_getAddr(0xFE00); // OAM start address

	for (uint8_t i = 0; i < 0xA0; i++) {
		*dst = *src;
		dst++; src++;
	}
}