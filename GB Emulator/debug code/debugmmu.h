#pragma once

#include "mmu.h"

#include <stdbool.h>
#include <stdint.h>

void DBMMU_zeros(GB_MMU* mmu);
bool DBMMU_testRdWr(GB_MMU* mmu);
bool DBMMU_testWr(GB_MMU* mmu);
bool DBMMU_testRd(GB_MMU* mmu);
