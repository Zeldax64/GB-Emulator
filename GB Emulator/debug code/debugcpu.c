#include "debugcpu.h"

/*----- Functions -----*/
void debugInstruction(uint16_t op);


struct InstructionSetDebug {
	char* inst;
};

struct CBInstructionSetDebug {
	char* inst;
};

const struct InstructionSetDebug instruction_debug[0x100];
const struct CBInstructionSetDebug cb_instruction_debug[0x100];


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
	if (rdByte(gb_cpu.pc) == 0xCB)
		printf("CB Instruction: %s\n", cb_instruction_debug[rdByte(gb_cpu.pc + 1)]);

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
			printf("Erro wram[%d]: GB_MMU %x    CIN_MMU %x\n", i, gb_mmu.wram[i], dbgb_mmu.wram[i]);
		}
	}
	for (int i = 0; i < 0x2000; i++) {
		if (gb_mmu.eram[i] != dbgb_mmu.eram[i]) {
			cmp = true;
			printf("Erro eram[%d]: GB_MMU %x    CIN_MMU %x\n", i, gb_mmu.eram[i], dbgb_mmu.eram[i]);
		}
	}
	for (int i = 0; i < 0x7F; i++) {
		if (gb_mmu.zram[i] != dbgb_mmu.zram[i]) {
			cmp = true;
			printf("Erro zram[%d]: GB_MMU %x    CIN_MMU %x\n", i, gb_mmu.zram[i], dbgb_mmu.zram[i]);
		}
	}
	for (int i = 0; i < 0x2000; i++) {
		if (gb_mmu.vram[i] != dbgb_mmu.vram[i]) {
			cmp = true;
			printf("Erro vram[%d]: GB_MMU %x    CIN_MMU %x\n", i, gb_mmu.vram[i], dbgb_mmu.vram[i]);
		}
	}
	for (int i = 0; i < 0xA0; i++) {
		if (gb_mmu.oam[i] != dbgb_mmu.oam[i]) {
			cmp = true;
			printf("Erro wram[%d]: GB_MMU %x    CIN_MMU %x\n", i, gb_mmu.wram[i], dbgb_mmu.wram[i]);
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
		getchar();
	}
	if (!cmpMem()) {
		printf("!!!Erro na memoria!!!\n");
		genReport(true, false);
		getchar();
	}

}

int16_t maxpc = -1;
void DBCPU_cycle(void) {

	//genReport(true, false);
	CPU_cycle();
	cpuStep(); // Cinoop's CPU step
	//getchar();
	//system("cls");
	gbCompare();
	//printf("#############################\n");
	if (gb_cpu.pc > maxpc) {
		maxpc = gb_cpu.pc;
		printf("Max PC: %x\n", gb_cpu.pc);
	}
}

const struct InstructionSetDebug instruction_debug[0x100] = {
	// 0x00
	{ " NOP " },
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

const struct CBInstructionSetDebug cb_instruction_debug[0x100] = {
	{ "RLC B" },           // 0x00
	{ "RLC C" },           // 0x01
	{ "RLC D" },           // 0x02
	{ "RLC E" },           // 0x03
	{ "RLC H" },           // 0x04
	{ "RLC L" },           // 0x05
	{ "RLC (HL)" },      // 0x06
	{ "RLC A" },           // 0x07
	{ "RRC B" },           // 0x08
	{ "RRC C" },           // 0x09
	{ "RRC D" },           // 0x0a
	{ "RRC E" },           // 0x0b
	{ "RRC H" },           // 0x0c
	{ "RRC L" },           // 0x0d
	{ "RRC (HL)" },      // 0x0e
	{ "RRC A" },           // 0x0f
	{ "RL B" },             // 0x10
	{ "RL C" },             // 0x11
	{ "RL D" },             // 0x12
	{ "RL E" },             // 0x13
	{ "RL H" },             // 0x14
	{ "RL L" },             // 0x15
	{ "RL (HL)" },        // 0x16
	{ "RL A" },             // 0x17
	{ "RR B" },             // 0x18
	{ "RR C" },             // 0x19
	{ "RR D" },             // 0x1a
	{ "RR E" },             // 0x1b
	{ "RR H" },             // 0x1c
	{ "RR L" },             // 0x1d
	{ "RR (HL)" },        // 0x1e
	{ "RR A" },             // 0x1f
	{ "SLA B" },           // 0x20
	{ "SLA C" },           // 0x21
	{ "SLA D" },           // 0x22
	{ "SLA E" },           // 0x23
	{ "SLA H" },           // 0x24
	{ "SLA L" },           // 0x25
	{ "SLA (HL)" },      // 0x26
	{ "SLA A" },           // 0x27
	{ "SRA B" },           // 0x28
	{ "SRA C" },           // 0x29
	{ "SRA D" },           // 0x2a
	{ "SRA E" },           // 0x2b
	{ "SRA H" },           // 0x2c
	{ "SRA L" },           // 0x2d
	{ "SRA (HL)" },      // 0x2e
	{ "SRA A" },           // 0x2f
	{ "SWAP B" },         // 0x30
	{ "SWAP C" },         // 0x31
	{ "SWAP D" },         // 0x32
	{ "SWAP E" },         // 0x33
	{ "SWAP H" },         // 0x34
	{ "SWAP L" },         // 0x35
	{ "SWAP (HL)" },    // 0x36
	{ "SWAP A" },         // 0x37
	{ "SRL B" },           // 0x38
	{ "SRL C" },           // 0x39
	{ "SRL D" },           // 0x3a
	{ "SRL E" },           // 0x3b
	{ "SRL H" },           // 0x3c
	{ "SRL L" },           // 0x3d
	{ "SRL (HL)" },      // 0x3e
	{ "SRL A" },           // 0x3f
	{ "BIT 0, B" },      // 0x40
	{ "BIT 0, C" },      // 0x41
	{ "BIT 0, D" },      // 0x42
	{ "BIT 0, E" },      // 0x43
	{ "BIT 0, H" },      // 0x44
	{ "BIT 0, L" },      // 0x45
	{ "BIT 0, (HL)" }, // 0x46
	{ "BIT 0, A" },      // 0x47
	{ "BIT 1, B" },      // 0x48
	{ "BIT 1, C" },      // 0x49
	{ "BIT 1, D" },      // 0x4a
	{ "BIT 1, E" },      // 0x4b
	{ "BIT 1, H" },      // 0x4c
	{ "BIT 1, L" },      // 0x4d
	{ "BIT 1, (HL)" }, // 0x4e
	{ "BIT 1, A" },      // 0x4f
	{ "BIT 2, B" },      // 0x50
	{ "BIT 2, C" },      // 0x51
	{ "BIT 2, D" },      // 0x52
	{ "BIT 2, E" },      // 0x53
	{ "BIT 2, H" },      // 0x54
	{ "BIT 2, L" },      // 0x55
	{ "BIT 2, (HL)" }, // 0x56
	{ "BIT 2, A" },      // 0x57
	{ "BIT 3, B" },      // 0x58
	{ "BIT 3, C" },      // 0x59
	{ "BIT 3, D" },      // 0x5a
	{ "BIT 3, E" },      // 0x5b
	{ "BIT 3, H" },      // 0x5c
	{ "BIT 3, L" },      // 0x5d
	{ "BIT 3, (HL)" }, // 0x5e
	{ "BIT 3, A" },      // 0x5f
	{ "BIT 4, B" },      // 0x60
	{ "BIT 4, C" },      // 0x61
	{ "BIT 4, D" },      // 0x62
	{ "BIT 4, E" },      // 0x63
	{ "BIT 4, H" },      // 0x64
	{ "BIT 4, L" },      // 0x65
	{ "BIT 4, (HL)" }, // 0x66
	{ "BIT 4, A" },      // 0x67
	{ "BIT 5, B" },      // 0x68
	{ "BIT 5, C" },      // 0x69
	{ "BIT 5, D" },      // 0x6a
	{ "BIT 5, E" },      // 0x6b
	{ "BIT 6, H" },      // 0x6c
	{ "BIT 6, L" },      // 0x6d
	{ "BIT 5, (HL)" }, // 0x6e
	{ "BIT 5, A" },      // 0x6f
	{ "BIT 6, B" },      // 0x70
	{ "BIT 6, C" },      // 0x71
	{ "BIT 6, D" },      // 0x72
	{ "BIT 6, E" },      // 0x73
	{ "BIT 6, H" },      // 0x74
	{ "BIT 6, L" },      // 0x75
	{ "BIT 6, (HL)" }, // 0x76
	{ "BIT 6, A" },      // 0x77
	{ "BIT 7, B" },      // 0x78
	{ "BIT 7, C" },      // 0x79
	{ "BIT 7, D" },      // 0x7a
	{ "BIT 7, E" },      // 0x7b
	{ "BIT 7, H" },      // 0x7c
	{ "BIT 7, L" },      // 0x7d
	{ "BIT 7, (HL)" }, // 0x7e
	{ "BIT 7, A" },      // 0x7f
	{ "RES 0, B" },      // 0x80
	{ "RES 0, C" },      // 0x81
	{ "RES 0, D" },      // 0x82
	{ "RES 0, E" },      // 0x83
	{ "RES 0, H" },      // 0x84
	{ "RES 0, L" },      // 0x85
	{ "RES 0, (HL)" }, // 0x86
	{ "RES 0, A" },      // 0x87
	{ "RES 1, B" },      // 0x88
	{ "RES 1, C" },      // 0x89
	{ "RES 1, D" },      // 0x8a
	{ "RES 1, E" },      // 0x8b
	{ "RES 1, H" },      // 0x8c
	{ "RES 1, L" },      // 0x8d
	{ "RES 1, (HL)" }, // 0x8e
	{ "RES 1, A" },      // 0x8f
	{ "RES 2, B" },      // 0x90
	{ "RES 2, C" },      // 0x91
	{ "RES 2, D" },      // 0x92
	{ "RES 2, E" },      // 0x93
	{ "RES 2, H" },      // 0x94
	{ "RES 2, L" },      // 0x95
	{ "RES 2, (HL)" }, // 0x96
	{ "RES 2, A" },      // 0x97
	{ "RES 3, B" },      // 0x98
	{ "RES 3, C" },      // 0x99
	{ "RES 3, D" },      // 0x9a
	{ "RES 3, E" },      // 0x9b
	{ "RES 3, H" },      // 0x9c
	{ "RES 3, L" },      // 0x9d
	{ "RES 3, (HL)" }, // 0x9e
	{ "RES 3, A" },      // 0x9f
	{ "RES 4, B" },      // 0xa0
	{ "RES 4, C" },      // 0xa1
	{ "RES 4, D" },      // 0xa2
	{ "RES 4, E" },      // 0xa3
	{ "RES 4, H" },      // 0xa4
	{ "RES 4, L" },      // 0xa5
	{ "RES 4, (HL)" }, // 0xa6
	{ "RES 4, A" },      // 0xa7
	{ "RES 5, B" },      // 0xa8
	{ "RES 5, C" },      // 0xa9
	{ "RES 5, D" },      // 0xaa
	{ "RES 5, E" },      // 0xab
	{ "RES 5, H" },      // 0xac
	{ "RES 5, L" },      // 0xad
	{ "RES 5, (HL)" }, // 0xae
	{ "RES 5, A" },      // 0xaf
	{ "RES 6, B" },      // 0xb0
	{ "RES 6, C" },      // 0xb1
	{ "RES 6, D" },      // 0xb2
	{ "RES 6, E" },      // 0xb3
	{ "RES 6, H" },      // 0xb4
	{ "RES 6, L" },      // 0xb5
	{ "RES 6, (HL)" }, // 0xb6
	{ "RES 6, A" },      // 0xb7
	{ "RES 7, B" },      // 0xb8
	{ "RES 7, C" },      // 0xb9
	{ "RES 7, D" },      // 0xba
	{ "RES 7, E" },      // 0xbb
	{ "RES 7, H" },      // 0xbc
	{ "RES 7, L" },      // 0xbd
	{ "RES 7, (HL)" }, // 0xbe
	{ "RES 7, A" },      // 0xbf
	{ "SET 0, B" },      // 0xc0
	{ "SET 0, C" },      // 0xc1
	{ "SET 0, D" },      // 0xc2
	{ "SET 0, E" },      // 0xc3
	{ "SET 0, H" },      // 0xc4
	{ "SET 0, L" },      // 0xc5
	{ "SET 0, (HL)" }, // 0xc6
	{ "SET 0, A" },      // 0xc7
	{ "SET 1, B" },      // 0xc8
	{ "SET 1, C" },      // 0xc9
	{ "SET 1, D" },      // 0xca
	{ "SET 1, E" },      // 0xcb
	{ "SET 1, H" },      // 0xcc
	{ "SET 1, L" },      // 0xcd
	{ "SET 1, (HL)" }, // 0xce
	{ "SET 1, A" },      // 0xcf
	{ "SET 2, B" },      // 0xd0
	{ "SET 2, C" },      // 0xd1
	{ "SET 2, D" },      // 0xd2
	{ "SET 2, E" },      // 0xd3
	{ "SET 2, H" },      // 0xd4
	{ "SET 2, L" },      // 0xd5
	{ "SET 2, (HL)" }, // 0xd6
	{ "SET 2, A" },      // 0xd7
	{ "SET 3, B" },      // 0xd8
	{ "SET 3, C" },      // 0xd9
	{ "SET 3, D" },      // 0xda
	{ "SET 3, E" },      // 0xdb
	{ "SET 3, H" },      // 0xdc
	{ "SET 3, L" },      // 0xdd
	{ "SET 3, (HL)" }, // 0xde
	{ "SET 3, A" },      // 0xdf
	{ "SET 4, B" },      // 0xe0
	{ "SET 4, C" },      // 0xe1
	{ "SET 4, D" },      // 0xe2
	{ "SET 4, E" },      // 0xe3
	{ "SET 4, H" },      // 0xe4
	{ "SET 4, L" },      // 0xe5
	{ "SET 4, (HL)" }, // 0xe6
	{ "SET 4, A" },      // 0xe7
	{ "SET 5, B" },      // 0xe8
	{ "SET 5, C" },      // 0xe9
	{ "SET 5, D" },      // 0xea
	{ "SET 5, E" },      // 0xeb
	{ "SET 5, H" },      // 0xec
	{ "SET 5, L" },      // 0xed
	{ "SET 5, (HL)" }, // 0xee
	{ "SET 5, A" },      // 0xef
	{ "SET 6, B" },      // 0xf0
	{ "SET 6, C" },      // 0xf1
	{ "SET 6, D" },      // 0xf2
	{ "SET 6, E" },      // 0xf3
	{ "SET 6, H" },      // 0xf4
	{ "SET 6, L" },      // 0xf5
	{ "SET 6, (HL)" }, // 0xf6
	{ "SET 6, A" },      // 0xf7
	{ "SET 7, B" },      // 0xf8
	{ "SET 7, C" },      // 0xf9
	{ "SET 7, D" },      // 0xfa
	{ "SET 7, E" },      // 0xfb
	{ "SET 7, H" },      // 0xfc
	{ "SET 7, L" },      // 0xfd
	{ "SET 7, (HL)" }, // 0xfe
	{ "SET 7, A" },      // 0xff
};