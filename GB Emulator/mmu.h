///////////////////////////////////
/*----- GB Emulator Project -----*/
///////////////////////////////////
/*----------- MEMORY ------------*/
///////////////////////////////////

#ifndef _MMU
#define _MMU

#include "cpu.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE 65535  // Useless?
#define MEM8K 0x1FFF  // Represents 8kB of memory

typedef struct{
	uint8_t rombank; // Selected ROM bank
	uint8_t rambank; // Selected RAM bank
	bool enableERam; //Is External RAM enable?
	bool mode; // 16/8 or 4/32
}MBC1;

typedef struct{

	MBC1 mbc1;

	bool inbios; 
	bool interruptEnabel;

	uint8_t carttype;
	uint8_t bios[0x100]; // BIOS
	uint8_t rom[0x100];	// ROM
	uint8_t wram[0x100]; // Working RAM
	uint8_t eram[0x100]; // External RAM (Cartridge)
	uint8_t zram[0x100]; // Zero page RAM
	uint8_t vram[0x100]; // Video RAM - GPU?
	uint8_t oam[0x100];	// OAM RAM - GPU?

	uint8_t romoffset;
	uint8_t ramoffset;


}MMU;
MMU mmu;

void load(char * file);
void reset();
uint8_t rdByte(uint16_t addr);
uint16_t rdWord(uint16_t addr);
void wrByte(uint16_t addr, uint8_t val);
void wrWord(uint16_t addr, uint8_t val);

#endif
