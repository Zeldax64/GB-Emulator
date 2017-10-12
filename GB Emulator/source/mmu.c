#include "mmu.h"

void MMU_init() {
	for (uint16_t i = 0; i < MEM8K; i++) {
		GB_mmu.rom[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		GB_mmu.wram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		GB_mmu.eram[i] = 0;
	}

	for (uint8_t i = 0; i < 0x7F; i++) {
		GB_mmu.zram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		GB_mmu.vram[i] = 0;
	}

	for (uint8_t i = 0; i < 0xA0; i++) {
		GB_mmu.oam[i] = 0;
	}

	reset();

}


bool loadROM(char * filename){
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
		GB_mmu.rom[i] = buffer[i];
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
	if(size > 0x8000){
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
		GB_mmu.rom[i] = buffer[i];
	}

	
	fclose(file);
	free(buffer);

	GB_mmu.carttype = GB_mmu.rom[0x0147];

	return true;

}

// Nota: Fix warnings!
void reset(){

	//Reset MBC1
	GB_mmu.rombank = 1;
	GB_mmu.rambank = 0;
	GB_mmu.enableERam = 0;
	GB_mmu.mode = 0;

	GB_mmu.romoffset = 0x4000;
	GB_mmu.ramoffset = 0x0000;


}

uint8_t rdByte(uint16_t addr) {

	switch (addr & 0xF000) {
	case 0x0000:
		// Bios check
		if (GB_mmu.inbios) {
			if (addr < 0x0100)
				return GB_mmu.bios[addr];
			else if (GB_cpu.pc == 0x0100)
				GB_mmu.inbios = 0;
		}
		// ROM 0
		/* DIFERENTE*/
		//return rom[addr];

	case 0x1000:
	case 0x2000:
	case 0x3000:
		return GB_mmu.rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return GB_mmu.rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return GB_mmu.vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return GB_mmu.eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return GB_mmu.wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return GB_mmu.wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return GB_mmu.wram[addr & MEM8K];

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
			return GB_mmu.oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			if (addr == 0xFFFF) {
				return GB_mmu.ie;
			}
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return GB_mmu.zram[addr & 0x7F];
			}
			else {
				// I/O control handling
				switch (addr & 0xFF) {
					case TIM_DIVR: return GB_tim.divr;	  // 0xFF04
					case TIM_TIMA: return GB_tim.tima;	  // 0xFF05
					case TIM_TMA:  return GB_tim.tma;	  // 0xFF06
					case TIM_TMC:  return GB_tim.tmc;	  // 0xFF07
					case INT_REQ:  return GB_int.int_req; // 0xFF0F
					case INT_ENA:  return GB_int.int_ena; // 0xFFFF
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
	switch (addr & 0xF) {

	case 0x0000:
	case 0x1000:
		switch (GB_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			if ((val & 0xF) == 0xA) {
				GB_mmu.enableERam = 1;
			}
		break;
		//GBCPUman!=
		case 5:
		case 6:
			if ((val & 0xF) == 0xA && testBit(addr, 4)) {
				GB_mmu.enableERam = 1;
			}

		}
	break;

	case 0x2000:
	case 0x3000:
		switch (GB_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			val = val & 0x1F;
			if (!val) val = 1;

			GB_mmu.rombank = GB_mmu.rombank & 0x60; 
			GB_mmu.rombank = GB_mmu.rombank | val;

			GB_mmu.romoffset = GB_mmu.rombank * 0x4000;

			if(GB_mmu.rombank == 0) GB_mmu.rombank = 1;
			switchROM();
		break;
		case 5:
		case 6:
			GB_mmu.rombank = val & 0xF;
			if(GB_mmu.rombank == 0) GB_mmu.rombank = 1;
			switchROM();
		break;
		}
	break;

	case 0x4000:
	case 0x5000:
		switch (GB_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			val = val & 0x3;
			if (GB_mmu.mode) {
				GB_mmu.rambank = val;
				GB_mmu.ramoffset = GB_mmu.rambank * 0x2000;// Por que esse offset?
			}
			else {
				GB_mmu.rombank = GB_mmu.rombank & 0x1F;
				GB_mmu.rombank = GB_mmu.rombank | val;
				GB_mmu.romoffset = GB_mmu.rombank * 0x4000;
				switchROM();
			}
			break;
		}
		break;

	case 0x6000:
	case 0x7000:
		switch (GB_mmu.carttype) {
		case 1:
		case 2:
		case 3:
			val = val & 0x1;
			GB_mmu.mode = val;
			break;
		}
		break;

		// Video RAM
	case 0x8000:
	case 0x9000:
		GB_mmu.vram[addr & 0x1FFF] = val;
		//updatetile(addr & 0x1FFF, val); // GPU
		// É necessário implementar a GPU!!
		break;

		// External RAM
	case 0xA000:
	case 0xB000:
		GB_mmu.eram[GB_mmu.ramoffset + (addr & 0x1FFF)] = val;
		break;

		// Internal RAM
	case 0xC000:
	case 0xD000:
	case 0xE000:
		GB_mmu.wram[addr & 0x1FFF] = val;
		break;

	case 0xF000:
		switch (addr & 0x0F00) {
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			GB_mmu.wram[addr & 0x1FFF] = val;
			break;

			/* Dependem da GPU e KEY */
			//Sprite Attribute Table
			// OAM
			//Caso implementado a partir do que foi observado no documento "GBCPUman.pdf",
			//porém ao comparar com o código do tutorial, está diferente.
		case 0xE00:
			if ((addr & 0xFF) < 0x9F) {
				GB_mmu.oam[addr & 0xFF] = val;
				//updateoam(addr, val); // GPU
			}
			break;

			// Zeropage RAM, I/O, interrupts
		case 0xF00:
			if (addr == 0xFFFF)
				GB_mmu.ie = val;
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				GB_mmu.zram[addr & 0x7F] = val;
			}
			else {
				// I/O control handling
				switch (addr & 0xFF) {
					case TIM_DIVR: GB_tim.divr = 0;   break; // 0xFF04
					case TIM_TIMA: GB_tim.tima = val; break; // 0xFF05
					case TIM_TMA:  GB_tim.tma = val;  break; // 0xFF06
					case TIM_TMC:  GB_tim.tmc = val;  break; // 0xFF07
					case INT_REQ:  GB_int.int_req = val; break;  // 0xFF0F
					case DMA_ADDR: DMA_doDMA(val); break;		 // 0xFF46	
					case INT_ENA:  GB_int.int_ena = val;  break; // 0xFFFF
				}
			}
		}
	}
}


void wrWord(uint16_t addr, uint16_t val){
	wrByte(addr, (uint8_t) val);
	wrByte(addr + 1, val>>8);
}

uint8_t* MMU_getAddr(uint16_t addr) {
	switch (addr & 0xF000) {
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return &GB_mmu.rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return &GB_mmu.rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return &GB_mmu.vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return &GB_mmu.eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return &GB_mmu.wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return &GB_mmu.wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return &GB_mmu.wram[addr & MEM8K];
		
		case 0x0E00:
			return &GB_mmu.oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			if (addr == 0xFFFF) {
				return &GB_mmu.ie;
			}
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return &GB_mmu.zram[addr & 0x7F];
			}
			else {
				// I/O control handling
				switch (addr & 0xFF) {
				case TIM_DIVR: return &GB_tim.divr;	  // 0xFF04
				case TIM_TIMA: return &GB_tim.tima;	  // 0xFF05
				case TIM_TMA:  return &GB_tim.tma;	  // 0xFF06
				case TIM_TMC:  return &GB_tim.tmc;	  // 0xFF07
				case INT_REQ:  return &GB_int.int_req; // 0xFF0F
				case INT_ENA:  return &GB_int.int_ena; // 0xFFFF
				}
			}
		}
	}
}

bool testBit(uint16_t addr, uint8_t bit){

	return (addr>>(bit-1)) & 1;
}

uint8_t * getAtAddr(uint16_t addr, uint16_t size){

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
void switchROM(){
	uint16_t i;
	uint8_t * buffer;
	if(GB_mmu.rombank == 0){
		buffer = getAtAddr(0x4000,0x4000);
	}
	else{
		buffer = getAtAddr(GB_mmu.rombank*0x4000,0x4000);
	}

	for(i=0;i<0x4000;i++){
		GB_mmu.rom[i+0x4000] = buffer[i];
	}
	
}
