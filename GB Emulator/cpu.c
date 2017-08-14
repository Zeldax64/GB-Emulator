#include <stdio.h>

#include "cpu.h"
#include "mmu.h"

/*----- Flag calculator functions -----*/
void flagC8(uint8_t val1, uint8_t val2) { // Flag C calculator for 8-Bit register
	if ((uint16_t)val1 + val2 & 0x100)
		cpu.f |= FLAGC;
}

void flagC16(uint16_t val1, uint16_t val2) { // Flag C calculator for 16-Bit register
	if ((uint32_t)val1 + val2 & 0x10000)
		cpu.f |= FLAGC;
}

void flagC8Sub(uint8_t val1, uint8_t val2) { // Flag C calculator for 8-Bit register
	if ((int16_t)val1 - val2 < 0)
		cpu.f |= FLAGC;
}

// NOTA
// DIFFERENT
void flagH8(uint8_t val1, uint8_t val2) { // Flag H calculator for 8-Bit register
	if (((val1 &= 0xF) + (val2 &= 0xF)) & 0x10)
		cpu.f |= FLAGH;
}

void flagH16(uint16_t val1, uint16_t val2) { // Flag H calculator for 16-Bit register
	if (((val1 &= 0xFFF) + (val2 &= 0xFFF)) & 0x1000)
		cpu.f |= FLAGH;
}

void flagH8Sub(uint8_t val1, uint8_t val2) { // Flag H calculator for 8-Bit register
	if (((val1 &= 0xF) - (val2 &= 0xF)) < 0)
		cpu.f |= FLAGH;
}

void flagZ8(uint8_t val) { // Flag Z calculator for 8-Bit register
	if (val == 0)
		cpu.f |= FLAGZ;
}

void flagZ16(uint16_t val) {
	if (val == 0)
		cpu.f |= FLAGZ;
}

/*----- Instructions -----*/

/*----- Load Instruction -----*/
/* 8-Bit */
void LDrr(uint8_t *reg1, uint8_t *reg2) { *(reg1) = *(reg2); cpu.pc++; cpu.m++; }		  // LD r, r'
void LDrn(uint8_t *reg) { *(reg) = rdByte(++cpu.pc); cpu.pc++; cpu.m += 2; }			  // LD r,n
void LDrHL(uint8_t *reg) { *(reg) = rdByte(cpu.hl); cpu.pc++; cpu.m += 2; }			  // LD r,(HL)
void LDHLr(uint8_t *reg) { wrByte(cpu.hl, *(reg)); cpu.pc++; cpu.m += 2; }				  // LD (HL), r
void LDHLn() { wrByte(cpu.hl, rdByte(++cpu.pc)); cpu.pc++; cpu.m += 3; }	  // LD (HL), n
void LDABC() { cpu.a = rdByte(cpu.bc); cpu.pc++; cpu.m += 2; }			  // LD A, (BC)
void LDADE() { cpu.a = rdByte(cpu.de); cpu.pc++;	cpu.m += 2; }			  // LD A, (DE)
void LDAC() { cpu.a = rdByte(cpu.c + 0xFF00); cpu.pc++; cpu.m += 2; }	  // LD A, (C)
void LDCA() { wrByte(cpu.c + 0xFF00, cpu.a); cpu.pc++; cpu.m += 2; }		  // LD (C), A
void LDAn() { cpu.a = rdByte(++cpu.pc); cpu.pc++; cpu.m += 3; }			  // LD A, (n)
void LDnA() { wrByte(++cpu.pc, cpu.a); cpu.pc++; cpu.m += 3; }			  // LD (n), A
void LDAnn() { cpu.a = rdByte(rdWord(++cpu.pc)); cpu.pc += 2; cpu.m += 4; } // LD A, (nn)
void LDnnA() { wrByte(rdWord(++cpu.pc), cpu.a); cpu.pc += 2; cpu.m += 4; }  // LD (nn), A
void LDAHLI() { cpu.a = rdByte(cpu.hl); cpu.hl++; cpu.pc++; cpu.m += 2; }	  // LD A, (HLI)
void LDAHLD() { cpu.a = rdByte(cpu.hl); cpu.hl--; cpu.pc++; cpu.m += 2; }	  // LD A, (HLD)
void LDBCA() { wrByte(cpu.bc, cpu.a); cpu.pc++; cpu.m += 2; }				  // LD (BC), A
void LDDEA() { wrByte(cpu.de, cpu.a); cpu.pc++; cpu.m += 2; }				  // LD (DE), A
void LDHLIA() { wrByte(cpu.hl, cpu.a); cpu.hl++; cpu.pc++; cpu.m += 2; }     // LD (HLI), A
void LDHLDA() { wrByte(cpu.hl, cpu.a); cpu.hl--; cpu.pc++; cpu.m += 2; }	  // LD (HLD), A

/*
	Stopped here!
*/
/* 16-Bit */
void LDddnn(uint16_t *reg) { *(reg) = rdWord(cpu.pc++); cpu.m += 3; cpu.pc += 2; }			// LD dd, nn
void LDSPHL() {cpu.sp = cpu.hl; cpu.m +=2 ; cpu.pc++					// LD SP, HL
void PUSHqq(uint16_t *reg1, uint16_t * reg2) { wrByte(cpu.sp--, reg1); wrByte(cpu.sp--, reg2); cpu.m += 4; cpu.pc++ // PUSH qq
// NOTA
// Different : rdWord is used instead of rdByte.
void POPqq(reg)			 reg = rdWord(cpu.sp); cpu.sp += 2; cpu.m += 3; cpu.pc++ // POP qq

void LDHLSPe() {  // LDHL SP, e
	int8_t e = (int8_t)rdByte(++cpu.pc);
	cpu.hl = cpu.sp + e;
	CLEAR_FLAGS;
	flagC8((cpu.sp >> 8), e);
	flagH8((cpu.sp >> 8), e);
	cpu.m += 3;
	cpu.pc++;
}

void LDnnSP() {  // LD (nn), SP
	uint16_t destaddr = rdWord(++cpu.pc);
	wrWord(destaddr, cpu.sp);
	cpu.m += 5;
	cpu.pc += 2;
}

/*----- ALU Instruction -----*/
void ADDAr(uint8_t reg) { // ADD A, r
	CLEAR_FLAGS;
	flagC8(cpu.a, reg);
	flagH8(cpu.a, reg);
	cpu.a += reg;
	flagZ8(cpu.a);

	cpu.m++;
	cpu.pc++;
}

void ADDAn() { // ADD A, n
	uint8_t n = rdByte(++cpu.pc);

	CLEAR_FLAGS;
	flagC8(cpu.a, n);
	flagH8(cpu.a, n);
	cpu.a += n;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

void ADDAHL() { // ADD A, (HL)
	uint8_t val = rdByte(cpu.hl);

	CLEAR_FLAGS;
	flagC8(cpu.a, val);
	flagH8(cpu.a, val);
	cpu.a += val;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

// ADC A,s functions. Each one represents a possibility to s
void ADCAr(uint8_t reg) { // ADC A, r
	uint8_t c = (cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	// Modified flagC8 with 3 variables (a, reg and Carry)
	if ((uint16_t)(cpu.a + reg + c) & 0x100)
		cpu.f |= FLAGC;
	// Modified flagH8 with 3 variables (a, reg and Carry)
	if (((cpu.a &= 0xF) + (reg &= 0xF) + c) & 0x10)
		cpu.f |= FLAGH;
	cpu.a += reg + c;
	flagZ8(cpu.a);

	cpu.m++;
	cpu.pc++;
}

void ADCAn() { // ADC A, n
	uint8_t n = rdByte(++cpu.pc);
	uint8_t c = (cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	// Modified flagC8 with 3 variables (a, reg and Carry)
	if ((uint16_t)(cpu.a + n + c) & 0x100)
		cpu.f |= FLAGC;
	// Modified flagH8 with 3 variables (a, reg and Carry)
	if (((cpu.a &= 0xF) + (n &= 0xF) + c) & 0x10)
		cpu.f |= FLAGH;
	// Modified flagZ8 with 3 variables (a, reg, and Carry)
	cpu.a += n + c;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

void ADCAHL() { // ADC A, (HL)
	uint8_t n = rdByte(cpu.hl);
	uint8_t c = (cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	// Modified flagC8 with 3 variables (a, reg and Carry)
	if ((uint16_t)(cpu.a + n + c) & 0x100)
		cpu.f |= FLAGC;
	// Modified flagH8 with 3 variables (a, reg and Carry)
	if (((cpu.a &= 0xF) + (n &= 0xF) + c) & 0x10)
		cpu.f |= FLAGH;
	// Modified flagZ8 with 3 variables (a, reg, and Carry)
	cpu.a += n + c;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

// SUB s functions. Each one represents a possibility to s
void SUBr(uint8_t reg) { // SUB r
	CLEAR_FLAGS;
	flagC8Sub(cpu.a, reg);
	flagH8Sub(cpu.a, reg);
	cpu.a -= reg;

	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m++;
	cpu.pc++;
}

void SUBn() { // SUB n
	uint8_t n = rdByte(++cpu.pc);

	CLEAR_FLAGS;
	flagC8Sub(cpu.a, n);
	flagH8Sub(cpu.a, n);
	cpu.a -= n;
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m += 2;
	cpu.pc++;
}

void SUBHL() { // SUB (HL)
	uint8_t n = rdByte(cpu.hl);

	CLEAR_FLAGS;
	flagC8Sub(cpu.a, n);
	flagH8Sub(cpu.a, n);
	cpu.a -= n;
	flagZ8(cpu.a);
	cpu.f |= FLAGN;
	cpu.m += 2;
	cpu.pc++;
}


// SBC a, s functions. Each one represents a possibility to s
void SBCAr(uint8_t reg) {  // SBC A, r
	uint8_t c = (cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	if ((int16_t)cpu.a - reg - c < 0)
		cpu.f |= FLAGC;
	if (((cpu.a &= 0xF) - (reg &= 0xF) - c) < 0)
		cpu.f |= FLAGH;
	cpu.a = cpu.a - reg - c;
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m++;
	cpu.pc++;
}

void SBCAn() {  // SBC A, n
	uint8_t n = rdByte(++cpu.pc);
	uint8_t c = (cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	if ((int16_t)cpu.a - n - c < 0)
		cpu.f |= FLAGC;
	if (((cpu.a &= 0xF) - (n &= 0xF) - c) < 0)
		cpu.f |= FLAGH;
	cpu.a = cpu.a - n - c;
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m += 2;
	cpu.pc++;
}

void SBCAHL() {  // SBC A, (HL)
	uint8_t n = rdByte(cpu.hl);
	uint8_t c = (cpu.f & FLAGC ? 1 : 0);

	CLEAR_FLAGS;
	if ((int16_t)cpu.a - n - c < 0)
		cpu.f |= FLAGC;
	if (((cpu.a &= 0xF) - (n &= 0xF) - c) < 0)
		cpu.f |= FLAGH;
	cpu.a = cpu.a - n - c;
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m += 2;
	cpu.pc++;
}

// AND s functions. Each one represents a possibility to s
void ANDr(uint8_t reg) {  // AND r
	cpu.a &= reg;
	CLEAR_FLAGS;
	cpu.f |= FLAGH;
	flagZ8(cpu.a);

	cpu.m++;
	cpu.pc++;
}

void ANDn() {  // AND n
	cpu.a &= rdByte(++cpu.pc);
	CLEAR_FLAGS;
	cpu.f |= FLAGH;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

void ANDHL() {  // AND (HL)
	cpu.a &= rdByte(cpu.hl);
	CLEAR_FLAGS;
	cpu.f |= FLAGH;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

// OR s functions. Each one represents a possibility to s
void ORr(uint8_t reg) {  // OR r
	cpu.a |= reg;
	CLEAR_FLAGS;
	flagZ8(cpu.a);

	cpu.m++;
	cpu.pc++;
}

void ORn() {  // OR n
	cpu.a |= rdByte(++cpu.pc);
	CLEAR_FLAGS;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

void ORHL() {  // OR (HL)
	cpu.a |= rdByte(cpu.hl);
	CLEAR_FLAGS;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

// XOR s functions. Each one represents a possibility to s
void XORr(uint8_t reg) {  // XOR r
	cpu.a ^= reg;
	CLEAR_FLAGS;
	flagZ8(cpu.a);

	cpu.m++;
	cpu.pc++;
}

void XORn() {  // XOR n
	cpu.a ^= rdByte(++cpu.pc);
	CLEAR_FLAGS;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

void XORHL() {  // XOR (HL)
	cpu.a ^= rdByte(cpu.hl);
	CLEAR_FLAGS;
	flagZ8(cpu.a);

	cpu.m += 2;
	cpu.pc++;
}

// CP s functions. Each one represents a possibility to s
// NOTA
// They are the same as SUBr, SUBn and SUBHL but the results
// are thrown away.
void CPr(uint8_t reg) {
	CLEAR_FLAGS;
	flagC8Sub(cpu.a, reg);
	flagH8Sub(cpu.a, reg);
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m++;
	cpu.pc++;
}

void CPn() {
	uint8_t n = rdByte(++cpu.pc);

	CLEAR_FLAGS;
	flagC8Sub(cpu.a, n);
	flagH8Sub(cpu.a, n);
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m += 2;
	cpu.pc++;
}

void CPHL() {
	uint8_t n = rdByte(cpu.hl);

	CLEAR_FLAGS;
	flagC8Sub(cpu.a, n);
	flagH8Sub(cpu.a, n);
	flagZ8(cpu.a);
	cpu.f |= FLAGN;

	cpu.m += 2;
	cpu.pc++;
}

void INCr(uint8_t *reg) {
	CLEAR_FLAGS;
	flagH8(*reg, 1);
	(*reg)++;
	flagZ8(*reg);

	cpu.m++;
	cpu.pc++;
}
/* I'll know which register increment '-'
void INCr(uint8_t reg) {
CLEAR_FLAGS;
flagH8(reg, 1);
(reg)++;
flagZ8(reg);
cpu.m++;
cpu.pc++;
}
*/

void INCHL() {
	uint8_t val = rdByte(cpu.hl);

	CLEAR_FLAGS;
	flagH8(val, 1);
	val++;
	flagZ8(val);
	wrByte(cpu.hl, val);

	cpu.m += 3;
	cpu.pc++;
}

void DECr(uint8_t *reg) {
	CLEAR_FLAGS;
	flagH8Sub(*reg, 1);
	(*reg)--;
	flagZ8(*reg);
	cpu.f |= FLAGN;

	cpu.m++;
	cpu.pc++;
}

void DECHL() {
	uint8_t val = rdByte(cpu.hl);

	CLEAR_FLAGS;
	flagH8Sub(val, 1);
	val--;
	flagZ8(val);
	cpu.f |= FLAGN;
	wrByte(cpu.hl, val);

	cpu.m += 3;
	cpu.pc++;
}

/*----- 16-Bit -----*/
void ADDHLss(uint16_t reg) {  // AD HL, ss (register pair)
	CLEAR_N;
	flagC16(cpu.hl, reg);
	flagH16(cpu.hl, reg);
	cpu.hl += reg;

	cpu.m += 2;
	cpu.pc++;
}

void ADDSPe() { // ADD SP, e
	uint8_t e = rdByte(++cpu.pc);

	CLEAR_FLAGS;
	flagC16(cpu.sp, e);
	flagH16(cpu.sp, e);
	cpu.sp += e;

	cpu.m += 4;
	cpu.pc++;
}

void INCss(uint16_t *reg) { // INC ss
	++(*reg);

	cpu.m += 2;
	cpu.pc++;
}

void DECss(uint16_t *reg) { // DEC ss
	--(*reg);

	cpu.m += 2;
	cpu.pc++;
}

/*----- Rotate Shift Instructions -----*/
void RLCA() {
	uint8_t bit = (cpu.a & 0x80) ? 1 : 0;

	CLEAR_FLAGS;
	if (bit) cpu.f |= FLAGC;
	cpu.a <<= 1;
	cpu.a |= bit;

	cpu.m++;
	cpu.pc++;
}

void RLA() {
	uint8_t bita = (cpu.a & 0x80) ? 1 : 0;
	uint8_t carry = (cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (bita) cpu.f |= FLAGC;
	cpu.a <<= 1;
	cpu.a |= carry;

	cpu.m++;
	cpu.pc++;
}

void RRCA() {
	uint8_t bit = (cpu.a & 0x1) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (bit) cpu.f |= FLAGC;
	cpu.a >>= 1;
	cpu.a |= bit;

	cpu.m++;
	cpu.pc++;
}

void RRA() {
	uint8_t bita = (cpu.a & 0x1) ? 1 : 0;
	uint8_t carry = (cpu.f & FLAGC) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (bita) cpu.f |= FLAGC;
	cpu.a >>= 1;
	cpu.a |= carry;

	cpu.m++;
	cpu.pc++;
}

// RLC m functions. Each one represents a possibility to m
void RLCr(uint8_t *reg) { // RLC r
	uint8_t bit = (*reg & 0x80) ? 1 : 0;

	CLEAR_FLAGS;
	if (bit) cpu.f |= FLAGC;
	*reg <<= 1;
	*reg |= bit;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void RLCHL() { // RLC (HL)
	uint8_t m = rdByte(cpu.hl);
	uint8_t bit = (m & 0x80) ? 1 : 0;

	CLEAR_FLAGS;
	if (bit) cpu.f |= FLAGC;
	m <<= 1;
	m |= bit;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// RL m functions. Each one represents a possibility to m
void RLr(uint8_t *reg) { // RL r
	uint8_t carry = (cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (*reg & 0x80) cpu.f |= FLAGC;
	*reg <<= 1;
	*reg |= carry;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void RLHL() { // RL (HL)
	uint8_t m = rdByte(cpu.hl);
	uint8_t bitr = (m & 0x80) ? 1 : 0;
	uint8_t carry = (cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (bitr) cpu.f |= FLAGC;
	m <<= 1;
	m |= carry;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// RRC m functions. Each one represents a possibility to m
void RRCr(uint8_t *reg) { // RRC r
	uint8_t bit = (*reg & 0x1) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (bit) cpu.f |= FLAGC;
	*reg >>= 1;
	*reg |= bit;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void RRCHL() { // RRCHL (HL)
	uint8_t m = rdByte(cpu.hl);
	uint8_t bitr = (m & 0x80) ? 0x80 : 0;
	uint8_t carry = (cpu.f & FLAGC) ? 1 : 0;

	CLEAR_FLAGS;
	if (bitr) cpu.f |= FLAGC;
	m >>= 1;
	m |= carry;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// RR m functions. Each one represents a possibility to m
void RRr(uint8_t *reg) { // RR r
	uint8_t carry = (cpu.f & FLAGC) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (*reg & 0x1) cpu.f |= FLAGC;
	*reg >>= 1;
	*reg |= carry;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void RRHL() { // RR (HL)
	uint8_t m = rdByte(cpu.hl);
	uint8_t carry = (cpu.f & FLAGC) ? 0x80 : 0;

	CLEAR_FLAGS;
	if (m & 0x1) cpu.f |= FLAGC;
	m >>= 1;
	m |= carry;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// SLA m functions. Each one represents a possibility to m
void SLAr(uint8_t *reg) { // SLA r
	CLEAR_FLAGS;
	cpu.f = (*reg & 0x80) ? FLAGC : 0;
	*reg <<= 1;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void SLAHL() { // SLA HL
	uint8_t m = rdByte(cpu.hl);

	CLEAR_FLAGS;
	cpu.f = (m & 0x80) ? FLAGC : 0;
	m <<= 1;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// SRA m functions. Each one represents a possibility to m
void SRAr(uint8_t *reg) { // SRA r
	uint8_t bitr = *reg & 0x80;

	CLEAR_FLAGS;
	cpu.f = (*reg & 0x1) ? FLAGC : 0;
	*reg >>= 1;
	*reg |= bitr;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void SRAHL() { // SRA (HL)
	uint8_t m = rdByte(cpu.hl);
	uint8_t bitr = m & 0x80;

	CLEAR_FLAGS;
	cpu.f = (m & 0x1) ? FLAGC : 0;
	m >>= 1;
	m |= bitr;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// SRL m functions. Each one represents a possibility to m
void SRLr(uint8_t *reg) { // SRL r
	CLEAR_FLAGS;
	cpu.f = (*reg & 0x1) ? FLAGC : 0;
	*reg >>= 1;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void SRLHL() { // SRL (HL)
	uint8_t m = rdByte(cpu.hl);

	CLEAR_FLAGS;
	cpu.f = (m & 0x1) ? FLAGC : 0;
	m >>= 1;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

// SWAP m functions. Each one represents a possibility to m
void SWAPr(uint8_t *reg) { // SWAP r
	*reg = ((*reg & 0xF0) >> 4) | ((*reg & 0xF) << 4);
	CLEAR_FLAGS;
	flagZ8(*reg);

	cpu.m += 2;
	cpu.pc += 2;
}

void SWAPHL() { // SWAP (HL)
	uint8_t m = rdByte(cpu.hl);

	m = ((m & 0xF0) >> 4) | ((m & 0xF) << 4);
	CLEAR_FLAGS;
	flagZ8(m);
	wrByte(cpu.hl, m);

	cpu.m += 4;
	cpu.pc += 2;
}

/*---- Bit Operations -----*/
void BITr(uint8_t bit, uint8_t reg) { // Bit b, r
	CLEAR_N;
	cpu.f = (bit & reg) ? 0 : FLAGZ;
	cpu.f |= FLAGH;

	cpu.m += 2;
	cpu.pc += 2;
}

void BITHL(uint8_t bit) { // BIT b, (HL)
	CLEAR_N;
	cpu.f = (bit & rdByte(cpu.hl)) ? 0 : FLAGZ;
	cpu.f |= FLAGH;

	cpu.m += 3;
	cpu.pc += 2;
}

void SETr(uint8_t bit, uint8_t *reg) { // SET b, r
	*reg |= bit;

	cpu.m += 2;
	cpu.pc += 2;
}

void SETHL(uint8_t bit) { // SET (HL)
	wrByte(cpu.hl, rdByte(cpu.hl) | bit);

	cpu.m += 4;
	cpu.pc += 2;
}

void RESr(uint8_t bit, uint8_t *reg) {
	*reg &= !bit;

	cpu.m += 2;
	cpu.pc += 2;
}

void RESHL(uint8_t bit) {
	wrByte(cpu.hl, rdByte(cpu.hl) & !bit);

	cpu.m += 4;
	cpu.pc += 2;
}

/*----- Jump Instructions -----*/
void JPnn() { // JP nn
	cpu.pc = rdWord(++cpu.pc);

	cpu.m += 4;
}

/*
NOTA
Useless
void JPccnn(uint8_t cc) { // JP cc, nn
	switch (cc) {
	case 0b00:	// NZ -> Flag Z = 0
		cpu.pc = (cpu.f & FLAGZ) ? cpu.pc : rdWord(++cpu.pc);
		cpu.m += 4;
		break;

	case 0b01: // Z -> Flag Z = 1
		cpu.pc = (cpu.f & FLAGZ) ? rdWord(++cpu.pc) : cpu.pc;
		cpu.m += 4;
		break;

	case 0b10: // NC -> Flag C = 0
		cpu.pc = (cpu.f & FLAGC) ? cpu.pc : rdWord(++cpu.pc);
		cpu.m += 4;
		break;

	case 0b11: // C -> Flag C = 1
		cpu.pc = (cpu.f & FLAGC) ? rdWord(++cpu.pc) : cpu.pc;
		cpu.m += 4;
		break;

	default:
		cpu.pc += 3;
		cpu.m += 3;
	}
}
*/

// JPccnn functions. Each one represents a possibility to cc
void JPNZnn() {
	if (cpu.f & FLAGZ) {
		cpu.pc += 3;
		cpu.m += 3;
	}
	else {
		cpu.pc = rdWord(cpu.pc + 1);
		cpu.m += 4;
	}
}

void JPZnn() {
	if (cpu.f & FLAGZ) {
		cpu.pc = rdWord(cpu.pc + 1);
		cpu.m += 4;
	}
	else {
		cpu.pc += 3;
		cpu.m += 3;
	}
}

void JPNCnn() {
	if (cpu.f & FLAGC) {
		cpu.pc += 3;
		cpu.m += 3;
	}
	else {
		cpu.pc = rdWord(cpu.pc + 1);
		cpu.m += 4;
	}
}

void JPCnn() {
	if (cpu.f & FLAGC) {
		cpu.pc = rdWord(cpu.pc + 1);
		cpu.m += 4;
	}
	else {
		cpu.pc += 3;
		cpu.m += 3;
	}
}


/*
NOTA
According to GB Programming Manual, the e parameter is different from the others. Now it can be from -127 to +129.
This function must be watched!
*/

void JRe() {
	int8_t e = rdByte(++cpu.pc);

	cpu.pc += e;

	cpu.m += 3;
}

/*
void JRcce(uint8_t cc) { // JR cc, e
	uint8_t e = rdByte(++cpu.pc);
	switch (cc) {
	case 0b00:	// NZ -> Flag Z = 0
		cpu.pc = (cpu.f & FLAGZ) ? cpu.pc : rdByte(++cpu.pc) + e;
		cpu.m += 3;
		break;

	case 0b01: // Z -> Flag Z = 1
		cpu.pc = (cpu.f & FLAGZ) ? rdWord(++cpu.pc) + e : cpu.pc;
		cpu.m += 3;
		break;

	case 0b10: // NC -> Flag C = 0
		cpu.pc = (cpu.f & FLAGC) ? cpu.pc : rdWord(++cpu.pc) + e;
		cpu.m += 3;
		break;

	case 0b11: // C -> Flag C = 1
		cpu.pc = (cpu.f & FLAGC) ? rdWord(++cpu.pc) + e : cpu.pc;
		cpu.m += 3;
		break;

	default:
		cpu.pc += 2;
		cpu.m += 2;
	}
}
*/

// JRcce functions. Each one represents a possibility to cc
void JRNZe() {
	if (cpu.f & FLAGZ) {
		cpu.pc += 2;
		cpu.m += 2;
	}
	else {
		cpu.pc += (int8_t) rdByte(cpu.pc + 1);
		cpu.m += 3;
	}
}

void JRZe() {
	if (cpu.f & FLAGZ) {
		cpu.pc += (int8_t)rdByte(cpu.pc + 1);
		cpu.m += 3;
	}
	else {
		cpu.pc += 2;
		cpu.m += 2;
	}
}

void JRNCe() {
	if (cpu.f & FLAGC) {
		cpu.pc += 2;
		cpu.m += 2;
	}
	else {
		cpu.pc += (int8_t)rdWord(cpu.pc + 1);
		cpu.m += 3;
	}
}
	

void JRCe() {
	if (cpu.f & FLAGC) {
		cpu.pc += (int8_t)rdWord(cpu.pc + 1);
		cpu.m += 3;
	}
	else {
		cpu.pc += 2;
		cpu.m += 2;
	}
}

void JPHL() { // JP (HL)
	cpu.pc = cpu.hl;
	cpu.m += 1;
}

/*----- Call and Return Instructions -----*/
void CALLnn() {
	cpu.sp -= 2;
	wrWord(cpu.sp, cpu.pc + 3);
	cpu.pc = rdWord(cpu.pc + 1);

	cpu.m += 6;
}

// CALLccnn functions. Each one represents a possibility to cc
void CALLNZnn() {
	if (cpu.f & FLAGZ) {
		cpu.pc += 3;
		cpu.m += 3;
	}
	else {
		cpu.sp -= 2;
		wrWord(cpu.sp, cpu.pc + 3);
		cpu.pc = rdWord(cpu.pc + 1);

		cpu.m += 6;
	}
}

void CALLZnn() {
	if (cpu.f & FLAGZ) {
		cpu.sp -= 2;
		wrWord(cpu.sp, cpu.pc + 3);
		cpu.pc = rdWord(cpu.pc + 1);

		cpu.m += 6;
	}
	else {
		cpu.pc += 3;
		cpu.m += 3;
	}
}

void CALLNCnn() {
	if (cpu.f & FLAGC) {
		cpu.pc += 3;
		cpu.m += 3;
	}
	else {
		cpu.sp -= 2;
		wrWord(cpu.sp, cpu.pc + 3);
		cpu.pc = rdWord(cpu.pc + 1);

		cpu.m += 6;
	}
}

void CALLCnn() {
	if (cpu.f & FLAGC) {
		cpu.sp -= 2;
		wrWord(cpu.sp, cpu.pc + 3);
		cpu.pc = rdWord(cpu.pc + 1);

		cpu.m += 6;
	}
	else {
		cpu.pc += 3;
		cpu.m += 3;
	}
}

void RET() {
	cpu.pc = rdWord(cpu.sp);
	cpu.sp += 2;

	cpu.m += 4;
}

/*
	NOTA
	This function works with interruptions which are
	not yet implemented.
*/
void RETI() {

}

// RETcc functions. Each one represents a possibility to cc
void RETNZ() {
	if (cpu.f & FLAGZ) {
		cpu.pc++;
		cpu.m += 2;
	}
	else {
		cpu.pc = rdWord(cpu.sp);
		cpu.m += 5;
	}
}

void RETZ() {
	if (cpu.f & FLAGZ) {
		cpu.pc = rdWord(cpu.sp);
		cpu.m += 5;
	}
	else {
		cpu.pc++;
		cpu.m += 2;
	}
}

void RETNC() {
	if (cpu.f & FLAGC) {
		cpu.pc++;
		cpu.m += 2;
	}
	else {
		cpu.pc = rdWord(cpu.sp);
		cpu.m += 5;
	}
}

void RETC() {
	if (cpu.f & FLAGC) {
		cpu.pc = rdWord(cpu.sp);
		cpu.m += 5;
	}
	else {
		cpu.pc++;
		cpu.m += 2;
	}
}

void RST(uint8_t t) {
	cpu.sp -= 2;
	wrWord(cpu.sp, cpu.pc + 1);

	cpu.pc = t;
	cpu.m += 4;
}

/*----- General-Purpose Arithmetic Operations and CPU Control Instructions -----*/
/*
	NOTA
	Different implementation!
	Source: https://stackoverflow.com/questions/8119577/z80-daa-instruction
	Worth viewing: http://forums.nesdev.com/viewtopic.php?t=9088
*/
void DAA() {
	uint8_t a = cpu.a;
	CLEAR_H;

	if (cpu.f & FLAGC) {
		if (cpu.f & FLAGH || (a & 0x0F) > 0x9) {
			cpu.a += 6;
		}
		if (cpu.f & FLAGC || (a & 0xF0) > 0x90) {
			cpu.a += 60;
			cpu.f &= FLAGC;
		}
	}
	else {
		if (cpu.f & FLAGH || (a & 0x0F) > 0x9) {
			cpu.a -= 6;
		}
		if (cpu.f & FLAGC || (a & 0xF0) > 0x90) {
			cpu.a -= 60;
			cpu.f &= FLAGC;
		}
	}
	if (cpu.a == 0) cpu.f &= FLAGZ;

	cpu.pc++;
	cpu.m++;
}

void CPL() {
	cpu.f &= FLAGH;
	cpu.f &= FLAGN;
	cpu.a = ~cpu.a;

	cpu.pc++;
	cpu.m++;
}

void NOP() {
	cpu.pc++;
	cpu.m++;
}

void CCF() {
	cpu.f ^= FLAGC;
	CLEAR_H;
	CLEAR_N;

	cpu.pc++;
	cpu.m++;
}

void SCF() {
	cpu.f &= FLAGC;
	CLEAR_H;
	CLEAR_N;

	cpu.pc++;
	cpu.m++;
}

void DI() {

}

void EI() {

}

void HALT() {

}

/*
NOTA
Not yet implemented instructions:
DI
EI
HALT
STOP
RETI
*/