
#include "mmu.h"

void MMU_init() {
	mmu.ROMbank = 1;
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
		mmu.rom[i] = buffer[i];
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
	mmu.carttype = mmu.rom[0x0147];

}

void reset(){

	//Reset MBC1
	mmu.mbc1.rombank = 0;
	mmu.mbc1.rambank = 0;
	mmu.mbc1.enableERam = 0;
	mmu.mbc1.mode = 0;

	mmu.romoffset = 0x4000;
	mmu.ramoffset = 0x0000;


}

uint8_t rdByte(uint16_t addr) {

	switch (addr & 0xF000) {
	case 0x0000:
		// Bios check
		if (mmu.inbios) {
			if (addr < 0x0100)
				return mmu.bios[addr];
			else if (cpu.pc == 0x0100)
				mmu.inbios = 0;
		}
		// ROM 0
		/* DIFERENTE*/
		//return rom[addr];

	case 0x1000:
	case 0x2000:
	case 0x3000:
		return mmu.rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return mmu.rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return mmu.vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return mmu.eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return mmu.wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return mmu.wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return mmu.wram[addr & MEM8K];

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
			return mmu.oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return mmu.zram[addr & 0x7F];
			}
			else {
				// I/O control handling
				//return ioport[addr & ]
				return 0;
			}
		}
	}
}

uint16_t rdWord(uint16_t addr) {
	return rdByte(addr, mmu, cpu) + (rdByte(addr + 1, mmu, cpu) << 8);
}



//Cálculo do offset?
void wrByte(uint16_t addr, uint8_t val) {
	switch(addr & 0xF){

		case 0x0000:
	    case 0x1000:
			switch(mmu.carttype){
				case 3:
					if((val & 0xF) == 0xA){
						mmu.mbc1.enableERam = 1;
					}
				break;
			}
		break;

		case 0x2000:
	    case 0x3000:
	    	switch(mmu.carttype){
				case 3:
					val = val & 0x1F;
					if(!val) val = 1;
					
					mmu.mbc1.rombank = mmu.mbc1.rombank & 0x60; // Por que isso é feito?
					mmu.mbc1.rombank = mmu.mbc1.rombank | val;

					mmu.romoffset = mmu.mbc1.rombank * 0x4000;

				break;
			}
		break;
		
		case 0x4000:
	    case 0x5000:
	    	switch(mmu.carttype){
				case 3:
					val = val & 0x3;
					if(mmu.mbc1.mode){
						mmu.mbc1.rambank = val;
						mmu.ramoffset = mmu.mbc1.rambank * 0x2000;// Por que esse offset?
					}
					else{
						mmu.mbc1.rombank = mmu.mbc1.rombank & 0x1F;
						mmu.mbc1.rombank = mmu.mbc1.rombank | val;
						mmu.romoffset = mmu.mbc1.rombank * 0x4000;
					}
				break;
			}
		break;

		case 0x6000:
	    case 0x7000:
	    	switch(mmu.carttype){
				case 3:
					val = val & 0x1;
					mmu.mbc1.mode = val;
				break;
			}
		break;

		// Video RAM
		case 0x8000:
	    case 0x9000:
	    	mmu.vram[addr&0x1FFF] = val;
	    	updatetile(addr&0x1FFF, val); // GPU
	    	// É necessário implementar a GPU!!
		break;

		// External RAM
		case 0xA000:
	    case 0xB000:
	    	mmu.eram[mmu.ramoffset + (addr&0x1FFF)] = val;
		break;

		// Internal RAM
		case 0xC000:
	    case 0xD000:
	    case 0xE000:
	    	mmu.wram[addr&0x1FFF] = val;
		break;

		case 0xF000:
			switch(addr&0x0F00){
				case 0x000: case 0x100: case 0x200: case 0x300:
				case 0x400: case 0x500: case 0x600: case 0x700:
				case 0x800: case 0x900: case 0xA00: case 0xB00:
				case 0xC00: case 0xD00:
					mmu.wram[addr&0x1FFF] = val;
				break;

				/* Dependem da GPU e KEY */
				//Sprite Attribute Table
				// OAM
				//Caso implementado a partir do que foi observado no documento "GBCPUman.pdf",
				//porém ao comparar com o código do tutorial, está diferente.
				case 0xE00:
					if(addr&0xFF < 0x9F){
						mmu.oam[addr&0xFF] = val;
						updateoam(addr, val); // GPU
					}
				break;

				// Zeropage RAM, I/O, interrupts
				case 0xF00:
					switch(addr&0xFF){
						case 0xA0:
							//... IO
						case 0x00:
							//... IO
						case 0x4C:
							//... IO
						case 0x80:
							mmu.zram[addr&0x7F] = val;
						break;
					}
				


			}
	}
}


void wrWord(uint16_t addr, uint8_t val){
	wrByte(addr, val);
	wrByte(addr + 1, val>>8);
}
