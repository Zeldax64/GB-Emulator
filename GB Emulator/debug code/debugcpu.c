#include "debugcpu.h"

/*----- Functions -----*/
void debugInstruction(uint16_t op);


struct InstructionSetDebug {
	char* inst;
};

const struct InstructionSetDebug instruction_debug[0x100];

const struct InstructionSetDebug instruction_debug[0x100] = {
	// 0x00
	{" NOP "},
	{ "LDBCnn" },
	{ "LDBCmA" },
	{ "INCBC" },
	{ "INCb" },
	{ "DECb" },
	{ "LDbn" },
	{ "RLCA" },
	{ "LDnnmSP" },
	{ "ADDHLBC" },
	{ "LDABCm" },
	{ "DECBC" },
	{ "INCc" },
	{ "DECc" },
	{ "LDcn" },
	{ "RRA" },

		// 0x10
	{ "STOP" },
	{ "LDDEnn" },
	{ "LDDEmA" },
	{ "INCDE" },

	{ "INCd" },
	{ "DECd" },
	{ "LDdn" },
	{ "RLA" },

	{ "JRe" },
	{ "ADDHLDE" },
	{ "LDADEm" },
	{ "DECDE" },

	{ "INCe" },
	{ "DECe" },
	{ "LDen" },
	{ "RRA" },

		// 0x20
	{ "JRNZe" },
	{ "LDHLnn" },
	{ "LDHLImA" },
	{ "INCHL" },

	{ "INCh" },
	{ "DECh" },
	{ "LDhn" },
	{ "DAA" },

	{ "JRZe" },
	{ "ADDHLHL" },
	{ "LDAHLIm" },
	{ "DECHL" },

	{ "INCl" },
	{ "DECl" },
	{ "LDln" },
	{ "CPL" },

		// 0x30
	{ "JRNCe" },
	{ "LDSPnn" },
	{ "LDHLDmA" },
	{ "INCSP" },

	{ "INCHLm" },
	{ "DECHLm" },
	{ "LDHLmn" },
	{ "SCF" },

	{ "JRCe" },
	{ "ADDHLSP" },
	{ "LDAHLDm" },
	{ "DECSP" },

	{ "INCa" },
	{ "DECa" },
	{ "LDan" },
	{ "CCF" },

		// 0x40
	{ "LDbb" },
	{ "LDbc" },
	{ "LDbd" },
	{ "LDbe" },

	{ "LDbh" },
	{ "LDbl" },
	{ "LDbHLm" },
	{ "LDba" },

	{ "LDcb" },
	{ "LDcc" },
	{ "LDcd" },
	{ "LDce" },

	{ "LDch" },
	{ "LDcl" },
	{ "LDcHLm" },
	{ "LDca" },

		// 0x50
	{ "LDdb" },
	{ "LDdc" },
	{ "LDdd" },
	{ "LDde" },

	{ "LDdh" },
	{ "LDdl" },
	{ "LDdHLm" },
	{ "LDda" },

	{ "LDeb" },
	{ "LDec" },
	{ "LDed" },
	{ "LDee" },

	{ "LDeh" },
	{ "LDel" },
	{ "LDeHLm" },
	{ "LDca" },

		// 0x60
	{ "LDhb" },
	{ "LDhc" },
	{ "LDhd" },
	{ "LDhe" },

	{ "LDhh" },
	{ "LDhl" },
	{ "LDhHLm" },
	{ "LDha" },

	{ "LDlb" },
	{ "LDlc" },
	{ "LDld" },
	{ "LDle" },

	{ "LDlh" },
	{ "LDll" },
	{ "LDlHLm" },
	{ "LDla" },

		// 0x70
	{ "LDHLmb" },
	{ "LDHLmc" },
	{ "LDHLmd" },
	{ "LDHLme" },

	{ "LDHLmh" },
	{ "LDHLml" },
	{ "HALT" },
	{ "LDHLma" },

	{ "LDab" },
	{ "LDac" },
	{ "LDad" },
	{ "LDae" },

	{ "LDah" },
	{ "LDal" },
	{ "LDaHLm" },
	{ "LDaa" },

		// 0x80
	{ "ADDAb" },
	{ "ADDAc" },
	{ "ADDAd" },
	{ "ADDAe" },

	{ "ADDAh" },
	{ "ADDAl" },
	{ "ADDAHLm" },
	{ "ADDAa" },

	{ "ADCAb" },
	{ "ADCAc" },
	{ "ADCAd" },
	{ "ADCAe" },

	{ "ADCAh" },
	{ "ADCAl" },
	{ "ADCAHLm" },
	{ "ADCAa" },

		// 0x90
	{ "SUBb" },
	{ "SUBc" },
	{ "SUBd" },
	{ "SUBe" },

	{ "SUBh" },
	{ "SUBl" },
	{ "SUBHLm" },
	{ "SUBa" },

	{ "SBCAb" },
	{ "SBCAc" },
	{ "SBCAd" },
	{ "SBCAe" },

	{ "SBCAh" },
	{ "SBCAl" },
	{ "SBCAHLm" },
	{ "SBCAa" },


		// 0xA0
	{ "ANDb" },
	{ "ANDc" },
	{ "ANDd" },
	{ "ANDe" },

	{ "ANDh" },
	{ "ANDl" },
	{ "ANDHLm" },
	{ "ANDa" },

	{ "XORb" },
	{ "XORc" },
	{ "XORd" },
	{ "XORe" },

	{ "XORh" },
	{ "XORl" },
	{ "XORHLm" },
	{ "XORa" },


		// 0xB0
	{ "ORb" },
	{ "ORc" },
	{ "ORd" },
	{ "ORe" },

	{ "ORh" },
	{ "ORl" },
	{ "ORHLm" },
	{ "ORa" },

	{ "CPb" },
	{ "CPc" },
	{ "CPd" },
	{ "CPe" },

	{ "CPh" },
	{ "CPl" },
	{ "CPHLm" },
	{ "CPa" },

		// 0xC0
	{ "RETNZ" },
	{ "POPBC" },
	{ "JPNZnn" },
	{ "JPnn" },

	{ "CALLNZnn" },
	{ "PUSHBC" },
	{ "ADDAn" },
	{ "RST00" },

	{ "RETZ" },
	{ "RET" },
	{ "JPZnn" },
	{ "CB" },

	{ "CALLZnn" },
	{ "CALLnn" },
	{ "ADCAn" },
	{ "RST08" },

		// 0xD0
	{ "RETNC" },
	{ "POPDE" },
	{ "JPNCnn" },
	{ "CPU_NULL" },

	{ "CALLNCnn" },
	{ "PUSHDE" },
	{ "SUBn" },
	{ "RST10" },

	{ "RETC" },
	{ "RETI" },
	{ "JPCnn" },
	{ "CPU_NULL" },

	{ "CALLCnn" },
	{ "CPU_NULL" },
	{ "SBCAn" },
	{ "RST18" },

		// 0xE0
	{ "LDnmA" },
	{ "POPHL" },
	{ "LDCmA" },
	{ "CPU_NULL" },

	{ "CPU_NULL" },
	{ "PUSHHL" },
	{ "ANDn" },
	{ "RST20" },

	{ "ADDSPe" },
	{ "JPHLm" },
	{ "LDnnmA" },
	{ "CPU_NULL" },

	{ "CPU_NULL" },
	{ "CPU_NULL" },
	{ "XORn" },
	{ "RST28" },

		// 0xF0
	{ "LDAnm" },
	{ "POPAF" },
	{ "LDACm" },
	{ "DI" },

	{ "CPU_NULL" },
	{ "PUSHAF" },
	{ "ORn" },
	{ "RST30" },

	{ "LDHLSPe" },
	{ "LDSPHL" },
	{ "LDAnnm" },
	{ "EI" },

	{ "CPU_NULL" },
	{ "CPU_NULL" },
	{ "CPn" },
	{ "RST38" }
};

void printCPUReg(void) {
	printf("--- GB CPU Registers ---\n");
	printf("A: %2x    F: %2x\n", gb_cpu.a, gb_cpu.f);
	printf("B: %2x    C: %2x\n", gb_cpu.b, gb_cpu.c);
	printf("D: %2x    E: %2x\n", gb_cpu.d, gb_cpu.e);
	printf("H: %2x    L: %2x\n", gb_cpu.h, gb_cpu.l);
	printf("PC: %4x    SP: %4x\n", gb_cpu.pc, gb_cpu.sp);
	printf("IME: %d    INT ENA: %x\n", gb_cpu.ime, rdByte(0xFFFF));
}

void printDbgReg(void) {
	printf("--- Cinoop CPU Registers ---\n");
	printf("A: %2x    F: %2x\n", registers.a, registers.f);
	printf("B: %2x    C: %2x\n", registers.b, registers.c);
	printf("D: %2x    E: %2x\n", registers.d, registers.e);
	printf("H: %2x    L: %2x\n", registers.h, registers.l);
	printf("PC: %4x    SP: %4x\n", registers.pc, registers.sp);
}

void printMem(uint16_t pc) {
	pc -= 2;
	printf("--- Mem Map ---\n");
	for (int i = 0; i < 5; i++, pc++) {
		if(i == 2)
			printf("-> [%x] = %x, %s <-\n", pc, rdWord(pc), instruction_debug[rdByte(pc)]);
		else
			printf("[%x] = %x, %s\n", pc, rdWord(pc), instruction_debug[rdByte(pc)]);
	}
}

void printLCDReg() {
	printf("--- LCD Registers ---\n");
	printf("LCDC: %2x    STAT: %2x\n", gb_lcd.lcdc, gb_lcd.stat);
	printf("WX: %2x    WY: %2x\n", gb_lcd.wx, gb_lcd.wy);
	printf("ly: %2x    lyc: %2x\n", gb_lcd.ly, gb_lcd.lyc);
	printf("\n");
}

void genReport(bool both_cpu, bool complete) {
	//printMem(gb_cpu.pc);
	printCPUReg();
	printf("-> [%x] = %x, %s <-\n", gb_cpu.pc, rdWord(gb_cpu.pc), instruction_debug[rdByte(gb_cpu.pc)]);
	if (both_cpu)
		printDbgReg();
	if (complete) {
		printLCDReg();
	}
}

bool cmpReg() {
	bool cmp = false;

	if (gb_cpu.af != registers.af) {
		printf("AF->GB: %4x    Cin: %4x\n", gb_cpu.af, registers.af);
		cmp = true;
	}
	if (gb_cpu.bc != registers.bc) {
		printf("BC->GB: %4x    Cin: %4x\n", gb_cpu.bc, registers.bc);
		cmp = true;
	}
	if (gb_cpu.de != registers.de) {
		printf("DE->GB: %4x    Cin: %4x\n", gb_cpu.de, registers.de);
		cmp = true;
	}
	if (gb_cpu.hl != registers.hl) {
		printf("HL->GB: %4x    Cin: %4x\n", gb_cpu.hl, registers.hl);
		cmp = true;
	}
	if (gb_cpu.pc != registers.pc) {
		printf("PC->GB: %4x    Cin: %4x\n", gb_cpu.pc, registers.pc);
		cmp = true;
	}

	if (cmp)
		return false;
	return true;
}

bool cmpMem() {
	bool cmp = false;

	// Compare WRAM
	for (int i = 0; i < 0x2000; i++) {
		if (gb_mmu.wram[i] != dbgb_mmu.wram[i]) {
			cmp = true;
			printf("Erro wram[%d]: GB_MMU %x    CIN_MMU %x", i, gb_mmu.wram[i], dbgb_mmu.wram[i]);
		}
	}
	for (int i = 0; i < 0x2000; i++) {
		if (gb_mmu.eram[i] != dbgb_mmu.eram[i]) {
			cmp = true;
			printf("Erro eram[%d]: GB_MMU %x    CIN_MMU %x", i, gb_mmu.eram[i], dbgb_mmu.eram[i]);
		}
	}
	for (int i = 0; i < 0x7F; i++) {
		if (gb_mmu.zram[i] != dbgb_mmu.zram[i]) {
			cmp = true;
			printf("Erro zram[%d]: GB_MMU %x    CIN_MMU %x", i, gb_mmu.zram[i], dbgb_mmu.zram[i]);
		}
	}
	for (int i = 0; i < 0x2000; i++) {
		if (gb_mmu.vram[i] != dbgb_mmu.vram[i]) {
			cmp = true;
			printf("Erro vram[%d]: GB_MMU %x    CIN_MMU %x", i, gb_mmu.vram[i], dbgb_mmu.vram[i]);
		}
	}
	for (int i = 0; i < 0xA0; i++) {
		if (gb_mmu.oam[i] != dbgb_mmu.oam[i]) {
			cmp = true;
			printf("Erro wram[%d]: GB_MMU %x    CIN_MMU %x", i, gb_mmu.wram[i], dbgb_mmu.wram[i]);
		}
	}
	if (cmp)
		return false;
	return true;
}

void gbCompare() {
	if (!cmpReg()) {
		printf("!!!Erro nos registros!!!\n");
		genReport(true, false);

		gb_cpu.pc -= 2;
		registers.pc -= 2;
		printf("GB [%4x] %4x   DB [%4x] %4x\n", gb_cpu.pc, rdByte(gb_cpu.pc), registers.pc, readByte(registers.pc));
		
		gb_cpu.pc++;
		registers.pc++;
		printf("GB [%4x] %4x   DB [%4x] %4x\n", gb_cpu.pc, rdByte(gb_cpu.pc), registers.pc, readByte(registers.pc));

		printf("GB [%4x] %4x   DB [%4x] %4x\n", rdByte(gb_cpu.pc) + 0xFF00, rdByte(gb_cpu.pc + 0xFF00), readByte(registers.pc) + 0xFF00 , readByte(registers.pc + 0xFF00));

		getchar();
	}
	if (!cmpMem()) {
		printf("!!!Erro na memoria!!!\n");
		genReport(true, false);
		getchar();
	}

}

void DBCPU_cycle(void) {

	genReport(true, false);
	CPU_cycle();
	cpuStep(); // Cinoop's CPU step
	//getchar();
	//system("cls");
	gbCompare();
	printf("#############################\n");
}