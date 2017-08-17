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
	printf("FlagZ: %d\n", (GB_cpu.f & FLAGZ ? 1 : 0));
	printf("FlagN: %d\n", (GB_cpu.f & FLAGN ? 1 : 0));
	printf("FlagH: %d\n", (GB_cpu.f & FLAGH ? 1 : 0));
	printf("FlagC: %d\n", (GB_cpu.f & FLAGC ? 1 : 0));
}

void main() {

	CLEAR_FLAGS;
	GB_cpu.a = 0x81;
	GB_cpu.b = 0x3B;
	//GB_cpu.f |= FLAGC;
	SETr(BIT7, &GB_cpu.b);
	printf("%Resultado 0x%x\n", GB_cpu.b);
	printFlag();
	
	/*
	GB_cpu.a = 0xFF;
	up(&GB_cpu.a);
	printf("%d\n", &GB_cpu.a);
	printf("%d\n", GB_cpu.a);
	*/
}

