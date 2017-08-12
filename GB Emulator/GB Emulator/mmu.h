#ifndef _MMU
#define _MMU

#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"

/*----- MMU -----*/
/*----- Defines -----*/
#define MEM_SIZE 65535  // Useless?
#define MEM8K 0x1FFF  // Represents 8kB of memory

/*----- Function Prototypes ------*/
uint8_t rdByte(uint16_t addr);
uint16_t rdWord(uint16_t addr);
void wrByte(uint16_t addr, uint8_t val);
void wrWord(uint16_t addr, uint16_t val);

/*----- Variables -----*/
bool inbios;
uint8_t bios[0x100];	// BIOS
uint8_t rom[0x100];		// ROM
uint8_t wram[0x100];	// Working RAM
uint8_t eram[0x100];	// External RAM (Cartridge)
uint8_t zram[0x100];	// Zero page RAM

uint8_t vram[0x100];	// Video RAM
uint8_t oam[0x100];		// OAM RAM

#endif
