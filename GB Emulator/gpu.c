#include "gpu.h"

/*
	Not finished
*/
#define SCR_WIDTH 160
#define SCR_HEIGHT 144
#define SCR_DEPTH 4
#define SCR_ELEM 92160 // SCR_WIDTH * SCR_HEIGHT * SCR_DEPTH

uint8_t scr_buffer[SCR_ELEM];
uint8_t tileset[0x100]; // Generic value
/*
	NOTA
	scr_buffer might be optimized.
*/
void GPU_resetSCR() {
	for (uint32_t i = 0; i < SCR_ELEM; i++) {
		scr_buffer[i] = 0xFF;
	}

	for (uint16_t i = 0; i < 384; i++) {
		tileset[i] = 0;
		// tileset[i] = 0 instead of tileset[i][j]
	}
}

void GPU_updateTile(uint16_t addr, uint8_t val) {
	// Get the "base address" for this tile row
	addr &= 0x1FFE;

	// Work out which tile and row was updated
	uint16_t tile = (addr >> 4) & 0x1FF;
	uint16_t y = (addr >> 1) & 0x7;

	uint16_t sx;
	for (uint8_t x = 0; x < 8; x++) {
		// Find bit index for this pixel
		sx = 1 << (7 - x);

		// Update tile set
		tileset
	}
}







/*
	GB's "GPU" has four modes. Each one is accessed
	in sequence (mode). All modes have a time which are spent
	into them (modeclk). This behaviour is implemented
	in GPU_step() switch.
*/

// Change them to static?
uint8_t mode = 0;
int32_t modeclk = 0;
uint8_t line = 0;

/*
	NOTA
	The times used here are base on CPU's clock, and not
	CPU's machine cycles. It's necessary to change them.
*/
void GPU_step() {
	modeclk += cpu.m;

	switch (mode) {
		// OAM read mode, scanline active
		case 2:
			if (modeclk >= 80) {
				modeclk = 0;
				mode = 3;
			}
			break;

		// VRAM read mode, scanline active
		// Treat end of mode 3 as end of scanline
		case 3:
			if (modeclk >= 172) {
				// Enter hblank
				modeclk = 0;
				mode = 0;
			}
			GPU_renderScan();
			break;

		case 0:
			if (modeclk >= 204) {
				modeclk = 0;
				line++;
				if (line == 143) {
					// Enter vblank
					mode = 1;
					//putImageData-> Use OpenGL
				}
				else {
					mode = 2;
				}
			}
			break;

		// Vblank (10 lines)
		case 1:
			if (modeclk >= 456) {
				modeclk = 0;
				line++;
				if (line > 153) {
					// Restart scanning modes
					mode = 2;
					line = 0;
				}
			}
			break;
	}


}