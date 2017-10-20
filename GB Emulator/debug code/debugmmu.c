#include "debugmmu.h"


void DBMMU_zeros(GB_MMU* mmu) {
	for (uint16_t i = 0; i < MEM8K; i++) {
		gb_mmu.rom[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		gb_mmu.wram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		gb_mmu.eram[i] = 0;
	}

	for (uint8_t i = 0; i < 0x7F; i++) {
		gb_mmu.zram[i] = 0;
	}

	for (uint16_t i = 0; i < 0x2000; i++) {
		gb_mmu.vram[i] = 0;
	}

	for (uint8_t i = 0; i < 0xA0; i++) {
		gb_mmu.oam[i] = 0;
	}
}

bool DBMMU_testRdWr(GB_MMU* mmu) {
	bool test = true;
	DBMMU_zeros(mmu);

	test = DBMMU_testWr(mmu);
	DBMMU_testRd(mmu);
	return test;
}



bool DBMMU_testWr(GB_MMU* mmu) {
	bool test_mem = true;  // Variable representing part of memory to be tested
	bool test_part = true; // Variable representing all the memory

	// No need to test write in ROM memory

	// WRAM
	for (uint16_t i = 0; i < 0x2000; i++) {
		wrByte(i + 0xC000, 2);
		if ((*mmu).wram[i] != 2) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testWr: WRAM false\n");
	}

	test_part = true;
	// ERAM
	for (uint16_t i = 0; i < 0x2000; i++) {
		wrByte(i + 0xA000, 3);
		if ((*mmu).eram[i] != 3) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testWr: ERAM false\n");
	}

	test_part = true;
	// ZRAM
	for (uint16_t i = 0; i < 0x7F; i++) {
		wrByte(i + 0xFF80, 4);
		if ((*mmu).zram[i] != 4) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testWr: ZRAM false\n");
	}

	test_part = true;
	// VRAM
	for (uint16_t i = 0; i < 0x2000; i++) {
		wrByte(i + 0x8000, 5);
		if ((*mmu).vram[i] != 5) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testWr: VRAM false\n");
	}

	test_part = true;
	// OAM
	for (uint16_t i = 0; i < 0xA0; i++) {
		wrByte(i + 0xFE00, 6);
		if ((*mmu).oam[i] != 6) {
			printf("i: %x\n", i);
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testWr: OAM false\n");
	}

	return test_mem;
}

bool DBMMU_testRd(GB_MMU* mmu) {
	bool test_mem = true;  // Variable representing part of memory to be tested
	bool test_part = true; // Variable representing all the memory

	// ROM
	for (uint16_t i = 0; i < 0x8000; i++) {
		(*mmu).rom[i] = 1;
	}

	for (uint16_t i = 0; i < 0x8000; i++) {
		if (rdByte(i) != 1) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testRd: ROM false\n");
	}

	test_part = true;
	// WRAM
	for (uint16_t i = 0; i < 0x2000; i++) {
		if (rdByte(i + 0xC000) != 2) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testRd: WRAM false\n");
	}

	test_part = true;
	// ERAM
	for (uint16_t i = 0; i < 0x2000; i++) {
		if (rdByte(i + 0xA000) != 3) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testRd: ERAM false\n");
	}

	test_part = true;
	// ZRAM
	for (uint16_t i = 0; i < 0x7F; i++) {
		if (rdByte(i + 0xFF80) != 4) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testRd: ZRAM false\n");
	}

	test_part = true;
	// VRAM
	for (uint16_t i = 0; i < 0x2000; i++) {
		if (rdByte(i + 0x8000) != 5) {
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testRd: VRAM false\n");
	}

	test_part = true;
	// OAM
	for (uint16_t i = 0; i < 0xA0; i++) {
		if (rdByte(i + 0xFE00) != 6) {
			printf("i: %x\n", i);
			test_mem = false;
			test_part = false;
		}
	}

	if (test_part == false) {
		printf("DBMMU_testRd: OAM false\n");
	}

	return test_mem;
}
