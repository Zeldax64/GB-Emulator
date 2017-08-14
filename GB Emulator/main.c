#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "mmu.h"

///////////////////////////////////
/*----- GB Emulator Project -----*/
///////////////////////////////////
/*----- Defines -----*/
#define GB_CLOCK 4194304
#define MAXCYCLES 69905 // GB_CLOCK / Frame Rate (60Hz)


void emulateCycle();

void printFlag() {
	printf("FlagZ: %d\n", (cpu.f & FLAGZ ? 1 : 0));
	printf("FlagN: %d\n", (cpu.f & FLAGN ? 1 : 0));
	printf("FlagH: %d\n", (cpu.f & FLAGH ? 1 : 0));
	printf("FlagC: %d\n", (cpu.f & FLAGC ? 1 : 0));
}

void main() {

	CLEAR_FLAGS;
	cpu.a = 0x81;
	cpu.b = 0x3B;
	//cpu.f |= FLAGC;
	SETr(BIT7, &cpu.b);
	printf("%Resultado 0x%x\n", cpu.b);
	printFlag();
	
	/*
	cpu.a = 0xFF;
	up(&cpu.a);
	printf("%d\n", &cpu.a);
	printf("%d\n", cpu.a);
	*/
}

