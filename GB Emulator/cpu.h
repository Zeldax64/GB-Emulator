#ifndef _CPU
#define _CPU

/*---- Includes -----*/
#include <stdint.h>

/*----- Defines -----*/
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define FLAGZ 0x80  // Zero flag
#define FLAGN 0x40  // Subtraction flag
#define FLAGH 0x20  // Half-carry flag
#define FLAGC 0x10  // Carry flag

#define CLEAR_Z cpu.f &= !FLAGZ // Clear flag Z
#define CLEAR_N cpu.f &= !FLAGN // Clear flag N
#define CLEAR_H cpu.f &= !FLAGH // Clear flag H
#define CLEAR_C cpu.f &= !FLAGC // Clear flag C

#define CLEAR_FLAGS cpu.f = 0

/*----- CPU Init function -----*/
void CPU_init();

/*----- Flag calculator functions -----*/
void flagC8(uint8_t val1, uint8_t val2);
void flagC16(uint16_t val1, uint16_t val2);
void flagC8Sub(uint8_t val1, uint8_t val2);
void flagH8(uint8_t val1, uint8_t val2);
void flagH16(uint16_t val1, uint16_t val2);
void flagH8Sub(uint8_t val1, uint8_t val2);
void flagZ8(uint8_t val);
void flagZ16(uint16_t val);

/*----- CPU Registers ------*/
struct cpuregisters {
	struct {
		union {
			struct {
				uint8_t a;
				uint8_t f;
			};
			uint16_t af;
		};
	};

	struct {
		union {
			struct {
				uint8_t b;
				uint8_t c;
			};
			uint16_t bc;
		};
	};

	struct {
		union {
			struct {
				uint8_t d;
				uint8_t e;
			};
			uint16_t de;
		};
	};

	struct {
		union {
			struct {
				uint8_t h;
				uint8_t l;
			};
			uint16_t hl;
		};
	};

	uint16_t sp;
	uint16_t pc;

	int32_t m; // Machine cycle
};
struct cpuregisters cpu;

////////////////////////////
/*----- Instructions -----*/
////////////////////////////



/*----- Load Instructions -----*/
/* 8-Bit */
void LDrr(uint8_t * reg1, uint8_t * reg2);
void LDrn(uint8_t * reg);
void LDrHL(uint8_t *reg);
void LDHLr(uint8_t reg);
void LDHLn();
void LDABC();
void LDADE();
void LDAC();
void LDCA();
void LDAn();
void LDnA();
void LDAnn();
void LDnnA();
void LDAHLI();
void LDAHLD();
void LDBCA();
void LDDEA();
void LDHLIA();
void LDHLDA();

/* 16-Bit */
void LDddnn(uint16_t *reg);
void LDSPHL();
void PUSHqq(uint16_t reg);
// NOTA
// Different : rdWord is used instead of rdByte.
void POPqq(uint16_t reg);
// NOTA
// Different
void LDHLSPe();
void LDnnSP();

/*----- ALU Instructions -----*/
/* 8-Bit */
void ADDAr(uint8_t reg);
void ADDAn();
void ADDAHL();
// ADC A,s functions. Each one represents a possibility to s
void ADCAr(uint8_t reg);
void ADCAn();
void ADCAHL();
// SUB s functions. Each one represents a possibility to s
void SUBr(uint8_t reg);
void SUBn();
void SUBHL();
// SBC a, s functions. Each one represents a possibility to s
void SBCAr(uint8_t reg);
void SBCAn();
void SBCAHL();
// AND s functions. Each one represents a possibility to s
void ANDr(uint8_t reg);
void ANDn();
void ANDHL();
// OR s functions. Each one represents a possibility to s
void ORr(uint8_t reg);
void ORn();
void ORHL();
// XOR s functions. Each one represents a possibility to s
void XORr(uint8_t reg);
void XORn();
void XORHL();
void CPr(uint8_t reg);
void CPn();
void CPHL();
void INCr(uint8_t *reg);
void INCHL();
void DECr(uint8_t *reg);
void DECHL();

/* 16-Bit */
void ADDHLss(uint16_t reg);
void ADDSPe();
void INCss(uint16_t *reg);
void DECss(uint16_t *reg);


/*----- Rotate Shift Instructions -----*/
/*----- Function Prototypes -----*/
void RLCA();
void RLA();
void RRCA();
void RRA();
void RLCr(uint8_t *reg);
void RLCHL();
void RLr(uint8_t *reg);
void RLHL();
void RRCr(uint8_t *reg);
void RRCHL();
void RRr(uint8_t *reg);
void RRHL();
void SLAr(uint8_t *reg);
void SLAHL();
void SRAr(uint8_t *reg);
void SRAHL();
void SRLr(uint8_t *reg);
void SRLHL();
void SWAPr(uint8_t *reg);
void SWAPHL();

/*---- Bit Operations -----*/
void BITr(uint8_t bit, uint8_t reg);
void BITHL(uint8_t bit);
void SETr(uint8_t bit, uint8_t *reg);
void SETHL(uint8_t bit);
void RESr(uint8_t bit, uint8_t *reg);
void RESHL(uint8_t bit);

/*----- Jump Instructions -----*/
void JPnn();
// void JPccnn(uint8_t cc);
void JPNZnn();
void JPZnn();
void JPNCnn();
void JPCnn();
void JRe();
//void JRcce(uint8_t cc);
/*
  Nota
  Watch all JRcce functions because e can be from -127 to 129.
*/
void JRNZe();
void JRZe();
void JRNCe();
void JRCe();
void JPHL();

/*----- Call and Return Instructions -----*/
void CALLnn();
void CALLNZnn();
void CALLZnn();
void CALLNCnn();
void CALLCnn();
void RET();
void RETI();
void RETNZ();
void RETZ();
void RETNC();
void RETC();
void RST(uint8_t t);
void CPL();
void NOP();
void HALT();

/*----- General-Purpose Arithmetic Operations and CPU Control Instructions -----*/

#endif