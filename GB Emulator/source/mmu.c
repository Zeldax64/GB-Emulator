#include "mmu.h"

void MMU_init() {
	for (uint16_t i = 0; i < MEM8K; i++) {
		gb_mmu.rom[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		gb_mmu.wram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		gb_mmu.eram[i] = 0;
	}

	for (uint8_t i = 0; i < 0x7F; i++) {
		gb_mmu.zram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		gb_mmu.vram[i] = 0;
	}

	for (uint8_t i = 0; i < 0xA0; i++) {
		gb_mmu.oam[i] = 0;
	}

	wrByte(TIM_TIMA, 0);
	wrByte(TIM_TMA, 0);
	wrByte(TIM_TMC, 0);
	wrByte(0xFF10, 0x80);
	wrByte(0xFF11, 0xBF);
	wrByte(0xFF12, 0xF3);
	wrByte(0xFF14, 0xBF);
	wrByte(0xFF16, 0x3F);
	wrByte(0xFF17, 0x00);
	wrByte(0xFF19, 0xBF);
	wrByte(0xFF1A, 0x7F); // Modified
	wrByte(0xFF1B, 0xFF);
	wrByte(0xFF1C, 0x9F);
	wrByte(0xFF1E, 0xBF);
	wrByte(0xFF20, 0xFF);
	wrByte(0xFF21, 0x00);
	wrByte(0xFF22, 0x00);
	wrByte(0xFF23, 0xBF);
	wrByte(0xFF24, 0x77);
	wrByte(0xFF25, 0xF3);
	wrByte(0xFF26, 0xF1);
	wrByte(LCD_LCDC, 0x91);
	wrByte(LCD_SCY, 0x00);
	wrByte(LCD_SCX, 0x00);
	wrByte(LCD_LYC, 0x00);
	wrByte(LCD_BGP, 0xFC);
	wrByte(0xFF48, 0xFF);
	wrByte(0xFF49, 0xFF);
	wrByte(LCD_WY, 0x00);
	wrByte(LCD_WX, 0x00);
	wrByte(INT_ENA, 0x00);
}


bool loadROM(char * filename) {
	uint32_t size;
	uint8_t *buffer;

	gameName = filename;

	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		printf("File couldn't be found\n");
		return false;
	}

	// Check file size
	fseek(file, 0L, SEEK_END);
	size = ftell(file);

	// Return file pointer to begin of file
	rewind(file);

	//Check the value of size and adapt it
	if (size > 0x8000) {
		size = 0x8000;
	}

	buffer = (uint8_t*)malloc(sizeof(uint8_t) * size);
	if (buffer == NULL) {
		printf("Memory error\n");
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread(buffer, 1, size, file);

	if (result != size) {
		printf("Reading error\n");
		return false;
	}

	for (uint32_t i = 0; i < size; i++) {
		gb_mmu.rom[i] = buffer[i];
	}


	fclose(file);
	free(buffer);

	gb_mmu.carttype = gb_mmu.rom[0x0147];

	return true;

}


// Nota: Fix warnings!
void reset(){

	//Reset MBC1
	gb_mmu.mbc1.rombank = 0;
	gb_mmu.mbc1.rambank = 0;
	gb_mmu.mbc1.enableERam = 0;
	gb_mmu.mbc1.mode = 0;

	gb_mmu.romoffset = 0x4000;
	gb_mmu.ramoffset = 0x0000;


}

uint8_t rdByte(uint16_t addr) {

	switch (addr & 0xF000) {
	case 0x0000:
		// Bios check
		if (gb_mmu.inbios) {
			if (addr < 0x0100)
				return gb_mmu.bios[addr];
			else if (gb_cpu.pc == 0x0100)
				gb_mmu.inbios = 0;
		}
		// ROM 0
		/* DIFERENTE*/
		//return rom[addr];

	case 0x1000:
	case 0x2000:
	case 0x3000:
		return gb_mmu.rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return gb_mmu.rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return gb_mmu.vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return gb_mmu.eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return gb_mmu.wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return gb_mmu.wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return gb_mmu.wram[addr & MEM8K];

			// Graphics: object attribute memory (OAM)
			// OAM is 160 bytes, remaining bytes read as 0
			/*CHANGING NOTE*/
			/*
			1) The emulator will start with 0 in all memory
			positions. Thus, this if isn't necessary
			case 0x0E00:
			if(addr < 0xFEA0)
			return oam[addr & 0xFF];
			else return 0;
			*/
			// Graphics: object attribute memory (OAM)
			// OAM is 160 bytes (A0), remaining bytes read as 0

		case 0x0E00:
			return gb_mmu.oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			if (addr == 0xFFFF) {
				return gb_mmu.ie;
			}
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return gb_mmu.zram[addr & 0x7F];
			}
			else {
				// I/O control handling
				switch (addr & 0xFFFF) {
					case TIM_DIVR: return gb_tim.divr;	  // 0xFF04
					case TIM_TIMA: return gb_tim.tima;	  // 0xFF05
					case TIM_TMA:  return gb_tim.tma;	  // 0xFF06
					case TIM_TMC:  return gb_tim.tmc;	  // 0xFF07
					case INT_REQ:  return gb_int.int_req; // 0xFF0F
					case LCD_LCDC: return gb_lcd.lcdc;	  // 0xFF40
					case LCD_STAT: return gb_lcd.stat;	  // 0xFF41
					case LCD_SCY:  return gb_lcd.scy;	  // 0xFF42
					case LCD_SCX:  return gb_lcd.scx;	  // 0xFF43
					case LCD_LY:   return gb_lcd.ly;	  // 0xFF44
					case LCD_LYC:  return gb_lcd.lyc;	  // 0xFF45
					case LCD_BGP:  return gb_lcd.bgp;	  // 0xFF47
					case LCD_WY:   return gb_lcd.wy;	  // 0xFF4A
					case LCD_WX:   return gb_lcd.wx;	  // 0xFF4B
					case INT_ENA:  return gb_int.int_ena; // 0xFFFF
				}
				return 0;
			}
		}
	}
}

uint16_t rdWord(uint16_t addr) {
	return rdByte(addr) + (rdByte(addr + 1) << 8);
}

void wrByte(uint16_t addr, uint8_t val) {
	switch (addr & 0xF000) {
	// Enable or Disable RAM
	case 0x0000:
	case 0x1000:
		switch (gb_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			if ((val & 0xF) == 0xA) {
				gb_mmu.enableERam = 1;
			}
			break;
		case 5:
		case 6:
			if ((val & 0xF) == 0xA && !testBit(addr, BIT3)) { // TestBit -> addr & 1000
				gb_mmu.enableERam = 1;
			}

		}
		break;
	//Change the ROM Bank
	case 0x2000:
	case 0x3000:
		switch (gb_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			val = val & 0x1F;
			if (!val) val = 1;

			gb_mmu.rombank = gb_mmu.rombank & 0x60;
			gb_mmu.rombank = gb_mmu.rombank | val;

			gb_mmu.romoffset = gb_mmu.rombank * 0x4000;

			if (gb_mmu.rombank == 0) gb_mmu.rombank = 1;
			swapROM();
			break;
		case 5:
		case 6:
			gb_mmu.rombank = val & 0xF;
			if (gb_mmu.rombank == 0) gb_mmu.rombank = 1;
			swapROM();
			break;
		}
		break;

	case 0x4000:
	case 0x5000:
		switch (gb_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			val = val & 0x3;
			// Change the RAM Bank
			if (gb_mmu.mode) {
				gb_mmu.rambank = val;
				gb_mmu.ramoffset = gb_mmu.rambank * 0x2000;
			}
			// change the ROM Bank [PROBLEM] [Using the GCPUMan Tutorial]
			else {
				gb_mmu.rombank = gb_mmu.rombank & 0x3F; // Cleaning the 2 MSB
				val = (val & 0x3)<<6; // Cleaning the 6 LSB and shift
				gb_mmu.rombank = gb_mmu.rombank | val;
				gb_mmu.romoffset = gb_mmu.rombank * 0x4000;
				swapROM();
			}
			break;
		}
		break;
	//Change mode (Just for MBC1)
	case 0x6000:
	case 0x7000:
		switch (gb_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			val = val & 0x1;
			gb_mmu.mode = val;
			break;
		}
		break;

		// Video RAM
	case 0x8000:
	case 0x9000:
		gb_mmu.vram[addr & 0x1FFF] = val;
		//updatetile(addr & 0x1FFF, val); // GPU
		//GPU is necessary!!
		break;

		// External RAM
	case 0xA000:
	case 0xB000:
		if (gb_mmu.enableERam){
			gb_mmu.eram[gb_mmu.ramoffset + (addr & 0x1FFF)] = val;
		}
		break;

		// Internal RAM
	case 0xC000:
	case 0xD000:
	case 0xE000:
		gb_mmu.wram[addr & 0x1FFF] = val;
		break;

	case 0xF000:
		switch (addr & 0x0F00) {
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			gb_mmu.wram[addr & 0x1FFF] = val;
			break;

		//Sprite Attribute Table
		// OAM
		case 0xE00:
			if ((addr & 0xFF) < 0xA0) {
				gb_mmu.oam[addr & 0xFF] = val;
				//updateoam(addr, val); // GPU
			}
			break;

			// Zeropage RAM, I/O, interrupts
		case 0xF00:
			if (addr == 0xFFFF)
				gb_mmu.ie = val;
			// Zero page is 128 bytes
			if (addr >= 0xFF7F) {
				gb_mmu.zram[addr & 0x7F] = val;
			}
			else {
				// I/O control handling
				switch (addr & 0xFFFF) {
				//case 0x0000: Do something with keys.
				case TIM_DIVR: GB_tim.divr = 0;   break; // 0xFF04
				case TIM_TIMA: GB_tim.tima = val; break; // 0xFF05
				case TIM_TMA:  GB_tim.tma = val;  break; // 0xFF06
				case TIM_TMC:  GB_tim.tmc = val;  break; // 0xFF07
				case INT_REQ:  GB_int.int_req = val; break;  // 0xFF0F
				case DMA_ADDR: DMA_doDMA(val); break;		 // 0xFF46	
				case INT_ENA:  GB_int.int_ena = val;  break; // 0xFFFF
				case LCD_BGP:  gb_lcd.bgp = val;	  // 0xFF47
				case LCD_WY:   gb_lcd.wy = val;	  // 0xFF4A
				case LCD_WX:   gb_lcd.wx = val;	  // 0xFF4B
				case INT_ENA:  gb_int.int_ena = val;  break; // 0xFFFF
				// It's necessary to implement more written about the GPU.
				}
			}
		}
	}
}


void wrWord(uint16_t addr, uint16_t val){
	wrByte(addr, (uint8_t) val);
	wrByte(addr + 1, val>>8);
}

uint8_t * MMU_getAddr(uint16_t addr) {
	switch (addr & 0xF000) {
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return &gb_mmu.rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return &gb_mmu.rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return &gb_mmu.vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return &gb_mmu.eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return &gb_mmu.wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return &gb_mmu.wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return &gb_mmu.wram[addr & MEM8K];
		
		case 0x0E00:
			return &gb_mmu.oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			if (addr == 0xFFFF) {
				return &gb_mmu.ie;
			}
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return &gb_mmu.zram[addr & 0x7F];
			}
			else {
				// I/O control handling
				switch (addr & 0xFFFF) {
				case TIM_DIVR: return &gb_tim.divr;	   // 0xFF04
				case TIM_TIMA: return &gb_tim.tima;	   // 0xFF05
				case TIM_TMA:  return &gb_tim.tma;	   // 0xFF06
				case TIM_TMC:  return &gb_tim.tmc;	   // 0xFF07
				case INT_REQ:  return &gb_int.int_req; // 0xFF0F
				case LCD_LCDC: return &gb_lcd.lcdc;	   // 0xFF40
				case LCD_STAT: return &gb_lcd.stat;	   // 0xFF41
				case LCD_SCY:  return &gb_lcd.scy;	   // 0xFF42
				case LCD_SCX:  return &gb_lcd.scx;	   // 0xFF43
				case LCD_LY:   return &gb_lcd.ly;	   // 0xFF44
				case LCD_LYC:  return &gb_lcd.lyc;	   // 0xFF45
				case LCD_BGP:  return &gb_lcd.bgp;	   // 0xFF47
				case LCD_WY:   return &gb_lcd.wy;	   // 0xFF4A
				case LCD_WX:   return &gb_lcd.wx;	   // 0xFF4B
				case INT_ENA:  return &gb_int.int_ena; // 0xFFFF
				}
			}
		}
	}
}

uint8_t * getAtAddr(uint16_t addr, uint16_t size) {

	uint8_t * buffer;

	buffer = (uint8_t*)malloc(sizeof(uint8_t) * size);

	FILE *file = fopen(gameName, "rb");

	if (file == NULL) {
		printf("File couldn't be found\n");
		return NULL;
	}

	fseek(file, addr, SEEK_SET);

	size_t result = fread(buffer, 1, size, file);

	if (result != size) {
		printf("Reading error\n");
		return NULL;
	}

	return buffer;
}

//"mmu.rom" divided in two parts
void swapROM() {
	uint16_t i;
	uint8_t * buffer;
	if (gb_mmu.rombank == 0) {
		buffer = getAtAddr(0x4000, 0x4000);
	}
	else {
		buffer = getAtAddr(gb_mmu.rombank * 0x4000, 0x4000);
	}

	for (i = 0; i<0x4000; i++) {
		gb_mmu.rom[i + 0x4000] = buffer[i];
	}

}
