#ifndef _DEBUGCPU
#define _DEBUGCPU

#include <stdio.h>
#include <stdbool.h>

#include "bit.h"
#include "cpu.h"
#include "mmu.h"

#include "cin_cpu.h"
#include "cin_registers.h"
#include "db_mmu.h"

void printReg(void);
void printDbgReg(void);
void printMem(uint16_t pc);
void genReport(bool both_cpu, bool complete);
bool cmpReg();
bool cmpMem();
void gbCompare();
void DBCPU_cycle(void);

#endif