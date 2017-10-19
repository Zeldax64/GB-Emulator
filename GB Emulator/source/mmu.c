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


void load(char * file){
	/*
	uint8_t sz;
	uint8_t buffer;
	uint8_t i;

	FILE * program = fopen(file, "rb");
	fseek(program, 0L, SEEK_END);
	sz = ftell(program);
	fseek(program, 0L, SEEK_SET);
	fread(&buffer,sz,1,program);

	for(i = 0; i<sz; i++){
		gb_mmu.rom[i] = buffer[i];
	} 
	*/
	/*
		Reads the cartridge type. It's important to know
		if it's necessary to swap rom banks.
		MBC 1:
		1 = 
		2 =
		3 =
		5, and 6 = MBC2
	*/
	gb_mmu.carttype = gb_mmu.rom[0x0147];

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



//Cálculo do offset?
void wrByte(uint16_t addr, uint8_t val) {
	switch (addr & 0xF000) {

	case 0x0000:
	case 0x1000:
		switch (gb_mmu.carttype) {
		case 3:
			if ((val & 0xF) == 0xA) {
				gb_mmu.mbc1.enableERam = 1;
			}
			break;
		}
		break;

	case 0x2000:
	case 0x3000:
		switch (gb_mmu.carttype) {
		case 3:
			val = val & 0x1F;
			if (!val) val = 1;

			gb_mmu.mbc1.rombank = gb_mmu.mbc1.rombank & 0x60; // Por que isso é feito?
			gb_mmu.mbc1.rombank = gb_mmu.mbc1.rombank | val;

			gb_mmu.romoffset = gb_mmu.mbc1.rombank * 0x4000;

			break;
		}
		break;

	case 0x4000:
	case 0x5000:
		switch (gb_mmu.carttype) {
		case 3:
			val = val & 0x3;
			if (gb_mmu.mbc1.mode) {
				gb_mmu.mbc1.rambank = val;
				gb_mmu.ramoffset = gb_mmu.mbc1.rambank * 0x2000;// Por que esse offset?
			}
			else {
				gb_mmu.mbc1.rombank = gb_mmu.mbc1.rombank & 0x1F;
				gb_mmu.mbc1.rombank = gb_mmu.mbc1.rombank | val;
				gb_mmu.romoffset = gb_mmu.mbc1.rombank * 0x4000;
			}
			break;
		}
		break;

	case 0x6000:
	case 0x7000:
		switch (gb_mmu.carttype) {
		case 3:
			val = val & 0x1;
			gb_mmu.mbc1.mode = val;
			break;
		}
		break;

		// Video RAM
	case 0x8000:
	case 0x9000:
		gb_mmu.vram[addr & 0x1FFF] = val;
		//updatetile(addr & 0x1FFF, val); // GPU
		// É necessário implementar a GPU!!
		break;

		// External RAM
	case 0xA000:
	case 0xB000:
		gb_mmu.eram[gb_mmu.ramoffset + (addr & 0x1FFF)] = val;
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

			/* Dependem da GPU e KEY */
			//Sprite Attribute Table
			// OAM
			//Caso implementado a partir do que foi observado no documento "GBCPUman.pdf",
			//porém ao comparar com o código do tutorial, está diferente.
		case 0xE00:
			if ((addr & 0xFF) <= 0x9F) {
				gb_mmu.oam[addr & 0xFF] = val;
				//updateoam(addr, val); // GPU
			}
			break;

			// Zeropage RAM, I/O, interrupts
		case 0xF00:
			if (addr == 0xFFFF)
				gb_mmu.ie = val;
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				gb_mmu.zram[addr & 0x7F] = val;
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