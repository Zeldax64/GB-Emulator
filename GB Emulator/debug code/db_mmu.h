///////////////////////////////////
/*----- GB Emulator Project -----*/
///////////////////////////////////
/*----------- MEMORY ------------*/
///////////////////////////////////

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "timer.h"
#include "dma.h"
#include "interrupt.h"

#include "cin_registers.h"

#define MEM8K 0x1FFF  // Represents 8kB of memory

/*----- Function Prototypes -----*/
void DBMMU_init();
uint8_t readByte(uint16_t addr);
uint16_t readShort(uint16_t addr);
void writeByte(uint16_t addr, uint8_t val);
void writeShort(uint16_t addr, uint16_t val);
uint8_t * DBMMU_getAddr(uint16_t addr);
unsigned short readShortFromStack(void);
void writeShortToStack(unsigned short value);

/*----- Structs -----*/
typedef struct{
	uint8_t rombank; // Selected ROM bank
	uint8_t rambank; // Selected RAM bank
	bool enableERam; //Is External RAM enable?
	bool mode; // 16/8 or 4/32
}DBMBC1;

typedef struct{

	DBMBC1 mbc1;

	bool inbios; 
	bool ie; // Interrupt Enable Register - 0xFFFF

	uint8_t carttype;
	uint8_t ROMbank;

	uint8_t bios[0x100];   // BIOS
	uint8_t rom[0x8000];   // ROM
	uint8_t wram[0x2000];  // Working RAM
	uint8_t eram[0x2000];  // External RAM (Cartridge)
	uint8_t zram[0x7F];    // Zero page RAM
	uint8_t vram[0x2000];  // Video RAM - GPU?
	uint8_t oam[0xA0];	   // OAM RAM - GPU?

	uint8_t romoffset;
	uint8_t ramoffset;


}DBGB_MMU;

DBGB_MMU dbgb_mmu;