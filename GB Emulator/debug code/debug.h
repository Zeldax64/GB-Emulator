/*
	General use debug functions
*/
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "debugcpu.h"
#include "debugmmu.h"

#include "db_mmu.h"

#include "cin_cpu.h"

/*----- Function Prototypes -----*/
bool cmpArray8(uint8_t * ar1, uint8_t * ar2, uint32_t end);
void matchMMU();
void debugCycle();

