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
#include "graphics.h"

/* Debug Includes */
#include "debug.h"
#include "debugcpu.h"
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

int cycles_this_update = 0;

void emulateCycle(void) {
	const int MAXCYCLES = 17477; // Machine Cycles
	while (cycles_this_update < MAXCYCLES) {
		printf("--- Cycle: %d ---\n", cycles_this_update);
		getchar();  // Wait for input (enter)
		genReport(false, true); // Generate report
		CPU_cycle(); // Runs one cycle
		cycles_this_update += gb_cpu.m;
		gb_cpu.m = 0;
		TIM_updateTimers(gb_cpu.m);
		LCD_update(gb_cpu.m);
		INT_doInt();
		printf("-----------------\n");
	}
	LCD_renderScreen();
	cycles_this_update -= MAXCYCLES;
}

//char* filename = "C:\\Users\\caiox\\Documents\\Emulators\\GB\\ROMs\\Tetris\\Tetris.gb";
//char* filename = "Tetris.gb";
char* filename = "gbbios.gb";

int main(int argc, char *argv[]) {
	//GFX_init();
	emulatorInit();
	gb_cpu.pc = 0x0;
	loadROM(filename);
	//debugCycle();
	
	while (true)
		emulateCycle();

	return 0;
}

