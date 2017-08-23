#include "lcd.h"

/*----- Variables -----*/
static int16_t scanline_counter;

/*----- Functions -----*/

void LCD_update(int16_t cycles) {
	LCD_setStatus();

	if (IS_LCD_ON)
		scanline_counter -= cycles;
	else
		return;

	// Move to next line
	if (scanline_counter <= 0) {
		gb_lcd.ly++;
		// NOTA: use of "+=" instead of "=" and M-cycle instead of clock cycle
		scanline_counter += 114;
		
		if (gb_lcd.ly == 144)
			INT_reqInt(VBLANK_BIT);

		else if (gb_lcd.ly > 153)
			gb_lcd.ly = 0;

		else if (gb_lcd.ly < 144)
			LCD_drawScanLine();
	}
}

void LCD_setStatus() {
	if (IS_LCD_ON == false) {
		// Set LCD mode to 1 during LCD disabled and reset scan line
		scanline_counter = 456;
		gb_lcd.ly = 0;
		gb_lcd.stat &= 0xFC; // 0b11111100
		gb_lcd.stat |= BIT0; // 0b00000001
		return;
	}

	uint8_t current_mode = gb_lcd.stat & 0x3;
	uint8_t mode = 0;

	uint8_t req_int = 0;

	if (gb_lcd.ly >= 144) { // Mode 01: VBlank
		mode = 1;
		gb_lcd.stat = setBit(gb_lcd.stat, BIT0);
		gb_lcd.stat = resetBit(gb_lcd.stat, BIT1);
		req_int = testBit(gb_lcd.stat, BIT4);
	}

	else {
		if (scanline_counter >= MODE_2) { // Mode 2
			mode = 2;
			gb_lcd.stat = setBit(gb_lcd.stat, BIT1);
			gb_lcd.stat = resetBit(gb_lcd.stat, BIT0);
			req_int = testBit(gb_lcd.stat, BIT5);
		}

		else if (scanline_counter >= MODE_3) { // Mode 3
			mode = 3;
			gb_lcd.stat = setBit(gb_lcd.stat, BIT1);
			gb_lcd.stat = resetBit(gb_lcd.stat, BIT0);
		}

		else { // Mode 0
			mode = 0;
			gb_lcd.stat = resetBit(gb_lcd.stat, BIT1);
			gb_lcd.stat = resetBit(gb_lcd.stat, BIT0);
			req_int = testBit(gb_lcd.stat, BIT3);
		}
	}

	// If new mode entered, request interrupt
	if (req_int && (mode != (gb_lcd.stat & 0b11)))
		INT_reqInt(LCD_BIT);

	// Check coincidence flag
	if (gb_lcd.ly == gb_lcd.lyc) {
		gb_lcd.stat = setBit(gb_lcd.stat, BIT2);
		if (testBit(gb_lcd.stat, BIT6))
			INT_reqInt(LCD_BIT);
	}

	else
		gb_lcd.stat = resetBit(gb_lcd.stat, BIT2);
}

void LCD_drawScanLine() {

}
