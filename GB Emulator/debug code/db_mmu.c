#include "db_mmu.h"

void DBMMU_init() {
	for (uint16_t i = 0; i < MEM8K; i++) {
		dbgb_mmu.rom[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		dbgb_mmu.wram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		dbgb_mmu.eram[i] = 0;
	}

	for (uint8_t i = 0; i < 0x7F; i++) {
		dbgb_mmu.zram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		dbgb_mmu.vram[i] = 0;
	}

	for (uint8_t i = 0; i < 0xA0; i++) {
		gb_mmu.oam[i] = 0;
	}


}

uint8_t readByte(uint16_t addr) {

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

uint16_t readShort(uint16_t addr) {
	return readByte(addr) + (rdByte(addr + 1) << 8);
}



//Cálculo do offset?
void writeByte(uint16_t addr, uint8_t val) {
	switch (addr & 0xF000) {

	case 0x0000:
	case 0x1000:
		switch (dbgb_mmu.carttype) {
		case 3:
			if ((val & 0xF) == 0xA) {
				dbgb_mmu.mbc1.enableERam = 1;
			}
			break;
		}
		break;

	case 0x2000:
	case 0x3000:
		switch (dbgb_mmu.carttype) {
		case 3:
			val = val & 0x1F;
			if (!val) val = 1;

			dbgb_mmu.mbc1.rombank = dbgb_mmu.mbc1.rombank & 0x60; // Por que isso é feito?
			dbgb_mmu.mbc1.rombank = dbgb_mmu.mbc1.rombank | val;

			dbgb_mmu.romoffset = dbgb_mmu.mbc1.rombank * 0x4000;

			break;
		}
		break;

	case 0x4000:
	case 0x5000:
		switch (dbgb_mmu.carttype) {
		case 3:
			val = val & 0x3;
			if (dbgb_mmu.mbc1.mode) {
				dbgb_mmu.mbc1.rambank = val;
				dbgb_mmu.ramoffset = dbgb_mmu.mbc1.rambank * 0x2000;// Por que esse offset?
			}
			else {
				dbgb_mmu.mbc1.rombank = dbgb_mmu.mbc1.rombank & 0x1F;
				dbgb_mmu.mbc1.rombank = dbgb_mmu.mbc1.rombank | val;
				dbgb_mmu.romoffset = dbgb_mmu.mbc1.rombank * 0x4000;
			}
			break;
		}
		break;

	case 0x6000:
	case 0x7000:
		switch (dbgb_mmu.carttype) {
		case 3:
			val = val & 0x1;
			dbgb_mmu.mbc1.mode = val;
			break;
		}
		break;

		// Video RAM
	case 0x8000:
	case 0x9000:
		dbgb_mmu.vram[addr & 0x1FFF] = val;
		//updatetile(addr & 0x1FFF, val); // GPU
		// É necessário implementar a GPU!!
		break;

		// External RAM
	case 0xA000:
	case 0xB000:
		dbgb_mmu.eram[dbgb_mmu.ramoffset + (addr & 0x1FFF)] = val;
		break;

		// Internal RAM
	case 0xC000:
	case 0xD000:
	case 0xE000:
		dbgb_mmu.wram[addr & 0x1FFF] = val;
		break;

	case 0xF000:
		switch (addr & 0x0F00) {
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			dbgb_mmu.wram[addr & 0x1FFF] = val;
			break;

			/* Dependem da GPU e KEY */
			//Sprite Attribute Table
			// OAM
			//Caso implementado a partir do que foi observado no documento "GBCPUman.pdf",
			//porém ao comparar com o código do tutorial, está diferente.
		case 0xE00:
			if ((addr & 0xFF) <= 0x9F) {
				dbgb_mmu.oam[addr & 0xFF] = val;
				//updateoam(addr, val); // GPU
			}
			break;

			// Zeropage RAM, I/O, interrupts
		case 0xF00:
			if (addr == 0xFFFF)
				dbgb_mmu.ie = val;
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				dbgb_mmu.zram[addr & 0x7F] = val;
			}
			else {
				// I/O control handling
				switch (addr & 0xFFFF) {
					case TIM_DIVR: gb_tim.divr = 0;   break; // 0xFF04
					case TIM_TIMA: gb_tim.tima = val; break; // 0xFF05
					case TIM_TMA:  gb_tim.tma = val;  break; // 0xFF06
					case TIM_TMC:  gb_tim.tmc = val;  break; // 0xFF07
					case INT_REQ:  gb_int.int_req = val; break;  // 0xFF0F
					case LCD_LCDC: gb_lcd.lcdc = val;	  // 0xFF40
					case LCD_STAT: gb_lcd.stat = val;	  // 0xFF41
					case LCD_SCY:  gb_lcd.scy = val;	  // 0xFF42
					case LCD_SCX:  gb_lcd.scx = val;	  // 0xFF43
					case LCD_LY:   gb_lcd.ly = val;	  // 0xFF44
					case LCD_LYC:  gb_lcd.lyc = val;	  // 0xFF45
					case DMA_ADDR: DMA_doDMA(val); break;		 // 0xFF46	
					case LCD_BGP:  gb_lcd.bgp = val;	  // 0xFF47
					case LCD_WY:   gb_lcd.wy = val;	  // 0xFF4A
					case LCD_WX:   gb_lcd.wx = val;	  // 0xFF4B
					case INT_ENA:  gb_int.int_ena = val;  break; // 0xFFFF
				}
			}
		}
	}
}


void writeShort(uint16_t addr, uint16_t val){
	writeByte(addr, (uint8_t) val);
	writeByte(addr + 1, val>>8);
}

uint8_t * DBMMU_getAddr(uint16_t addr) {
	switch (addr & 0xF000) {
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return &dbgb_mmu.rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return &dbgb_mmu.rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return &dbgb_mmu.vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return &dbgb_mmu.eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return &dbgb_mmu.wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return &dbgb_mmu.wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return &dbgb_mmu.wram[addr & MEM8K];
		
		case 0x0E00:
			return &dbgb_mmu.oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			if (addr == 0xFFFF) {
				return &dbgb_mmu.ie;
			}
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return &dbgb_mmu.zram[addr & 0x7F];
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

void writeShortToStack(unsigned short value) {
	registers.sp -= 2;
	writeShort(registers.sp, value);
}

unsigned short readShortFromStack(void) {
	unsigned short value = readShort(registers.sp);
	registers.sp += 2;

	return value;
}