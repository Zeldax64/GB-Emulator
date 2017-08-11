#include <stdint.h>

#include "mmu.h"

/* Maybe pointers are going to be necessary */
uint8_t rdByte(uint16_t addr) {

	switch (addr & 0xF000) {
	case 0x0000:
		// Bios check
		if (inbios) {
			if (addr < 0x0100)
				return bios[addr];
			else if (cpu.pc == 0x0100)
				inbios = 0;
		}
		// ROM 0
		/* DIFERENTE*/
		//return rom[addr];

	case 0x1000:
	case 0x2000:
	case 0x3000:
		return rom[addr];

		// ROM 1 (Unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return vram[addr & MEM8K];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return eram[addr & MEM8K];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return wram[addr & MEM8K];

		// Working RAM shadow
	case 0xE000:
		return wram[addr & MEM8K];

		// Working RAM shadow, I/O, Zero Page RAM
	case 0xF000:
		switch (addr & 0x0F00) {
			// Working RAM shadow
		case 0x0000: case 0x0100: case 0x0200: case 0x0300:
		case 0x0400: case 0x0500: case 0x0600: case 0x0700:
		case 0x0800: case 0x0900: case 0x0A00: case 0x0B00:
		case 0x0C00: case 0x0D00:
			return wram[addr & MEM8K];

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
			return oam[addr & 0xFF];

			// Zero page
		case 0x0F00:
			// Zero page is 128 bytes
			if (addr >= 0xFF80) {
				return zram[addr & 0x7F];
			}
			else {
				// I/O control handling
				return 0;
			}

		}
	}
}

/*  GB's CPU uses little-endian format, so the
highest address contains the MSB.
*/
uint16_t rdWord(uint16_t addr) {
	return rdByte(addr) + (rdByte(addr + 1) << 8);
}

void wrByte(uint16_t addr, uint8_t val) {

}

void wrWord(uint16_t addr, uint16_t val) {

}