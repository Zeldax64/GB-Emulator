#include <stdio.h>

#include "cpu.h"


const void(*instructions[0x100])();
const void(*CBinstructions[0x100])();

/*----- CPU Init function -----*/
// Verify GB's cpu initialization
void CPU_init(void) {
	gb_cpu.pc = 0x100;
	gb_cpu.af = 0x01B0;
	gb_cpu.bc = 0x0013;
	gb_cpu.de = 0x00D8;
	gb_cpu.hl = 0x014D;
	gb_cpu.sp = 0xFFFE;
}

void CPU_cycle(void) {
	uint8_t op = rdByte(gb_cpu.pc);
	printf("OP: %x\n", op);
	(*instructions[op])();
}

/*----- Flag calculator functions -----*/
void flagC8(uint8_t val1, uint8_t val2) { // Flag C calculator for 8-Bit register
	if ((uint16_t)val1 + val2 & 0x100)
		gb_cpu.f |= FLAGC;
}

void flagC16(uint16_t val1, uint16_t val2) { // Flag C calculator for 16-Bit register
	if ((uint32_t)val1 + val2 & 0x10000)
		gb_cpu.f |= FLAGC;
}

void flagC8Sub(uint8_t val1, uint8_t val2) { // Flag C calculator for 8-Bit register
	if ((int16_t)val1 - val2 < 0)
		gb_cpu.f |= FLAGC;
}

// NOTA
// DIFFERENT
void flagH8(uint8_t val1, uint8_t val2) { // Flag H calculator for 8-Bit register
	if (((val1 &= 0xF) + (val2 &= 0xF)) & 0x10)
		gb_cpu.f |= FLAGH;
}

void flagH16(uint16_t val1, uint16_t val2) { // Flag H calculator for 16-Bit register
	if (((val1 &= 0xFFF) + (val2 &= 0xFFF)) & 0x1000)
		gb_cpu.f |= FLAGH;
}

void flagH8Sub(uint8_t val1, uint8_t val2) { // Flag H calculator for 8-Bit register
	if (((val1 &= 0xF) - (val2 &= 0xF)) < 0)
		gb_cpu.f |= FLAGH;
}

void flagZ8(uint8_t val) { // Flag Z calculator for 8-Bit register
	if (val == 0)
		gb_cpu.f |= FLAGZ;
}

void flagZ16(uint16_t val) {
	if (val == 0)
		gb_cpu.f |= FLAGZ;
}

/*----- Instructions -----*/

/*----- Load Instruction -----*/
/* 8-Bit */

// LDrr instructions expansion to all possibilities of r.
void LDrr(uint8_t *reg1, uint8_t *reg2) { *(reg1) = *(reg2); gb_cpu.pc++; gb_cpu.m++; }  // LD r, r'

// LDAr
void LDaa() { LDrr(&gb_cpu.a, &gb_cpu.a); }
void LDab() { LDrr(&gb_cpu.a, &gb_cpu.b); }
void LDac() { LDrr(&gb_cpu.a, &gb_cpu.c); }
void LDad() { LDrr(&gb_cpu.a, &gb_cpu.d); }
void LDae() { LDrr(&gb_cpu.a, &gb_cpu.e); }
void LDah() { LDrr(&gb_cpu.a, &gb_cpu.h); }
void LDal() { LDrr(&gb_cpu.a, &gb_cpu.l); }

// LDBr
void LDba() { LDrr(&gb_cpu.b, &gb_cpu.a); }
void LDbb() { LDrr(&gb_cpu.b, &gb_cpu.b); }
void LDbc() { LDrr(&gb_cpu.b, &gb_cpu.c); }
void LDbd() { LDrr(&gb_cpu.b, &gb_cpu.d); }
void LDbe() { LDrr(&gb_cpu.b, &gb_cpu.e); }
void LDbh() { LDrr(&gb_cpu.b, &gb_cpu.h); }
void LDbl() { LDrr(&gb_cpu.b, &gb_cpu.l); }

// LDCr
void LDca() { LDrr(&gb_cpu.c, &gb_cpu.a); }
void LDcb() { LDrr(&gb_cpu.c, &gb_cpu.b); }
void LDcc() { LDrr(&gb_cpu.c, &gb_cpu.c); }
void LDcd() { LDrr(&gb_cpu.c, &gb_cpu.d); }
void LDce() { LDrr(&gb_cpu.c, &gb_cpu.e); }
void LDch() { LDrr(&gb_cpu.c, &gb_cpu.h); }
void LDcl() { LDrr(&gb_cpu.c, &gb_cpu.l); }

// LDDr
void LDda() { LDrr(&gb_cpu.d, &gb_cpu.a); }
void LDdb() { LDrr(&gb_cpu.d, &gb_cpu.b); }
void LDdc() { LDrr(&gb_cpu.d, &gb_cpu.c); }
void LDdd() { LDrr(&gb_cpu.d, &gb_cpu.d); }
void LDde() { LDrr(&gb_cpu.d, &gb_cpu.e); }
void LDdh() { LDrr(&gb_cpu.d, &gb_cpu.h); }
void LDdl() { LDrr(&gb_cpu.d, &gb_cpu.l); }

// LDEr
void LDea() { LDrr(&gb_cpu.e, &gb_cpu.a); }
void LDeb() { LDrr(&gb_cpu.e, &gb_cpu.b); }
void LDec() { LDrr(&gb_cpu.e, &gb_cpu.c); }
void LDed() { LDrr(&gb_cpu.e, &gb_cpu.d); }
void LDee() { LDrr(&gb_cpu.e, &gb_cpu.e); }
void LDeh() { LDrr(&gb_cpu.e, &gb_cpu.h); }
void LDel() { LDrr(&gb_cpu.e, &gb_cpu.l); }

// LDHr
void LDha() { LDrr(&gb_cpu.h, &gb_cpu.a); }
void LDhb() { LDrr(&gb_cpu.h, &gb_cpu.b); }
void LDhc() { LDrr(&gb_cpu.h, &gb_cpu.c); }
void LDhd() { LDrr(&gb_cpu.h, &gb_cpu.d); }
void LDhe() { LDrr(&gb_cpu.h, &gb_cpu.e); }
void LDhh() { LDrr(&gb_cpu.h, &gb_cpu.h); }
void LDhl() { LDrr(&gb_cpu.h, &gb_cpu.l); }

// LDLr
void LDla() { LDrr(&gb_cpu.l, &gb_cpu.a); }
void LDlb() { LDrr(&gb_cpu.l, &gb_cpu.b); }
void LDlc() { LDrr(&gb_cpu.l, &gb_cpu.c); }
void LDld() { LDrr(&gb_cpu.l, &gb_cpu.d); }
void LDle() { LDrr(&gb_cpu.l, &gb_cpu.e); }
void LDlh() { LDrr(&gb_cpu.l, &gb_cpu.h); }
void LDll() { LDrr(&gb_cpu.l, &gb_cpu.l); }

// End

// LDrn instructions expansion to all possibilities of r.
void LDrn(uint8_t *reg) { *(reg) = rdByte(++gb_cpu.pc); gb_cpu.pc++; gb_cpu.m += 2; }	   // LD r,n

void LDan(uint8_t * reg){ LDrn(&gb_cpu.a); }
void LDbn(uint8_t * reg){ LDrn(&gb_cpu.b); }
void LDcn(uint8_t * reg){ LDrn(&gb_cpu.c); }
void LDdn(uint8_t * reg){ LDrn(&gb_cpu.d); }
void LDen(uint8_t * reg){ LDrn(&gb_cpu.e); }
void LDhn(uint8_t * reg){ LDrn(&gb_cpu.h); }
void LDln(uint8_t * reg){ LDrn(&gb_cpu.l); }

// End

// LDr(HL) instructions expansion to all possibilities of r.
void LDrHLm(uint8_t *reg) { *(reg) = rdByte(gb_cpu.hl); gb_cpu.pc++; gb_cpu.m += 2; }		   // LD r,(HL)

void LDaHLm(uint8_t *reg) { LDrHLm(&gb_cpu.a); }
void LDbHLm(uint8_t *reg) { LDrHLm(&gb_cpu.b); }
void LDcHLm(uint8_t *reg) { LDrHLm(&gb_cpu.c); }
void LDdHLm(uint8_t *reg) { LDrHLm(&gb_cpu.d); }
void LDeHLm(uint8_t *reg) { LDrHLm(&gb_cpu.e); }
void LDhHLm(uint8_t *reg) { LDrHLm(&gb_cpu.h); }
void LDlHLm(uint8_t *reg) { LDrHLm(&gb_cpu.l); }

// End

// LD(HL)r instructions expansion to all possibilities of r.
void LDHLmr(uint8_t reg) { wrByte(gb_cpu.hl, reg); gb_cpu.pc++; gb_cpu.m += 2; } // LD (HL), r

void LDHLma(uint8_t reg) { LDHLmr(gb_cpu.a); }
void LDHLmb(uint8_t reg) { LDHLmr(gb_cpu.b); }
void LDHLmc(uint8_t reg) { LDHLmr(gb_cpu.c); }
void LDHLmd(uint8_t reg) { LDHLmr(gb_cpu.d); }
void LDHLme(uint8_t reg) { LDHLmr(gb_cpu.e); }
void LDHLmh(uint8_t reg) { LDHLmr(gb_cpu.h); }
void LDHLml(uint8_t reg) { LDHLmr(gb_cpu.l); }

// End

void LDHLmn() { wrByte(gb_cpu.hl, rdByte(++gb_cpu.pc)); gb_cpu.pc++; gb_cpu.m += 3; }		   // LD (HL), n
void LDABCm() { gb_cpu.a = rdByte(gb_cpu.bc); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD A, (BC)
void LDADEm() { gb_cpu.a = rdByte(gb_cpu.de); gb_cpu.pc++;	gb_cpu.m += 2; }				   // LD A, (DE)
void LDACm() { gb_cpu.a = rdByte(gb_cpu.c + 0xFF00); gb_cpu.pc++; gb_cpu.m += 2; }			   // LD A, (C)
void LDCmA() { wrByte(gb_cpu.c + 0xFF00, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 2; }			   // LD (C), A
void LDAnm() { gb_cpu.a = rdByte(++gb_cpu.pc); gb_cpu.pc++; gb_cpu.m += 3; }					   // LD A, (n)
void LDnmA() { wrByte(++gb_cpu.pc, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 3; }					   // LD (n), A
void LDAnnm() { gb_cpu.a = rdByte(rdWord(++gb_cpu.pc)); gb_cpu.pc += 2; gb_cpu.m += 4; }		   // LD A, (nn)
void LDnnmA() { wrByte(rdWord(++gb_cpu.pc), gb_cpu.a); gb_cpu.pc += 2; gb_cpu.m += 4; }		   // LD (nn), A
void LDAHLIm() { gb_cpu.a = rdByte(gb_cpu.hl); gb_cpu.hl++; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD A, (HLI)
void LDAHLDm() { gb_cpu.a = rdByte(gb_cpu.hl); gb_cpu.hl--; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD A, (HLD)
void LDBCmA() { wrByte(gb_cpu.bc, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD (BC), A
void LDDEmA() { wrByte(gb_cpu.de, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD (DE), A
void LDHLImA() { wrByte(gb_cpu.hl, gb_cpu.a); gb_cpu.hl++; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD (HLI), A
void LDHLDmA() { wrByte(gb_cpu.hl, gb_cpu.a); gb_cpu.hl--; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD (HLD), A

/* 16-Bit */
// LDddnn instructions expansion to all possibilities of dd.
void LDddnn(uint16_t *reg) { *(reg) = rdWord(gb_cpu.pc++); gb_cpu.pc += 2; gb_cpu.m += 3;}	  // LD dd, nn

void LDBCnn(uint16_t *reg) { LDddnn(&gb_cpu.bc); }
void LDDEnn(uint16_t *reg) { LDddnn(&gb_cpu.de); }
void LDHLnn(uint16_t *reg) { LDddnn(&gb_cpu.hl); }
void LDSPnn(uint16_t *reg) { LDddnn(&gb_cpu.sp); }

// End

void LDSPHL() { gb_cpu.sp = gb_cpu.hl; gb_cpu.pc++; gb_cpu.m += 2; }							  // LD SP, HL

// PUSHqq instructions expansion to all possibilities of qq.
void PUSHqq(uint16_t reg) { gb_cpu.sp -= 2; wrWord(gb_cpu.sp, reg); gb_cpu.pc++; gb_cpu.m += 4; } // PUSH qq

void PUSHBC(uint16_t reg) { PUSHqq(gb_cpu.bc); }
void PUSHDE(uint16_t reg) { PUSHqq(gb_cpu.de); }
void PUSHHL(uint16_t reg) { PUSHqq(gb_cpu.hl); }
void PUSHAF(uint16_t reg) { PUSHqq(gb_cpu.af); }

// End

// POPqq instructions expansion to all possibilities of qq.
// NOTA
// Different : rdWord is used instead of rdByte.
void POPqq(uint16_t *reg) { *reg = rdWord(gb_cpu.sp); gb_cpu.sp += 2; gb_cpu.pc++; gb_cpu.m += 3; } // POP qq

void POPBC(uint16_t *reg) { POPqq(&gb_cpu.bc); }
void POPDE(uint16_t *reg) { POPqq(&gb_cpu.de); }
void POPHL(uint16_t *reg) { POPqq(&gb_cpu.hl); }
void POPAF(uint16_t *reg) { POPqq(&gb_cpu.af); }

// End

void LDHLSPe() {  // LDHL SP, e
	int8_t e = (int8_t)rdByte(++gb_cpu.pc);
	gb_cpu.hl = gb_cpu.sp + e;
	CLEAR_FLAGS;
	flagC8((gb_cpu.sp >> 8), e);
	flagH8((gb_cpu.sp >> 8), e);
	gb_cpu.m += 3;
	gb_cpu.pc++;
}

void LDnnmSP() {  // LD (nn), SP
	uint16_t destaddr = rdWord(++gb_cpu.pc);
	wrWord(destaddr, gb_cpu.sp);
	gb_cpu.m += 5;
	gb_cpu.pc += 2;
}

////////////////////////////////
/*----- ALU Instructions -----*/
////////////////////////////////

/* 8-Bit */ 

// ADDAr instructions expansion to all possibilities of r.
void ADDAr(uint8_t reg) { // ADD A, r
	CLEAR_FLAGS;
	flagC8(gb_cpu.a, reg);
	flagH8(gb_cpu.a, reg);
	gb_cpu.a += reg;
	flagZ8(gb_cpu.a);

	gb_cpu.m++;
	gb_cpu.pc++;
}

void ADDAa(uint8_t reg) { ADDAr(gb_cpu.a); }
void ADDAb(uint8_t reg) { ADDAr(gb_cpu.b); }
void ADDAc(uint8_t reg) { ADDAr(gb_cpu.c); }
void ADDAd(uint8_t reg) { ADDAr(gb_cpu.d); }
void ADDAe(uint8_t reg) { ADDAr(gb_cpu.e); }
void ADDAh(uint8_t reg) { ADDAr(gb_cpu.h); }
void ADDAl(uint8_t reg) { ADDAr(gb_cpu.l); }

// End

void ADDAn() { // ADD A, n
	uint8_t n = rdByte(++gb_cpu.pc);

	CLEAR_FLAGS;
	flagC8(gb_cpu.a, n);
	flagH8(gb_cpu.a, n);
	gb_cpu.a += n;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ADDAHLm() { // ADD A, (HL)
	uint8_t val = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagC8(gb_cpu.a, val);
	flagH8(gb_cpu.a, val);
	gb_cpu.a += val;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// ADC A,s functions. Each one represents a possibility to s
void ADCAr(uint8_t reg) { // ADC A, r
	uint8_t c = (gb_cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	// Modified flagC8 with 3 variables (a, reg and Carry)
	if ((uint16_t)(gb_cpu.a + reg + c) & 0x100)
		gb_cpu.f |= FLAGC;
	// Modified flagH8 with 3 variables (a, reg and Carry)
	if (((gb_cpu.a &= 0xF) + (reg &= 0xF) + c) & 0x10)
		gb_cpu.f |= FLAGH;
	gb_cpu.a += reg + c;
	flagZ8(gb_cpu.a);

	gb_cpu.m++;
	gb_cpu.pc++;
}

void ADCAa(uint8_t reg) { ADCAr(gb_cpu.a); }
void ADCAb(uint8_t reg) { ADCAr(gb_cpu.b); }
void ADCAc(uint8_t reg) { ADCAr(gb_cpu.c); }
void ADCAd(uint8_t reg) { ADCAr(gb_cpu.d); }
void ADCAe(uint8_t reg) { ADCAr(gb_cpu.e); }
void ADCAh(uint8_t reg) { ADCAr(gb_cpu.h); }
void ADCAl(uint8_t reg) { ADCAr(gb_cpu.l); }

// End

void ADCAn() { // ADC A, n
	uint8_t n = rdByte(++gb_cpu.pc);
	uint8_t c = (gb_cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	// Modified flagC8 with 3 variables (a, reg and Carry)
	if ((uint16_t)(gb_cpu.a + n + c) & 0x100)
		gb_cpu.f |= FLAGC;
	// Modified flagH8 with 3 variables (a, reg and Carry)
	if (((gb_cpu.a &= 0xF) + (n &= 0xF) + c) & 0x10)
		gb_cpu.f |= FLAGH;
	// Modified flagZ8 with 3 variables (a, reg, and Carry)
	gb_cpu.a += n + c;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ADCAHLm() { // ADC A, (HL)
	uint8_t n = rdByte(gb_cpu.hl);
	uint8_t c = (gb_cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	// Modified flagC8 with 3 variables (a, reg and Carry)
	if ((uint16_t)(gb_cpu.a + n + c) & 0x100)
		gb_cpu.f |= FLAGC;
	// Modified flagH8 with 3 variables (a, reg and Carry)
	if (((gb_cpu.a &= 0xF) + (n &= 0xF) + c) & 0x10)
		gb_cpu.f |= FLAGH;
	// Modified flagZ8 with 3 variables (a, reg, and Carry)
	gb_cpu.a += n + c;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// SUB s functions. Each one represents a possibility to s
void SUBr(uint8_t reg) { // SUB r
	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, reg);
	flagH8Sub(gb_cpu.a, reg);
	gb_cpu.a -= reg;

	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void SUBa(uint8_t reg) { SUBr(gb_cpu.a); }
void SUBb(uint8_t reg) { SUBr(gb_cpu.b); }
void SUBc(uint8_t reg) { SUBr(gb_cpu.c); }
void SUBd(uint8_t reg) { SUBr(gb_cpu.d); }
void SUBe(uint8_t reg) { SUBr(gb_cpu.e); }
void SUBh(uint8_t reg) { SUBr(gb_cpu.h); }
void SUBl(uint8_t reg) { SUBr(gb_cpu.l); }

// End

void SUBn() { // SUB n
	uint8_t n = rdByte(++gb_cpu.pc);

	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, n);
	flagH8Sub(gb_cpu.a, n);
	gb_cpu.a -= n;
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void SUBHLm() { // SUB (HL)
	uint8_t n = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, n);
	flagH8Sub(gb_cpu.a, n);
	gb_cpu.a -= n;
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;
	gb_cpu.m += 2;
	gb_cpu.pc++;
}


// SBC a, s functions. Each one represents a possibility to s
void SBCAr(uint8_t reg) {  // SBC A, r
	uint8_t c = (gb_cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	if ((int16_t)gb_cpu.a - reg - c < 0)
		gb_cpu.f |= FLAGC;
	if (((gb_cpu.a &= 0xF) - (reg &= 0xF) - c) < 0)
		gb_cpu.f |= FLAGH;
	gb_cpu.a = gb_cpu.a - reg - c;
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void SBCAa(uint8_t reg) { SBCAr(gb_cpu.a); }
void SBCAb(uint8_t reg) { SBCAr(gb_cpu.b); }
void SBCAc(uint8_t reg) { SBCAr(gb_cpu.c); }
void SBCAd(uint8_t reg) { SBCAr(gb_cpu.d); }
void SBCAe(uint8_t reg) { SBCAr(gb_cpu.e); }
void SBCAh(uint8_t reg) { SBCAr(gb_cpu.h); }
void SBCAl(uint8_t reg) { SBCAr(gb_cpu.l); }

// End

void SBCAn() {  // SBC A, n
	uint8_t n = rdByte(++gb_cpu.pc);
	uint8_t c = (gb_cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	if ((int16_t)gb_cpu.a - n - c < 0)
		gb_cpu.f |= FLAGC;
	if (((gb_cpu.a &= 0xF) - (n &= 0xF) - c) < 0)
		gb_cpu.f |= FLAGH;
	gb_cpu.a = gb_cpu.a - n - c;
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void SBCAHLm() {  // SBC A, (HL)
	uint8_t n = rdByte(gb_cpu.hl);
	uint8_t c = (gb_cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	if ((int16_t)gb_cpu.a - n - c < 0)
		gb_cpu.f |= FLAGC;
	if (((gb_cpu.a &= 0xF) - (n &= 0xF) - c) < 0)
		gb_cpu.f |= FLAGH;
	gb_cpu.a = gb_cpu.a - n - c;
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// AND s functions. Each one represents a possibility to s
void ANDr(uint8_t reg) {  // AND r
	gb_cpu.a &= reg;
	CLEAR_FLAGS;
	gb_cpu.f |= FLAGH;
	flagZ8(gb_cpu.a);

	gb_cpu.m++;
	gb_cpu.pc++;
}

void ANDa(uint8_t reg) { ANDr(gb_cpu.a); }
void ANDb(uint8_t reg) { ANDr(gb_cpu.b); }
void ANDc(uint8_t reg) { ANDr(gb_cpu.c); }
void ANDd(uint8_t reg) { ANDr(gb_cpu.d); }
void ANDe(uint8_t reg) { ANDr(gb_cpu.e); }
void ANDh(uint8_t reg) { ANDr(gb_cpu.h); }
void ANDl(uint8_t reg) { ANDr(gb_cpu.l); }

// End

void ANDn() {  // AND n
	gb_cpu.a &= rdByte(++gb_cpu.pc);
	CLEAR_FLAGS;
	gb_cpu.f |= FLAGH;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ANDHLm() {  // AND (HL)
	gb_cpu.a &= rdByte(gb_cpu.hl);
	CLEAR_FLAGS;
	gb_cpu.f |= FLAGH;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// OR s functions. Each one represents a possibility to s
void ORr(uint8_t reg) {  // OR r
	gb_cpu.a |= reg;
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m++;
	gb_cpu.pc++;
}

void ORa(uint8_t reg) { ORr(gb_cpu.a); }
void ORb(uint8_t reg) { ORr(gb_cpu.b); }
void ORc(uint8_t reg) { ORr(gb_cpu.c); }
void ORd(uint8_t reg) { ORr(gb_cpu.d); }
void ORe(uint8_t reg) { ORr(gb_cpu.e); }
void ORh(uint8_t reg) { ORr(gb_cpu.h); }
void ORl(uint8_t reg) { ORr(gb_cpu.l); }

// End

void ORn() {  // OR n
	gb_cpu.a |= rdByte(++gb_cpu.pc);
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ORHLm() {  // OR (HL)
	gb_cpu.a |= rdByte(gb_cpu.hl);
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// XOR s functions. Each one represents a possibility to s
void XORr(uint8_t reg) {  // XOR r
	gb_cpu.a ^= reg;
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m++;
	gb_cpu.pc++;
}

void XORa(uint8_t reg) { XORr(gb_cpu.a); }
void XORb(uint8_t reg) { XORr(gb_cpu.b); }
void XORc(uint8_t reg) { XORr(gb_cpu.c); }
void XORd(uint8_t reg) { XORr(gb_cpu.d); }
void XORe(uint8_t reg) { XORr(gb_cpu.e); }
void XORh(uint8_t reg) { XORr(gb_cpu.h); }
void XORl(uint8_t reg) { XORr(gb_cpu.l); }

// End

void XORn() {  // XOR n
	gb_cpu.a ^= rdByte(++gb_cpu.pc);
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void XORHLm() {  // XOR (HL)
	gb_cpu.a ^= rdByte(gb_cpu.hl);
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// CP s functions. Each one represents a possibility to s
// NOTA
// They are the same as SUBr, SUBn and SUBHL but the results
// are thrown away.

// CPr instructions expansion to all possibilities of r.
void CPr(uint8_t reg) {
	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, reg);
	flagH8Sub(gb_cpu.a, reg);
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void CPa(uint8_t reg) { CPr(gb_cpu.a); }
void CPb(uint8_t reg) { CPr(gb_cpu.b); }
void CPc(uint8_t reg) { CPr(gb_cpu.c); }
void CPd(uint8_t reg) { CPr(gb_cpu.d); }
void CPe(uint8_t reg) { CPr(gb_cpu.e); }
void CPh(uint8_t reg) { CPr(gb_cpu.h); }
void CPl(uint8_t reg) { CPr(gb_cpu.l); }

// End

void CPn() {
	uint8_t n = rdByte(++gb_cpu.pc);

	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, n);
	flagH8Sub(gb_cpu.a, n);
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void CPHLm() {
	uint8_t n = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, n);
	flagH8Sub(gb_cpu.a, n);
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

// INCr instructions expansion to all possibilities of r.
void INCr(uint8_t *reg) {
	CLEAR_FLAGS;
	flagH8(*reg, 1);
	(*reg)++;
	flagZ8(*reg);

	gb_cpu.m++;
	gb_cpu.pc++;
}

void INCa(uint8_t *reg) { INCr(&gb_cpu.a); }
void INCb(uint8_t *reg) { INCr(&gb_cpu.b); }
void INCc(uint8_t *reg) { INCr(&gb_cpu.c); }
void INCd(uint8_t *reg) { INCr(&gb_cpu.d); }
void INCe(uint8_t *reg) { INCr(&gb_cpu.e); }
void INCh(uint8_t *reg) { INCr(&gb_cpu.h); }
void INCl(uint8_t *reg) { INCr(&gb_cpu.l); }

// End

void INCHLm() {
	uint8_t val = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagH8(val, 1);
	val++;
	flagZ8(val);
	wrByte(gb_cpu.hl, val);

	gb_cpu.m += 3;
	gb_cpu.pc++;
}

// DECr instructions expansion to all possibilities of r.
void DECr(uint8_t *reg) {
	CLEAR_FLAGS;
	flagH8Sub(*reg, 1);
	(*reg)--;
	flagZ8(*reg);
	gb_cpu.f |= FLAGN;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void DECa(uint8_t *reg) { DECr(&gb_cpu.a); }
void DECb(uint8_t *reg) { DECr(&gb_cpu.b); }
void DECc(uint8_t *reg) { DECr(&gb_cpu.c); }
void DECd(uint8_t *reg) { DECr(&gb_cpu.d); }
void DECe(uint8_t *reg) { DECr(&gb_cpu.e); }
void DECh(uint8_t *reg) { DECr(&gb_cpu.h); }
void DECl(uint8_t *reg) { DECr(&gb_cpu.l); }

// END

void DECHLm() {
	uint8_t val = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagH8Sub(val, 1);
	val--;
	flagZ8(val);
	gb_cpu.f |= FLAGN;
	wrByte(gb_cpu.hl, val);

	gb_cpu.m += 3;
	gb_cpu.pc++;
}

/*----- 16-Bit -----*/
// ADDHLss instructions expansion to all possibilities of ss.
void ADDHLss(uint16_t reg) {  // AD HL, ss (register pair)
	CLEAR_N;
	flagC16(gb_cpu.hl, reg);
	flagH16(gb_cpu.hl, reg);
	gb_cpu.hl += reg;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ADDHLBC(uint16_t reg) { ADDHLss(gb_cpu.bc); }
void ADDHLDE(uint16_t reg) { ADDHLss(gb_cpu.de); }
void ADDHLHL(uint16_t reg) { ADDHLss(gb_cpu.hl); }
void ADDHLSP(uint16_t reg) { ADDHLss(gb_cpu.sp); }

// End

void ADDSPe() { // ADD SP, e
	uint8_t e = rdByte(++gb_cpu.pc);

	CLEAR_FLAGS;
	flagC16(gb_cpu.sp, e);
	flagH16(gb_cpu.sp, e);
	gb_cpu.sp += e;

	gb_cpu.m += 4;
	gb_cpu.pc++;
}

// INCss instructions expansion to all possibilities of ss.
void INCss(uint16_t *reg) { // INC ss
	++(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void INCBC(uint16_t *reg) { INCss(&gb_cpu.bc); }
void INCDE(uint16_t *reg) { INCss(&gb_cpu.de); }
void INCHL(uint16_t *reg) { INCss(&gb_cpu.hl); }
void INCSP(uint16_t *reg) { INCss(&gb_cpu.sp); }

// End

// DECss instructions expansion to all possibilities of ss.
void DECss(uint16_t *reg) { // DEC ss
	--(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void DECBC(uint16_t *reg) { DECss(&gb_cpu.bc); }
void DECDE(uint16_t *reg) { DECss(&gb_cpu.de); }
void DECHL(uint16_t *reg) { DECss(&gb_cpu.hl); }
void DECSP(uint16_t *reg) { DECss(&gb_cpu.sp); }

// End

/*----- Rotate Shift Instructions -----*/
void RLCA() {
	uint8_t bit = (gb_cpu.a & 0x80) ? 1 : 0;

	CLEAR_FLAGS;
	if (bit) gb_cpu.f |= FLAGC;
	gb_cpu.a <<= 1;
	gb_cpu.a |= bit;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void RLA() {
	uint8_t bita = (gb_cpu.a & 0x80) ? 1 : 0;
	uint8_t carry = (gb_cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (bita) gb_cpu.f |= FLAGC;
	gb_cpu.a <<= 1;
	gb_cpu.a |= carry;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void RRCA() {
	uint8_t bit = (gb_cpu.a & 0x1) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (bit) gb_cpu.f |= FLAGC;
	gb_cpu.a >>= 1;
	gb_cpu.a |= bit;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void RRA() {
	uint8_t bita = (gb_cpu.a & 0x1) ? 1 : 0;
	uint8_t carry = (gb_cpu.f & FLAGC) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (bita) gb_cpu.f |= FLAGC;
	gb_cpu.a >>= 1;
	gb_cpu.a |= carry;

	gb_cpu.m++;
	gb_cpu.pc++;
}

// RLC m functions. Each one represents a possibility to m
// RLCr instructions expansion to all possibilities of r.
void RLCr(uint8_t *reg) { // RLC r
	uint8_t bit = (*reg & 0x80) ? 1 : 0;

	CLEAR_FLAGS;
	if (bit) gb_cpu.f |= FLAGC;
	*reg <<= 1;
	*reg |= bit;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void RLCa(uint8_t *reg) { RLCr(&gb_cpu.a); }
void RLCb(uint8_t *reg) { RLCr(&gb_cpu.b); }
void RLCc(uint8_t *reg) { RLCr(&gb_cpu.c); }
void RLCd(uint8_t *reg) { RLCr(&gb_cpu.d); }
void RLCe(uint8_t *reg) { RLCr(&gb_cpu.e); }
void RLCh(uint8_t *reg) { RLCr(&gb_cpu.h); }
void RLCl(uint8_t *reg) { RLCr(&gb_cpu.l); }

// End


void RLCHLm() { // RLC (HL)
	uint8_t m = rdByte(gb_cpu.hl);
	uint8_t bit = (m & 0x80) ? 1 : 0;

	CLEAR_FLAGS;
	if (bit) gb_cpu.f |= FLAGC;
	m <<= 1;
	m |= bit;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// RL m functions. Each one represents a possibility to m
void RLr(uint8_t *reg) { // RL r
	uint8_t carry = (gb_cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (*reg & 0x80) gb_cpu.f |= FLAGC;
	*reg <<= 1;
	*reg |= carry;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void RLa(uint8_t *reg) { RLr(&gb_cpu.a); }
void RLb(uint8_t *reg) { RLr(&gb_cpu.b); }
void RLc(uint8_t *reg) { RLr(&gb_cpu.c); }
void RLd(uint8_t *reg) { RLr(&gb_cpu.d); }
void RLe(uint8_t *reg) { RLr(&gb_cpu.e); }
void RLh(uint8_t *reg) { RLr(&gb_cpu.h); }
void RLl(uint8_t *reg) { RLr(&gb_cpu.l); }

// End

void RLHLm() { // RL (HL)
	uint8_t m = rdByte(gb_cpu.hl);
	uint8_t bitr = (m & 0x80) ? 1 : 0;
	uint8_t carry = (gb_cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (bitr) gb_cpu.f |= FLAGC;
	m <<= 1;
	m |= carry;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// RRC m functions. Each one represents a possibility to m
void RRCr(uint8_t *reg) { // RRC r
	uint8_t bit = (*reg & 0x1) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (bit) gb_cpu.f |= FLAGC;
	*reg >>= 1;
	*reg |= bit;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void RRCa(uint8_t *reg) { RRCr(&gb_cpu.a); }
void RRCb(uint8_t *reg) { RRCr(&gb_cpu.b); }
void RRCc(uint8_t *reg) { RRCr(&gb_cpu.c); }
void RRCd(uint8_t *reg) { RRCr(&gb_cpu.d); }
void RRCe(uint8_t *reg) { RRCr(&gb_cpu.e); }
void RRCh(uint8_t *reg) { RRCr(&gb_cpu.h); }
void RRCl(uint8_t *reg) { RRCr(&gb_cpu.l); }

// End

void RRCHLm() { // RRCHL (HL)
	uint8_t m = rdByte(gb_cpu.hl);
	uint8_t bitr = (m & 0x80) ? 0x80 : 0;
	uint8_t carry = (gb_cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (bitr) gb_cpu.f |= FLAGC;
	m >>= 1;
	m |= carry;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// RR m functions. Each one represents a possibility to m
void RRr(uint8_t *reg) { // RR r
	uint8_t carry = (gb_cpu.f & FLAGC) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (*reg & 0x1) gb_cpu.f |= FLAGC;
	*reg >>= 1;
	*reg |= carry;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void RRa(uint8_t *reg) { RRr(&gb_cpu.a); }
void RRb(uint8_t *reg) { RRr(&gb_cpu.b); }
void RRc(uint8_t *reg) { RRr(&gb_cpu.c); }
void RRd(uint8_t *reg) { RRr(&gb_cpu.d); }
void RRe(uint8_t *reg) { RRr(&gb_cpu.e); }
void RRh(uint8_t *reg) { RRr(&gb_cpu.h); }
void RRl(uint8_t *reg) { RRr(&gb_cpu.l); }

// End

void RRHLm() { // RR (HL)
	uint8_t m = rdByte(gb_cpu.hl);
	uint8_t carry = (gb_cpu.f & FLAGC) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (m & 0x1) gb_cpu.f |= FLAGC;
	m >>= 1;
	m |= carry;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// SLA m functions. Each one represents a possibility to m
void SLAr(uint8_t *reg) { // SLA r
	CLEAR_FLAGS;
	gb_cpu.f = (*reg & 0x80) ? FLAGC : 0;
	*reg <<= 1;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void SLAa(uint8_t *reg) { SLAr(&gb_cpu.a); }
void SLAb(uint8_t *reg) { SLAr(&gb_cpu.b); }
void SLAc(uint8_t *reg) { SLAr(&gb_cpu.c); }
void SLAd(uint8_t *reg) { SLAr(&gb_cpu.d); }
void SLAe(uint8_t *reg) { SLAr(&gb_cpu.e); }
void SLAh(uint8_t *reg) { SLAr(&gb_cpu.h); }
void SLAl(uint8_t *reg) { SLAr(&gb_cpu.l); }

// End

void SLAHLm() { // SLA HL
	uint8_t m = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	gb_cpu.f = (m & 0x80) ? FLAGC : 0;
	m <<= 1;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// SRA m functions. Each one represents a possibility to m
void SRAr(uint8_t *reg) { // SRA r
	uint8_t bitr = *reg & 0x80;

	CLEAR_FLAGS;
	gb_cpu.f = (*reg & 0x1) ? FLAGC : 0;
	*reg >>= 1;
	*reg |= bitr;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void SRAa(uint8_t *reg) {SRAr(&gb_cpu.a); }
void SRAb(uint8_t *reg) {SRAr(&gb_cpu.b); }
void SRAc(uint8_t *reg) {SRAr(&gb_cpu.c); }
void SRAd(uint8_t *reg) {SRAr(&gb_cpu.d); }
void SRAe(uint8_t *reg) {SRAr(&gb_cpu.e); }
void SRAh(uint8_t *reg) {SRAr(&gb_cpu.h); }
void SRAl(uint8_t *reg) {SRAr(&gb_cpu.l); }

// End

void SRAHLm() { // SRA (HL)
	uint8_t m = rdByte(gb_cpu.hl);
	uint8_t bitr = m & 0x80;

	CLEAR_FLAGS;
	gb_cpu.f = (m & 0x1) ? FLAGC : 0;
	m >>= 1;
	m |= bitr;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// SRL m functions. Each one represents a possibility to m
void SRLr(uint8_t *reg) { // SRL r
	CLEAR_FLAGS;
	gb_cpu.f = (*reg & 0x1) ? FLAGC : 0;
	*reg >>= 1;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void SRLa(uint8_t *reg) { SRLr(&gb_cpu.a); }
void SRLb(uint8_t *reg) { SRLr(&gb_cpu.b); }
void SRLc(uint8_t *reg) { SRLr(&gb_cpu.c); }
void SRLd(uint8_t *reg) { SRLr(&gb_cpu.d); }
void SRLe(uint8_t *reg) { SRLr(&gb_cpu.e); }
void SRLh(uint8_t *reg) { SRLr(&gb_cpu.h); }
void SRLl(uint8_t *reg) { SRLr(&gb_cpu.l); }

// End

void SRLHLm() { // SRL (HL)
	uint8_t m = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	gb_cpu.f = (m & 0x1) ? FLAGC : 0;
	m >>= 1;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

// SWAP m functions. Each one represents a possibility to m
void SWAPr(uint8_t *reg) { // SWAP r
	*reg = ((*reg & 0xF0) >> 4) | ((*reg & 0xF) << 4);
	CLEAR_FLAGS;
	flagZ8(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void SWAPa(uint8_t *reg) { SWAPr(&gb_cpu.a); }
void SWAPb(uint8_t *reg) { SWAPr(&gb_cpu.b); }
void SWAPc(uint8_t *reg) { SWAPr(&gb_cpu.c); }
void SWAPd(uint8_t *reg) { SWAPr(&gb_cpu.d); }
void SWAPe(uint8_t *reg) { SWAPr(&gb_cpu.e); }
void SWAPh(uint8_t *reg) { SWAPr(&gb_cpu.h); }
void SWAPl(uint8_t *reg) { SWAPr(&gb_cpu.l); }

// End

void SWAPHLm() { // SWAP (HL)
	uint8_t m = rdByte(gb_cpu.hl);

	m = ((m & 0xF0) >> 4) | ((m & 0xF) << 4);
	CLEAR_FLAGS;
	flagZ8(m);
	wrByte(gb_cpu.hl, m);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

/*---- Bit Operations -----*/
void BITr(uint8_t bit, uint8_t reg) { // Bit b, r
	CLEAR_N;
	gb_cpu.f = (bit & reg) ? 0 : FLAGZ;
	gb_cpu.f |= FLAGH;

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void BITHL(uint8_t bit) { // BIT b, (HL)
	CLEAR_N;
	gb_cpu.f = (bit & rdByte(gb_cpu.hl)) ? 0 : FLAGZ;
	gb_cpu.f |= FLAGH;

	gb_cpu.m += 3;
	gb_cpu.pc += 2;
}

void SETr(uint8_t bit, uint8_t *reg) { // SET b, r
	*reg |= bit;

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void SETHL(uint8_t bit) { // SET (HL)
	wrByte(gb_cpu.hl, rdByte(gb_cpu.hl) | bit);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

void RESr(uint8_t bit, uint8_t *reg) {
	*reg &= !bit;

	gb_cpu.m += 2;
	gb_cpu.pc += 2;
}

void RESHL(uint8_t bit) {
	wrByte(gb_cpu.hl, rdByte(gb_cpu.hl) & !bit);

	gb_cpu.m += 4;
	gb_cpu.pc += 2;
}

/*----- Jump Instructions -----*/
void JPnn() { // JP nn
	gb_cpu.pc = rdWord(++gb_cpu.pc);

	gb_cpu.m += 4;
}

// JPccnn functions. Each one represents a possibility to cc
void JPNZnn() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
	else {
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);
		gb_cpu.m += 4;
	}
}

void JPZnn() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);
		gb_cpu.m += 4;
	}
	else {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
}

void JPNCnn() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
	else {
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);
		gb_cpu.m += 4;
	}
}

void JPCnn() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);
		gb_cpu.m += 4;
	}
	else {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
}


/*
NOTA
According to GB Programming Manual, the e parameter is different from the others. Now it can be from -127 to +129.
This function must be watched!
*/

void JRe() {
	int8_t e = rdByte(++gb_cpu.pc);

	gb_cpu.pc += e;

	gb_cpu.m += 3;
}

/*
void JRcce(uint8_t cc) { // JR cc, e
	uint8_t e = rdByte(++gb_cpu.pc);
	switch (cc) {
	case 0b00:	// NZ -> Flag Z = 0
		gb_cpu.pc = (gb_cpu.f & FLAGZ) ? gb_cpu.pc : rdByte(++gb_cpu.pc) + e;
		gb_cpu.m += 3;
		break;

	case 0b01: // Z -> Flag Z = 1
		gb_cpu.pc = (gb_cpu.f & FLAGZ) ? rdWord(++gb_cpu.pc) + e : gb_cpu.pc;
		gb_cpu.m += 3;
		break;

	case 0b10: // NC -> Flag C = 0
		gb_cpu.pc = (gb_cpu.f & FLAGC) ? gb_cpu.pc : rdWord(++gb_cpu.pc) + e;
		gb_cpu.m += 3;
		break;

	case 0b11: // C -> Flag C = 1
		gb_cpu.pc = (gb_cpu.f & FLAGC) ? rdWord(++gb_cpu.pc) + e : gb_cpu.pc;
		gb_cpu.m += 3;
		break;

	default:
		gb_cpu.pc += 2;
		gb_cpu.m += 2;
	}
}
*/

// JRcce functions. Each one represents a possibility to cc
void JRNZe() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc += 2;
		gb_cpu.m += 2;
	}
	else {
		gb_cpu.pc += (int8_t) rdByte(gb_cpu.pc + 1);
		gb_cpu.m += 3;
	}
}

void JRZe() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc += (int8_t)rdByte(gb_cpu.pc + 1);
		gb_cpu.m += 3;
	}
	else {
		gb_cpu.pc += 2;
		gb_cpu.m += 2;
	}
}

void JRNCe() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc += 2;
		gb_cpu.m += 2;
	}
	else {
		gb_cpu.pc += (int8_t)rdWord(gb_cpu.pc + 1);
		gb_cpu.m += 3;
	}
}
	

void JRCe() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc += (int8_t)rdWord(gb_cpu.pc + 1);
		gb_cpu.m += 3;
	}
	else {
		gb_cpu.pc += 2;
		gb_cpu.m += 2;
	}
}

void JPHLm() { // JP (HL)
	gb_cpu.pc = rdByte(gb_cpu.hl);
	gb_cpu.m += 1;
}

/*----- Call and Return Instructions -----*/
void CALLnn() {
	gb_cpu.sp -= 2;
	wrWord(gb_cpu.sp, gb_cpu.pc + 3);
	gb_cpu.pc = rdWord(gb_cpu.pc + 1);

	gb_cpu.m += 6;
}

// CALLccnn functions. Each one represents a possibility to cc
void CALLNZnn() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
	else {
		gb_cpu.sp -= 2;
		wrWord(gb_cpu.sp, gb_cpu.pc + 3);
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);

		gb_cpu.m += 6;
	}
}

void CALLZnn() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.sp -= 2;
		wrWord(gb_cpu.sp, gb_cpu.pc + 3);
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);

		gb_cpu.m += 6;
	}
	else {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
}

void CALLNCnn() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
	else {
		gb_cpu.sp -= 2;
		wrWord(gb_cpu.sp, gb_cpu.pc + 3);
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);

		gb_cpu.m += 6;
	}
}

void CALLCnn() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.sp -= 2;
		wrWord(gb_cpu.sp, gb_cpu.pc + 3);
		gb_cpu.pc = rdWord(gb_cpu.pc + 1);

		gb_cpu.m += 6;
	}
	else {
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
}

void RET() {
	gb_cpu.pc = rdWord(gb_cpu.sp);
	gb_cpu.sp += 2;

	gb_cpu.m += 4;
}

void RETI() {
	gb_cpu.pc = rdWord(gb_cpu.sp);
	gb_cpu.sp += 2;
	gb_cpu.ime = 1;


	gb_cpu.m += 4;
}

// RETcc functions. Each one represents a possibility to cc
void RETNZ() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc++;
		gb_cpu.m += 2;
	}
	else {
		gb_cpu.pc = rdWord(gb_cpu.sp);
		gb_cpu.m += 5;
	}
}

void RETZ() {
	if (gb_cpu.f & FLAGZ) {
		gb_cpu.pc = rdWord(gb_cpu.sp);
		gb_cpu.m += 5;
	}
	else {
		gb_cpu.pc++;
		gb_cpu.m += 2;
	}
}

void RETNC() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc++;
		gb_cpu.m += 2;
	}
	else {
		gb_cpu.pc = rdWord(gb_cpu.sp);
		gb_cpu.m += 5;
	}
}

void RETC() {
	if (gb_cpu.f & FLAGC) {
		gb_cpu.pc = rdWord(gb_cpu.sp);
		gb_cpu.m += 5;
	}
	else {
		gb_cpu.pc++;
		gb_cpu.m += 2;
	}
}

// RST instructions expansion to all possibilities of values.
void RST(uint8_t t) {
	gb_cpu.sp -= 2;
	wrWord(gb_cpu.sp, gb_cpu.pc + 1);

	gb_cpu.pc = t;
	gb_cpu.m += 4;
}

void RST00() { RST(0x00); }
void RST08() { RST(0x08); }
void RST10() { RST(0x10); }
void RST18() { RST(0x18); }
void RST20() { RST(0x20); }
void RST28() { RST(0x28); }
void RST30() { RST(0x30); }
void RST38() { RST(0x38); }

// End

/*----- General-Purpose Arithmetic Operations and CPU Control Instructions -----*/
/*
	NOTA
	Different implementation!
	Source: https://stackoverflow.com/questions/8119577/z80-daa-instruction
	Worth viewing: http://forums.nesdev.com/viewtopic.php?t=9088
*/
void DAA() {
	uint8_t a = gb_cpu.a;
	CLEAR_H;

	if (gb_cpu.f & FLAGC) {
		if (gb_cpu.f & FLAGH || (a & 0x0F) > 0x9) {
			gb_cpu.a += 6;
		}
		if (gb_cpu.f & FLAGC || (a & 0xF0) > 0x90) {
			gb_cpu.a += 60;
			gb_cpu.f &= FLAGC;
		}
	}
	else {
		if (gb_cpu.f & FLAGH || (a & 0x0F) > 0x9) {
			gb_cpu.a -= 6;
		}
		if (gb_cpu.f & FLAGC || (a & 0xF0) > 0x90) {
			gb_cpu.a -= 60;
			gb_cpu.f &= FLAGC;
		}
	}
	if (gb_cpu.a == 0) gb_cpu.f &= FLAGZ;

	gb_cpu.pc++;
	gb_cpu.m++;
}

void CPL() {
	gb_cpu.f &= FLAGH;
	gb_cpu.f &= FLAGN;
	gb_cpu.a = ~gb_cpu.a;

	gb_cpu.pc++;
	gb_cpu.m++;
}

void NOP() {
	gb_cpu.pc++;
	gb_cpu.m++;
}

void CCF() {
	gb_cpu.f ^= FLAGC;
	CLEAR_H;
	CLEAR_N;

	gb_cpu.pc++;
	gb_cpu.m++;
}

void SCF() {
	gb_cpu.f &= FLAGC;
	CLEAR_H;
	CLEAR_N;

	gb_cpu.pc++;
	gb_cpu.m++;
}

void DI() {
	gb_cpu.ime = false;

	gb_cpu.pc++;
	gb_cpu.m++;
}

void EI() {
	gb_cpu.ime = true;
	
	gb_cpu.pc++;
	gb_cpu.m++;
}

void HALT() {

}

void STOP() {

}

/*
NOTA
Not yet implemented instructions:
HALT
STOP
*/

// Null instruction
void CPU_NULL(void) {

}

// Prefixed CB instructions
void CB(void) {
	printf("CB instruction \n");
}

const void(*instructions[0x100])() = {
	// 0x00
	NOP,	LDBCnn,		LDBCmA,		INCBC,
	INCb, DECb, LDbn, RLCA,
	LDnnmSP, ADDHLBC, LDABCm, DECBC,
	INCc, DECc, LDcn, RRA,

	// 0x10
	STOP, LDDEnn, LDDEmA, INCDE,
	INCd, DECd, LDdn, RLA,
	JRe, ADDHLDE, LDADEm, DECDE,
	INCe, DECe, LDen, RRA,

	// 0x20
	JRNZe, LDHLnn, LDHLImA, INCHL,
	INCh, DECh, LDhn, DAA, 
	JRZe, ADDHLHL, LDAHLIm, DECHL,
	INCl, DECl, LDln, CPL,

	// 0x30
	JRNCe, LDSPnn, LDHLDmA, INCSP,
	INCHLm, DECHLm, LDHLmn, SCF,
	JRCe, ADDHLSP, LDAHLDm, DECSP,
	INCa, DECa, LDan, CCF,

	// 0x40
	LDbb, LDbc, LDbd, LDbe,
	LDbh, LDbl, LDbHLm, LDba,
	LDcb, LDcc, LDcd, LDce,
	LDch, LDcl, LDcHLm, LDca,

	// 0x50
	LDdb, LDdc, LDdd, LDde,
	LDdh, LDdl, LDdHLm, LDda,
	LDeb, LDec, LDed, LDee,
	LDeh, LDel, LDeHLm, LDca,

	// 0x60
	LDhb, LDhc, LDhd, LDhe,
	LDhh, LDhl, LDhHLm, LDha,
	LDlb, LDlc, LDld, LDle,
	LDlh, LDll, LDlHLm, LDla,

	// 0x70
	LDHLmb, LDHLmc, LDHLmd, LDHLme,
	LDHLmh, LDHLml, HALT, LDHLma,
	LDab, LDac, LDad, LDae,
	LDah, LDal, LDaHLm, LDaa,

	// 0x80
	ADDAb, ADDAc, ADDAd, ADDAe,
	ADDAh, ADDAl, ADDAHLm, ADDAa,
	ADCAb, ADCAc, ADCAd, ADCAe,
	ADCAh, ADCAl, ADCAHLm, ADCAa,

	// 0x90
	SUBb, SUBc, SUBd, SUBe,
	SUBh, SUBl, SUBHLm, SUBa,
	SBCAb, SBCAc, SBCAd, SBCAe,
	SBCAh, SBCAl, SBCAHLm, SBCAa,

	// 0xA0
	ANDb, ANDc, ANDd, ANDe,
	ANDh, ANDl, ANDHLm, ANDa,
	XORb, XORc, XORd, XORe,
	XORh, XORl, XORHLm, XORa,

	// 0xB0
	ORb, ORc, ORd, ORe,
	ORh, ORl, ORHLm, ORa,
	CPb, CPc,CPd, CPe,
	CPh, CPl, CPHLm, CPa,

	// 0xC0
	RETNZ, POPBC, JPNZnn, JPnn,
	CALLNZnn, PUSHBC, ADDAn, RST00,
	RETZ, RET, JPZnn, CB,
	CALLZnn, CALLnn, ADCAn, RST08,

	// 0xD0
	RETNC, POPDE, JPNCnn, CPU_NULL,
	CALLNCnn, PUSHDE, SUBn, RST10,
	RETC, RETI, JPCnn, CPU_NULL,
	CALLCnn, CPU_NULL, SBCAn, RST18,

	// 0xE0
	LDnmA,       POPHL,       LDCmA,     CPU_NULL,
	CPU_NULL,    PUSHHL,      ANDn,      RST20,
	ADDSPe,      JPHLm,       LDnnmA,    CPU_NULL,
	CPU_NULL,    CPU_NULL,    XORn,      RST28,

	// 0xF0
	LDAnm,       POPAF,       LDACm,     DI,
	CPU_NULL,    PUSHAF,      ORn,	     RST30,
	LDHLSPe,	 LDSPHL,	  LDAnnm,    EI,
	CPU_NULL,    CPU_NULL,    CPn,		 RST38
};

const void(*CBinstructions[0x100])() = {
	// 0x00
	RLCb, RLCc, RLCd, RLCe,
	RLCh, RLCl, RLCHLm, RLCa,
	RRCb, RRCc, RRCd, RRCe,
	RRCh, RRCl, RRCHLm, RRCa,

	// 0x10
	RLb, RLc, RLd, RLe,
	RLh, RLl, RLHLm, RLa,
	RRb, RRc, RRd, RRe,
	RRh, RRl, RRHLm, RRa,

	// 0x20
	SWAPb, SWAPc, SWAPd, SWAPe,
	SWAPh, SWAPl, SWAPHLm, SWAPa,
	SRLb, SRLc, SRLd, SRLe,
	SRLh, SRLl, SRLHLm, SRLa,

	// 0x30
	// NOTA: Stopped here!
};