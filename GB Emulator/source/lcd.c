#include "lcd.h"

// Source: http://www.codeslinger.co.uk/pages/projects/gameboy/graphics.html
// Many improvements can be made to speed up this process. This is
// just a temporary solution to test tile emulation.

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
	if (testBit(gb_lcd.lcdc, BIT0))
		LCD_renderTiles();
	
}

void LCD_renderScreen() {
	if (testBit(gb_lcd.lcdc, BIT0))
		LCD_renderTiles();
	if (testBit(gb_lcd.lcdc, BIT1))
		LCD_renderSprites();
	//GFX_render();
	//GFX_display();
}

void LCD_renderTiles() {
	uint16_t tile_data = 0;
	uint16_t background_memory = 0;
	bool unsig = true;

	/////////////////
	// Maybe create a function "checkWindow()"
	// Is the window enabled?
	bool using_window = false;
	if (testBit(gb_lcd.lcdc, BIT5))
		// Is the current scanline we're drawing within the WY pos?
		if (gb_lcd.wy <= gb_lcd.ly)
			using_window = true;

	//////////////////
	// Maybe create a function getTileSet()
	// Which tile data are we using?
	if (testBit(gb_lcd.lcdc, BIT4)) // Bit4 = Background tile set
		tile_data = LCD_TILE_DATA_1;
	else {
		// Important: this memory region uses signed bytes as tile identifiers
		tile_data = LCD_TILE_DATA_2;
		unsig = false;
	}

	///////////////////
	// Which background mem?
	if (using_window == false) {
		if (testBit(gb_lcd.lcdc, BIT3)) // Checks for background tile map
			background_memory = LCD_BG_MEM_1;
		else
			background_memory = LCD_BG_MEM_0;
	}
	else {
		// Which window memory?
		if (testBit(gb_lcd.lcdc, BIT6))
			background_memory = LCD_BG_MEM_1;
		else
			background_memory = LCD_BG_MEM_0;
	}
	/////////////////
	// ypos is used to calculate which of 32 vertical tiles the current scanline is drawing
	uint8_t ypos = 0; 

	if (!using_window)
		ypos = gb_lcd.scy + gb_lcd.ly; // LY-Current scanline
	else
		ypos = gb_lcd.ly - gb_lcd.wy;

	// Which of the 8 vertical pixels of the current is the scanline on?
	uint16_t tile_row = (((uint8_t)ypos / 8) * 32); // ypos *4
	/////////////////
	// Time to start drawing the 160 horizontal pixels for this scanline
	for (uint8_t pixel = 0; pixel < 160; pixel++) {
		uint8_t xpos = pixel + gb_lcd.scx;

		// Translate the current xpos to window space if necessary

		if (using_window)
			if (pixel >= gb_lcd.wx) // If pixel to be drawn is within the window
				xpos = pixel - gb_lcd.wx;

		// Which one of the 32 horizontal tiles does this xpos fall within?

		uint16_t tile_col = xpos / 8;
		int16_t tile_num;

		// Get the tile identity number. Remember it can be signed or unsigned

		uint16_t tileAddress = background_memory + tile_row + tile_col;

		if (unsig)
			tile_num = (uint8_t)gb_mmu.vram[tileAddress];
		else
			tile_num = (int8_t)gb_mmu.vram[tileAddress];
		
		// Deduce where this tile identifier is in memory.
		uint16_t tile_location = tile_data;
		if (unsig)
			tile_location += (tile_num * 16);
		else
			tile_location += (tile_num + 128) * 16;

		// Tile to get tile data from memory
		uint8_t line = ypos % 8;
		line *= 2; // Each vertical line takes up two bytes of memory
		uint8_t data1 = gb_mmu.vram[tile_location + line];
		uint8_t data2 = gb_mmu.vram[tile_location + line + 1];

		// Pixel 0 in the tile is it 7 of data 1 and data 2
		// Pixel 1 is bit 6 etc...

		int colour_bit = xpos % 8;
		colour_bit -= 7;
		colour_bit *= -1;
		
		// Combine data 2 and data 1 to get the colour id for this pixel in the tile
		int colour_num = data2 >> colour_bit;
		colour_num <<= 1;
		colour_num |= data1 >> colour_bit;

		// Colour from palette 0xFF47 (BGP - Background Palette)
		uint8_t col = LCD_getColour(colour_num, gb_lcd.bgp);
		
		// Set pixel into buffer.
		uint16_t pixel_pos = gb_lcd.ly * LCD_WIDTH + pixel;
		gb_lcd.buffer[pixel_pos] = col;
	}
}
void LCD_renderSprites() {
}

uint8_t LCD_getColour(uint8_t col, uint8_t palette)	{
	uint8_t res = WHITE;
	
	uint8_t hi = 0, lo = 0;

	// Which bits of the colour palette does the colour id map to?
	switch (col) {
		case 0: hi = 1; lo = 0; break;
		case 1: hi = 3; lo = 2; break;
		case 2: hi = 5; lo = 4; break;
		case 3: hi = 7; lo = 6; break;
	}

	// use the palette to get the colour;
	uint8_t colour = 0;
	colour = palette >> (hi - 1);
	colour |= palette >> lo;

	switch (colour) {
		case 0: res = WHITE; break;
		case 1: res = LIGHT_GRAY; break;
		case 2: res = DARK_GRAY; break;
		case 3: res = BLACK; break;
	}

	return res;
}
