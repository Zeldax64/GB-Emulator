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
void LDrr(uint8_t *reg1, uint8_t *reg2) { *(reg1) = *(reg2); gb_cpu.pc++; gb_cpu.m++; }  // LD r, r'
void LDrn(uint8_t *reg) { *(reg) = rdByte(++gb_cpu.pc); gb_cpu.pc++; gb_cpu.m += 2; }	   // LD r,n
void LDrHL(uint8_t *reg) { *(reg) = rdByte(gb_cpu.hl); gb_cpu.pc++; gb_cpu.m += 2; }		   // LD r,(HL)
void LDHLr(uint8_t reg) { wrByte(gb_cpu.hl, reg); gb_cpu.pc++; gb_cpu.m += 2; }			   // LD (HL), r
void LDHLn() { wrByte(gb_cpu.hl, rdByte(++gb_cpu.pc)); gb_cpu.pc++; gb_cpu.m += 3; }		   // LD (HL), n
void LDABC() { gb_cpu.a = rdByte(gb_cpu.bc); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD A, (BC)
void LDADE() { gb_cpu.a = rdByte(gb_cpu.de); gb_cpu.pc++;	gb_cpu.m += 2; }				   // LD A, (DE)
void LDAC() { gb_cpu.a = rdByte(gb_cpu.c + 0xFF00); gb_cpu.pc++; gb_cpu.m += 2; }			   // LD A, (C)
void LDCA() { wrByte(gb_cpu.c + 0xFF00, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 2; }			   // LD (C), A
void LDAn() { gb_cpu.a = rdByte(++gb_cpu.pc); gb_cpu.pc++; gb_cpu.m += 3; }					   // LD A, (n)
void LDnA() { wrByte(++gb_cpu.pc, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 3; }					   // LD (n), A
void LDAnn() { gb_cpu.a = rdByte(rdWord(++gb_cpu.pc)); gb_cpu.pc += 2; gb_cpu.m += 4; }		   // LD A, (nn)
void LDnnA() { wrByte(rdWord(++gb_cpu.pc), gb_cpu.a); gb_cpu.pc += 2; gb_cpu.m += 4; }		   // LD (nn), A
void LDAHLI() { gb_cpu.a = rdByte(gb_cpu.hl); gb_cpu.hl++; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD A, (HLI)
void LDAHLD() { gb_cpu.a = rdByte(gb_cpu.hl); gb_cpu.hl--; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD A, (HLD)
void LDBCA() { wrByte(gb_cpu.bc, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD (BC), A
void LDDEA() { wrByte(gb_cpu.de, gb_cpu.a); gb_cpu.pc++; gb_cpu.m += 2; }					   // LD (DE), A
void LDHLIA() { wrByte(gb_cpu.hl, gb_cpu.a); gb_cpu.hl++; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD (HLI), A
void LDHLDA() { wrByte(gb_cpu.hl, gb_cpu.a); gb_cpu.hl--; gb_cpu.pc++; gb_cpu.m += 2; }		   // LD (HLD), A

/* 16-Bit */
void LDddnn(uint16_t *reg) { *(reg) = rdWord(gb_cpu.pc++); gb_cpu.pc += 2; gb_cpu.m += 3;}	  // LD dd, nn
void LDSPHL() { gb_cpu.sp = gb_cpu.hl; gb_cpu.pc++; gb_cpu.m += 2; }							  // LD SP, HL
void PUSHqq(uint16_t reg) { gb_cpu.sp -= 2; wrWord(gb_cpu.sp, reg); gb_cpu.pc++; gb_cpu.m += 4; } // PUSH qq
// NOTA
// Different : rdWord is used instead of rdByte.
void POPqq(uint16_t reg) { reg = rdWord(gb_cpu.sp); gb_cpu.sp += 2; gb_cpu.pc++; gb_cpu.m += 3; } // POP qq

void LDHLSPe() {  // LDHL SP, e
	int8_t e = (int8_t)rdByte(++gb_cpu.pc);
	gb_cpu.hl = gb_cpu.sp + e;
	CLEAR_FLAGS;
	flagC8((gb_cpu.sp >> 8), e);
	flagH8((gb_cpu.sp >> 8), e);
	gb_cpu.m += 3;
	gb_cpu.pc++;
}

void LDnnSP() {  // LD (nn), SP
	uint16_t destaddr = rdWord(++gb_cpu.pc);
	wrWord(destaddr, gb_cpu.sp);
	gb_cpu.m += 5;
	gb_cpu.pc += 2;
}

/*----- ALU Instruction -----*/
void ADDAr(uint8_t reg) { // ADD A, r
	CLEAR_FLAGS;
	flagC8(gb_cpu.a, reg);
	flagH8(gb_cpu.a, reg);
	gb_cpu.a += reg;
	flagZ8(gb_cpu.a);

	gb_cpu.m++;
	gb_cpu.pc++;
}

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

void ADDAHL() { // ADD A, (HL)
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

void ADCAHL() { // ADC A, (HL)
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

void SUBHL() { // SUB (HL)
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

void SBCAHL() {  // SBC A, (HL)
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

void ANDn() {  // AND n
	gb_cpu.a &= rdByte(++gb_cpu.pc);
	CLEAR_FLAGS;
	gb_cpu.f |= FLAGH;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ANDHL() {  // AND (HL)
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

void ORn() {  // OR n
	gb_cpu.a |= rdByte(++gb_cpu.pc);
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ORHL() {  // OR (HL)
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

void XORn() {  // XOR n
	gb_cpu.a ^= rdByte(++gb_cpu.pc);
	CLEAR_FLAGS;
	flagZ8(gb_cpu.a);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void XORHL() {  // XOR (HL)
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
void CPr(uint8_t reg) {
	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, reg);
	flagH8Sub(gb_cpu.a, reg);
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m++;
	gb_cpu.pc++;
}

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

void CPHL() {
	uint8_t n = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagC8Sub(gb_cpu.a, n);
	flagH8Sub(gb_cpu.a, n);
	flagZ8(gb_cpu.a);
	gb_cpu.f |= FLAGN;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void INCr(uint8_t *reg) {
	CLEAR_FLAGS;
	flagH8(*reg, 1);
	(*reg)++;
	flagZ8(*reg);

	gb_cpu.m++;
	gb_cpu.pc++;
}
/* I'll know which register increment '-'
void INCr(uint8_t reg) {
CLEAR_FLAGS;
flagH8(reg, 1);
(reg)++;
flagZ8(reg);
gb_cpu.m++;
gb_cpu.pc++;
}
*/

void INCHL() {
	uint8_t val = rdByte(gb_cpu.hl);

	CLEAR_FLAGS;
	flagH8(val, 1);
	val++;
	flagZ8(val);
	wrByte(gb_cpu.hl, val);

	gb_cpu.m += 3;
	gb_cpu.pc++;
}

void DECr(uint8_t *reg) {
	CLEAR_FLAGS;
	flagH8Sub(*reg, 1);
	(*reg)--;
	flagZ8(*reg);
	gb_cpu.f |= FLAGN;

	gb_cpu.m++;
	gb_cpu.pc++;
}

void DECHL() {
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
void ADDHLss(uint16_t reg) {  // AD HL, ss (register pair)
	CLEAR_N;
	flagC16(gb_cpu.hl, reg);
	flagH16(gb_cpu.hl, reg);
	gb_cpu.hl += reg;

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void ADDSPe() { // ADD SP, e
	uint8_t e = rdByte(++gb_cpu.pc);

	CLEAR_FLAGS;
	flagC16(gb_cpu.sp, e);
	flagH16(gb_cpu.sp, e);
	gb_cpu.sp += e;

	gb_cpu.m += 4;
	gb_cpu.pc++;
}

void INCss(uint16_t *reg) { // INC ss
	++(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

void DECss(uint16_t *reg) { // DEC ss
	--(*reg);

	gb_cpu.m += 2;
	gb_cpu.pc++;
}

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

void RLCHL() { // RLC (HL)
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

void RRCHL() { // RRCHL (HL)
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

void SLAHL() { // SLA HL
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

void SRLHL() { // SRL (HL)
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

void SWAPHL() { // SWAP (HL)
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

/*
NOTA
Useless
void JPccnn(uint8_t cc) { // JP cc, nn
	switch (cc) {
	case 0b00:	// NZ -> Flag Z = 0
		gb_cpu.pc = (gb_cpu.f & FLAGZ) ? gb_cpu.pc : rdWord(++gb_cpu.pc);
		gb_cpu.m += 4;
		break;

	case 0b01: // Z -> Flag Z = 1
		gb_cpu.pc = (gb_cpu.f & FLAGZ) ? rdWord(++gb_cpu.pc) : gb_cpu.pc;
		gb_cpu.m += 4;
		break;

	case 0b10: // NC -> Flag C = 0
		gb_cpu.pc = (gb_cpu.f & FLAGC) ? gb_cpu.pc : rdWord(++gb_cpu.pc);
		gb_cpu.m += 4;
		break;

	case 0b11: // C -> Flag C = 1
		gb_cpu.pc = (gb_cpu.f & FLAGC) ? rdWord(++gb_cpu.pc) : gb_cpu.pc;
		gb_cpu.m += 4;
		break;

	default:
		gb_cpu.pc += 3;
		gb_cpu.m += 3;
	}
}
*/

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

/*
NOTA
Not yet implemented instructions:
HALT
STOP
*/