///////////////////////////////////
/*----- GB Emulator Project -----*/
///////////////////////////////////

/* General Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* SDL Includes */
#include <SDL.h>


/* Emulator Includes */
#include "cpu.h"
#include "mmu.h"

/* Debug Includes */
#include "debug.h"

/* Defines */


/*---------*/

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
		gb_mmu.rom[i] = buffer[i];
	}

	
	fclose(file);
	free(buffer);
	return true;
}

void printFlag() {
	printf("FlagZ: %d\n", (gb_cpu.f & FLAGZ ? 1 : 0));
	printf("FlagN: %d\n", (gb_cpu.f & FLAGN ? 1 : 0));
	printf("FlagH: %d\n", (gb_cpu.f & FLAGH ? 1 : 0));
	printf("FlagC: %d\n", (gb_cpu.f & FLAGC ? 1 : 0));
}

void printOAM() {
	for (uint8_t i = 0; i < 0xA0; i++) {
		printf("%x: %x\n", i, gb_mmu.oam[i]);
	}
}

void emulatorInit(void) {
	CPU_init();
	MMU_init();
}

void emulateCycle(void) {
	const int MAXCYCLES = 17477; // Machine Cycles
	int cycles_this_update = 0;
	while (cycles_this_update < MAXCYCLES) {
		CPU_cycle();
		cycles_this_update += gb_cpu.m;
		TIM_updateTimers(gb_cpu.m);
		LCD_update(gb_cpu.m);
		INT_doInt();
	}
	LCD_renderScreen();
}

char* filename = "C:\\Users\\caiox\\Documents\\Emulators\\GB\\ROMs\\Tetris\\Tetris.gb";

void main() {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	/*
	loadROM(filename);
	emulatorInit();
	debugCycle();
	/*
	while (true)
		emulateCycle();
	*/
}

