///////////////////////////////////
/*----- GB Emulator Project -----*/
///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "mmu.h"

/*----- Defines -----*/
#define GB_CLOCK 4194304
#define MAXCYCLES 69905 // GB_CLOCK / Frame Rate (60Hz)

bool loadROM(char * filename) {
	uint32_t size;
	uint8_t *buffer;

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
	return true;
}

void printFlag() {
	printf("FlagZ: %d\n", (GB_cpu.f & FLAGZ ? 1 : 0));
	printf("FlagN: %d\n", (GB_cpu.f & FLAGN ? 1 : 0));
	printf("FlagH: %d\n", (GB_cpu.f & FLAGH ? 1 : 0));
	printf("FlagC: %d\n", (GB_cpu.f & FLAGC ? 1 : 0));
}

void printOAM() {
	for (uint8_t i = 0; i < 0xA0; i++) {
		printf("%x: %x\n", i, GB_mmu.oam[i]);
	}
}

/*
	Compare arrays function.
*/
bool cmpArray8(uint8_t *ar1, uint8_t *ar2, uint32_t end) {
	for (uint32_t i = 0; i < end; i++) {
		if (*(ar1 + i) != *(ar2 + i)) {
		//if (*ar1 != *ar2) {

			printf("Error in value %d\nar1: %d   ar2: %d\n", i, *(ar1 + i), *(ar2 + i));
			return false;
		}
		//ar1++; ar2++;
	}
	printf("Identical arrays!\n");
	return true;
}

void main() {
	char *filename = "Z:\\Jogos\\GB\\Roms\\Tetris\\Tetris.gb";
	
	MMU_init();

	if (loadROM(filename)) {
		printf("Load success!\n");
	}

	uint16_t addr = 0x0;
	DMA_doDMA(addr);
	addr <<= 8;
	printOAM();
	uint8_t *src = MMU_getAddr(addr);
	uint8_t *dst = MMU_getAddr(0xFE00);
	printf("%d", cmpArray8(src, dst, 0xA0));
}

