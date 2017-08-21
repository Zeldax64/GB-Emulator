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
		GB_lcd.ly++;
		// NOTA: use of "+=" instead of "="
		scanline_counter += 456;
		
		// NOTA: maybe use a switch statement instead of if-else chain
		if (GB_lcd.ly == 144)
			INT_reqInt(VBLANK_BIT);

		else if (GB_lcd.ly > 153)
			GB_lcd.ly = 0;

		else if (GB_lcd.ly < 144)
			LCD_drawScanLine();
	}
}

void LCD_setStatus() {
	if (IS_LCD_ON == false) {
		// Set LCD mode to 1 during LCD disabled and reset scan line
		scanline_counter += 456;
		GB_lcd.ly = 0;
		GB_lcd.stat &= 0xFC; // 0b11111100
		GB_lcd.stat |= 0x1;  // 0b00000001
		return;
	}


}

void LCD_drawScanLine() {

}
