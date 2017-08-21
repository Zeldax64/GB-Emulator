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
#include "interrupt.h"

/*----- Defines -----*/
// LCD I/O memory addresses
#define LCD_LCDC 0xFF40
#define LCD_STAT 0xFF41
#define LCD_SCY  0xFF42
#define LCD_SCX  0xFF43
#define LCD_LY   0xFF44

#define IS_LCD_ON (GB_lcd.lcdc & 0x80)

/*----- Function prototypes -----*/
void LCD_update(int16_t cycles);
void LCD_setStatus();
void LCD_drawScanLine();


/*----- LCD struct -----*/
typedef struct gblcd {
	uint8_t lcdc;
	uint8_t stat;
	uint8_t scy;
	uint8_t scx;
	uint8_t ly;

}GB_LCD;

GB_LCD GB_lcd;

#endif
