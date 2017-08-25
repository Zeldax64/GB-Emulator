#include <stdio.h>

#include "cpu.h"

/*----- CPU Init function -----*/
// Verify GB's cpu initialization
void CPU_init() {
	gb_cpu.pc = 0x100;
	gb_cpu.af = 0x01B0;
	gb_cpu.bc = 0x0013;
	gb_cpu.de = 0x00D8;
	gb_cpu.hl = 0x014D;
	gb_cpu.sp = 0xFFFE;
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
void LDAA() { LDrr(&gb_cpu.a, &gb_cpu.a); }
void LDAB() { LDrr(&gb_cpu.a, &gb_cpu.b); }
void LDAC() { LDrr(&gb_cpu.a, &gb_cpu.c); }
void LDAD() { LDrr(&gb_cpu.a, &gb_cpu.d); }
void LDAE() { LDrr(&gb_cpu.a, &gb_cpu.e); }
void LDAH() { LDrr(&gb_cpu.a, &gb_cpu.h); }
void LDAL() { LDrr(&gb_cpu.a, &gb_cpu.l); }

// LDBr
void LDBA() { LDrr(&gb_cpu.b, &gb_cpu.a); }
void LDBB() { LDrr(&gb_cpu.b, &gb_cpu.b); }
void LDBC() { LDrr(&gb_cpu.b, &gb_cpu.c); }
void LDBD() { LDrr(&gb_cpu.b, &gb_cpu.d); }
void LDBE() { LDrr(&gb_cpu.b, &gb_cpu.e); }
void LDBH() { LDrr(&gb_cpu.b, &gb_cpu.h); }
void LDBL() { LDrr(&gb_cpu.b, &gb_cpu.l); }

// LDCr
void LDCA() { LDrr(&gb_cpu.c, &gb_cpu.a); }
void LDCB() { LDrr(&gb_cpu.c, &gb_cpu.b); }
void LDCC() { LDrr(&gb_cpu.c, &gb_cpu.c); }
void LDCD() { LDrr(&gb_cpu.c, &gb_cpu.d); }
void LDCE() { LDrr(&gb_cpu.c, &gb_cpu.e); }
void LDCH() { LDrr(&gb_cpu.c, &gb_cpu.h); }
void LDCL() { LDrr(&gb_cpu.c, &gb_cpu.l); }

// LDDr
void LDDA() { LDrr(&gb_cpu.d, &gb_cpu.a); }
void LDDB() { LDrr(&gb_cpu.d, &gb_cpu.b); }
void LDDC() { LDrr(&gb_cpu.d, &gb_cpu.c); }
void LDDD() { LDrr(&gb_cpu.d, &gb_cpu.d); }
void LDDE() { LDrr(&gb_cpu.d, &gb_cpu.e); }
void LDDH() { LDrr(&gb_cpu.d, &gb_cpu.h); }
void LDDL() { LDrr(&gb_cpu.d, &gb_cpu.l); }

// LDEr
void LDEA() { LDrr(&gb_cpu.e, &gb_cpu.a); }
void LDEB() { LDrr(&gb_cpu.e, &gb_cpu.b); }
void LDEC() { LDrr(&gb_cpu.e, &gb_cpu.c); }
void LDED() { LDrr(&gb_cpu.e, &gb_cpu.d); }
void LDEE() { LDrr(&gb_cpu.e, &gb_cpu.e); }
void LDEH() { LDrr(&gb_cpu.e, &gb_cpu.h); }
void LDEL() { LDrr(&gb_cpu.e, &gb_cpu.l); }

// LDHr
void LDHA() { LDrr(&gb_cpu.h, &gb_cpu.a); }
void LDHB() { LDrr(&gb_cpu.h, &gb_cpu.b); }
void LDHC() { LDrr(&gb_cpu.h, &gb_cpu.c); }
void LDHD() { LDrr(&gb_cpu.h, &gb_cpu.d); }
void LDHE() { LDrr(&gb_cpu.h, &gb_cpu.e); }
void LDHH() { LDrr(&gb_cpu.h, &gb_cpu.h); }
void LDHL() { LDrr(&gb_cpu.h, &gb_cpu.l); }

// LDLr
void LDLA() { LDrr(&gb_cpu.l, &gb_cpu.a); }
void LDLB() { LDrr(&gb_cpu.l, &gb_cpu.b); }
void LDLC() { LDrr(&gb_cpu.l, &gb_cpu.c); }
void LDLD() { LDrr(&gb_cpu.l, &gb_cpu.d); }
void LDLE() { LDrr(&gb_cpu.l, &gb_cpu.e); }
void LDLH() { LDrr(&gb_cpu.l, &gb_cpu.h); }
void LDLL() { LDrr(&gb_cpu.l, &gb_cpu.l); }

// End

// LDrn instructions expansion to all possibilities of r.
void LDrn(uint8_t *reg) { *(reg) = rdByte(++gb_cpu.pc); gb_cpu.pc++; gb_cpu.m += 2; }	   // LD r,n

void LDAn(uint8_t * reg){ LDrn(&gb_cpu.a); }
void LDBn(uint8_t * reg){ LDrn(&gb_cpu.b); }
void LDCn(uint8_t * reg){ LDrn(&gb_cpu.c); }
void LDDn(uint8_t * reg){ LDrn(&gb_cpu.d); }
void LDEn(uint8_t * reg){ LDrn(&gb_cpu.e); }
void LDHn(uint8_t * reg){ LDrn(&gb_cpu.h); }
void LDLn(uint8_t * reg){ LDrn(&gb_cpu.l); }

// End

// LDr(HL) instructions expansion to all possibilities of r.
void LDrHLm(uint8_t *reg) { *(reg) = rdByte(gb_cpu.hl); gb_cpu.pc++; gb_cpu.m += 2; }		   // LD r,(HL)

void LDAHLm(uint8_t *reg) { LDrHLm(&gb_cpu.a); }
void LDBHLm(uint8_t *reg) { LDrHLm(&gb_cpu.b); }
void LDCHLm(uint8_t *reg) { LDrHLm(&gb_cpu.c); }
void LDDHLm(uint8_t *reg) { LDrHLm(&gb_cpu.d); }
void LDEHLm(uint8_t *reg) { LDrHLm(&gb_cpu.e); }
void LDHHLm(uint8_t *reg) { LDrHLm(&gb_cpu.h); }
void LDLHLm(uint8_t *reg) { LDrHLm(&gb_cpu.l); }

// End

// LD(HL)r instructions expansion to all possibilities of r.
void LDHLmr(uint8_t reg) { wrByte(gb_cpu.hl, reg); gb_cpu.pc++; gb_cpu.m += 2; } // LD (HL), r

void LDHLmA(uint8_t reg) { LDHLmr(gb_cpu.a); }
void LDHLmB(uint8_t reg) { LDHLmr(gb_cpu.b); }
void LDHLmC(uint8_t reg) { LDHLmr(gb_cpu.c); }
void LDHLmD(uint8_t reg) { LDHLmr(gb_cpu.d); }
void LDHLmE(uint8_t reg) { LDHLmr(gb_cpu.e); }
void LDHLmH(uint8_t reg) { LDHLmr(gb_cpu.h); }
void LDHLmL(uint8_t reg) { LDHLmr(gb_cpu.l); }

// End

void LDHLn() { wrByte(gb_cpu.hl, rdByte(++gb_cpu.pc)); gb_cpu.pc++; gb_cpu.m += 3; }		   // LD (HL), n
void LDABC() { gb_cpu.a = rdByte(gb_cpu.bc); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD A, (BC)
void LDADE() { gb_cpu.a = rdByte(gb_cpu.de); gb_cpu.pc++;	gb_cpu.m += 2; }				   // LD A, (DE)
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

void ADDAA(uint8_t reg) { ADDAr(gb_cpu.a); }
void ADDAB(uint8_t reg) { ADDAr(gb_cpu.b); }
void ADDAC(uint8_t reg) { ADDAr(gb_cpu.c); }
void ADDAD(uint8_t reg) { ADDAr(gb_cpu.d); }
void ADDAE(uint8_t reg) { ADDAr(gb_cpu.e); }
void ADDAH(uint8_t reg) { ADDAr(gb_cpu.h); }
void ADDAL(uint8_t reg) { ADDAr(gb_cpu.l); }

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

void ADCAA(uint8_t reg) { ADCAr(gb_cpu.a); }
void ADCAB(uint8_t reg) { ADCAr(gb_cpu.b); }
void ADCAC(uint8_t reg) { ADCAr(gb_cpu.c); }
void ADCAD(uint8_t reg) { ADCAr(gb_cpu.d); }
void ADCAE(uint8_t reg) { ADCAr(gb_cpu.e); }
void ADCAH(uint8_t reg) { ADCAr(gb_cpu.h); }
void ADCAL(uint8_t reg) { ADCAr(gb_cpu.l); }

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

void SUBA(uint8_t reg) { SUBr(gb_cpu.a); }
void SUBB(uint8_t reg) { SUBr(gb_cpu.b); }
void SUBC(uint8_t reg) { SUBr(gb_cpu.c); }
void SUBD(uint8_t reg) { SUBr(gb_cpu.d); }
void SUBE(uint8_t reg) { SUBr(gb_cpu.e); }
void SUBH(uint8_t reg) { SUBr(gb_cpu.h); }
void SUBL(uint8_t reg) { SUBr(gb_cpu.l); }

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

void SBCAA(uint8_t reg) { SBCAr(gb_cpu.a); }
void SBCAB(uint8_t reg) { SBCAr(gb_cpu.b); }
void SBCAC(uint8_t reg) { SBCAr(gb_cpu.c); }
void SBCAD(uint8_t reg) { SBCAr(gb_cpu.d); }
void SBCAE(uint8_t reg) { SBCAr(gb_cpu.e); }
void SBCAH(uint8_t reg) { SBCAr(gb_cpu.h); }
void SBCAL(uint8_t reg) { SBCAr(gb_cpu.l); }

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

void ANDA(uint8_t reg) { ANDr(gb_cpu.a); }
void ANDB(uint8_t reg) { ANDr(gb_cpu.b); }
void ANDC(uint8_t reg) { ANDr(gb_cpu.c); }
void ANDD(uint8_t reg) { ANDr(gb_cpu.d); }
void ANDE(uint8_t reg) { ANDr(gb_cpu.e); }
void ANDH(uint8_t reg) { ANDr(gb_cpu.h); }
void ANDL(uint8_t reg) { ANDr(gb_cpu.l); }

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

void ORA(uint8_t reg) { ORr(gb_cpu.a); }
void ORB(uint8_t reg) { ORr(gb_cpu.b); }
void ORC(uint8_t reg) { ORr(gb_cpu.c); }
void ORD(uint8_t reg) { ORr(gb_cpu.d); }
void ORE(uint8_t reg) { ORr(gb_cpu.e); }
void ORH(uint8_t reg) { ORr(gb_cpu.h); }
void ORL(uint8_t reg) { ORr(gb_cpu.l); }

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

void XORA(uint8_t reg) { XORr(gb_cpu.a); }
void XORB(uint8_t reg) { XORr(gb_cpu.b); }
void XORC(uint8_t reg) { XORr(gb_cpu.c); }
void XORD(uint8_t reg) { XORr(gb_cpu.d); }
void XORE(uint8_t reg) { XORr(gb_cpu.e); }
void XORH(uint8_t reg) { XORr(gb_cpu.h); }
void XORL(uint8_t reg) { XORr(gb_cpu.l); }

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

void CPA(uint8_t reg) { CPr(gb_cpu.a); }
void CPB(uint8_t reg) { CPr(gb_cpu.b); }
void CPC(uint8_t reg) { CPr(gb_cpu.c); }
void CPD(uint8_t reg) { CPr(gb_cpu.d); }
void CPE(uint8_t reg) { CPr(gb_cpu.e); }
void CPH(uint8_t reg) { CPr(gb_cpu.h); }
void CPLr(uint8_t reg) { CPr(gb_cpu.l); }

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

void INCA(uint8_t *reg) { INCr(&gb_cpu.a); }
void INCB(uint8_t *reg) { INCr(&gb_cpu.b); }
void INCC(uint8_t *reg) { INCr(&gb_cpu.c); }
void INCD(uint8_t *reg) { INCr(&gb_cpu.d); }
void INCE(uint8_t *reg) { INCr(&gb_cpu.e); }
void INCH(uint8_t *reg) { INCr(&gb_cpu.h); }
void INCL(uint8_t *reg) { INCr(&gb_cpu.l); }

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

void DECA(uint8_t *reg) { DECr(&gb_cpu.a); }
void DECB(uint8_t *reg) { DECr(&gb_cpu.b); }
void DECC(uint8_t *reg) { DECr(&gb_cpu.c); }
void DECD(uint8_t *reg) { DECr(&gb_cpu.d); }
void DECE(uint8_t *reg) { DECr(&gb_cpu.e); }
void DECH(uint8_t *reg) { DECr(&gb_cpu.h); }
void DECL(uint8_t *reg) { DECr(&gb_cpu.l); }

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

void RLHL() { // RL (HL)
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

void RRHL() { // RR (HL)
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

void SRAHL() { // SRA (HL)
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

void JPHL() { // JP (HL)
	gb_cpu.pc = gb_cpu.hl;
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

void RST(uint8_t t) {
	gb_cpu.sp -= 2;
	wrWord(gb_cpu.sp, gb_cpu.pc + 1);

	gb_cpu.pc = t;
	gb_cpu.m += 4;
}

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


// Switch Statement 
//
//This function (CPU_cycle()) uses a switch statement approach to test the
//instruction set. When I finish all tests the switch statement
//will be replaced to a array of function pointers.
//
//void CPU_cycle() {
//	uint16_t opcode = rdWord(gb_cpu.pc);
//
//	switch (opcode) {
//		case 0x00: NOP(); break;
//		case 0x01: LDddnn(&gb_cpu.bc); break;
//		case 0x02: LDBCA(); break;
//		case 0x03: INCss(&gb_cpu.bc); break;
//		case 0x04: INCr(&gb_cpu.b); break;
//		case 0x05: DECr(&gb_cpu.b); break;
//		case 0x06: LDrn(&gb_cpu.b); break;
//		case 0x07: RLCA(); break;
//		case 0x08: LDnnSP(); break;
//		case 0x09: ADDHLss(gb_cpu.bc); break;
//		case 0x0A: LDABC(); break;
//		case 0x0B: DECss(&gb_cpu.bc); break;
//		case 0x0C: INCr(&gb_cpu.c); break;
//		case 0x0D: DECr(&gb_cpu.c); break;
//		case 0x0E: LDrn(gb_cpu.c); break;
//		case 0x0F: RRCA(); break;
//		case 0x10: STOP(); break;
//		case 0x11: LDddnn(&gb_cpu.de); break;
//		case 0x12: LDDEA(); break;
//		case 0x13: INCss(&gb_cpu.de); break;
//		case 0x14: INCr(&gb_cpu.d); break;
//		case 0x15: DECr(&gb_cpu.d); break;
//		case 0x16: LDrn(&gb_cpu.d); break;
//		case 0x17: RLA(); break;
//		case 0x18: JRe(); break;
//		case 0x19: ADDHLss(gb_cpu.de); break;
//		case 0x1A: LDADE(); break;
//		case 0x1B: DECss(&gb_cpu.de); break;
//		case 0x1C: INCr(&gb_cpu.e); break;
//		case 0x1D: DECr(&gb_cpu.e); break;
//		case 0x1E: LDrn(&gb_cpu.e); break;
//		case 0x1F: RRA(); break;
//		case 0x20: JRNZe(); break;
//		case 0x21: LDddnn(&gb_cpu.hl); break;
//		case 0x22: LDHLIA(); break;
//		case 0x23: INCss(&gb_cpu.hl); break;
//		case 0x24: INCr(&gb_cpu.h); break;
//		case 0x25: DEC(&gb_cpu.h); break;
//		case 0x26: LDrn(&gb_cpu.h); break;
//		case 0x27: DAA(); break;
//		case 0x28: JRZe(); break;
//		case 0x29: ADDHLss(gb_cpu.hl); break;
//		case 0x2A: LDAHLI(); break;
//		case 0x2B: DECHL(); break;
//		case 0x2C: INCr(&gb_cpu.l); break;
//		case 0x2D: DECr(&gb_cpu.l); break;
//		case 0x2E: LDrn(&gb_cpu.l); break;
//		case 0x2F: CPL(); break;
//		case 0x30:; break;
//		case 0x31:; break;
//		case 0x32:; break;
//		case 0x33:; break;
//		case 0x34:; break;
//		case 0x35:; break;
//		case 0x36:; break;
//		case 0x37:; break;
//		case 0x38:; break;
//		case 0x39:; break;
//		case 0x3A:; break;
//		case 0x3B:; break;
//		case 0x3C:; break;
//		case 0x3D:; break;
//		case 0x3E:; break;
//		case 0x3F:; break;
//		case 0x40:; break;
//		case 0x41:; break;
//		case 0x42:; break;
//		case 0x43:; break;
//		case 0x44:; break;
//		case 0x45:; break;
//		case 0x46:; break;
//		case 0x47:; break;
//		case 0x48:; break;
//		case 0x49:; break;
//		case 0x4A:; break;
//		case 0x4B:; break;
//		case 0x4C:; break;
//		case 0x4D:; break;
//		case 0x4E:; break;
//		case 0x4F:; break;
//		case 0x50:; break;
//		case 0x51:; break;
//		case 0x52:; break;
//		case 0x53:; break;
//		case 0x54:; break;
//		case 0x55:; break;
//		case 0x56:; break;
//		case 0x57:; break;
//		case 0x58:; break;
//		case 0x59:; break;
//		case 0x5A:; break;
//		case 0x5B:; break;
//		case 0x5C:; break;
//		case 0x5D:; break;
//		case 0x5E:; break;
//		case 0x5F:; break;
//		case 0x60:; break;
//		case 0x61:; break;
//		case 0x62:; break;
//		case 0x63:; break;
//		case 0x64:; break;
//		case 0x65:; break;
//		case 0x66:; break;
//		case 0x67:; break;
//		case 0x68:; break;
//		case 0x69:; break;
//		case 0x6A:; break;
//		case 0x6B:; break;
//		case 0x6C:; break;
//		case 0x6D:; break;
//		case 0x6E:; break;
//		case 0x6F:; break;
//		case 0x70:; break;
//		case 0x71:; break;
//		case 0x72:; break;
//		case 0x73:; break;
//		case 0x74:; break;
//		case 0x75:; break;
//		case 0x76:; break;
//		case 0x77:; break;
//		case 0x78:; break;
//		case 0x79:; break;
//		case 0x7A:; break;
//		case 0x7B:; break;
//		case 0x7C:; break;
//		case 0x7D:; break;
//		case 0x7E:; break;
//		case 0x7F:; break;
//		case 0x80:; break;
//		case 0x81:; break;
//		case 0x82:; break;
//		case 0x83:; break;
//		case 0x84:; break;
//		case 0x85:; break;
//		case 0x86:; break;
//		case 0x87:; break;
//		case 0x88:; break;
//		case 0x89:; break;
//		case 0x8A:; break;
//		case 0x8B:; break;
//		case 0x8C:; break;
//		case 0x8D:; break;
//		case 0x8E:; break;
//		case 0x90:; break;
//		case 0x91:; break;
//		case 0x92:; break;
//		case 0x93:; break;
//		case 0x94:; break;
//		case 0x95:; break;
//		case 0x96:; break;
//		case 0x97:; break;
//		case 0x98:; break;
//		case 0x99:; break;
//		case 0x9A:; break;
//		case 0x9B:; break;
//		case 0x9C:; break;
//		case 0x9D:; break;
//		case 0x9E:; break;
//		case 0x9F:; break;
//		case 0xA0:; break;
//		case 0xA1:; break;
//		case 0xA2:; break;
//		case 0xA3:; break;
//		case 0xA4:; break;
//		case 0xA5:; break;
//		case 0xA6:; break;
//		case 0xA7:; break;
//		case 0xA8:; break;
//		case 0xA9:; break;
//		case 0xAA:; break;
//		case 0xAB:; break;
//		case 0xAC:; break;
//		case 0xAD:; break;
//		case 0xAE:; break;
//		case 0xAF:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//		case 0x04:; break;
//
//
//	}
//
//}
