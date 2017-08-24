/*
	Game Boy LCD
	All names given are based on GBCPUman
*/

#ifndef _LCD
#define _LCD

/* General Includes */
#include <stdint.h>
#include <stdbool.h>

/* Emulator Includes */
#include "bit.h"
#include "interrupt.h"
#include "mmu.h"

/*----- Defines -----*/
#define LCD_WIDTH  160
#define LCD_HEIGHT 144

#define WHITE		0xFF
#define LIGHT_GRAY  0x
#define DARK_GRAY
#define BLACK
// VRAM offsets
#define LCD_TILE_DATA_1 0x0
#define LCD_TILE_DATA_2 0x800

#define LCD_BG_MEM_0 0x1800
#define LCD_BG_MEM_1 0x1C00
// LCD registers
#define LCD_LCDC 0xFF40
#define LCD_STAT 0xFF41
#define LCD_SCY  0xFF42
#define LCD_SCX  0xFF43
#define LCD_LY   0xFF44
#define LCD_LYC  0xFF45
#define LCD_BGP  0xFF47
#define LCD_WY	 0xFF4A
#define LCD_WX	 0xFF4B


#define IS_LCD_ON (gb_lcd.lcdc & BIT7)

// Mode defines. Mode transition takes 114 M-Cycles
// or 456 clock cycles.
#define MODE_2 94 // 114 - 20 = 94 (M-Cycles)
#define MODE_3 51 // MODE2 - 43 = 51 (M-Cycles)

/*----- Function prototypes -----*/
void LCD_update(int16_t cycles);
void LCD_setStatus();
void LCD_drawScanLine();
void LCD_renderScreen();
void LCD_renderTiles();
void LCD_renderSprites();
uint8_t LCD_getColour(uint8_t col, uint8_t palette);

/*----- LCD struct -----*/
typedef struct gblcd {
	uint8_t lcdc;
	uint8_t stat;
	uint8_t scy;
	uint8_t scx;
	uint8_t ly;
	uint8_t lyc;
	uint8_t bgp;
	uint8_t wy;
	uint8_t wx;

	uint8_t buffer[LCD_WIDTH * LCD_HEIGHT];
}GB_LCD;

GB_LCD gb_lcd;

#endif
