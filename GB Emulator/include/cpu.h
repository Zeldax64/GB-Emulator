#ifndef _CPU
#define _CPU

/*---- Includes -----*/
#include <stdint.h>
#include <stdbool.h>

#include "bit.h"
#include "mmu.h"

/*----- Defines -----*/
#define FLAGZ BIT7  // Zero flag
#define FLAGN BIT6  // Subtraction flag
#define FLAGH BIT5  // Half-carry flag
#define FLAGC BIT4  // Carry flag

#define CLEAR_Z gb_cpu.f &= ~FLAGZ // Clear flag Z
#define CLEAR_N gb_cpu.f &= ~FLAGN // Clear flag N
#define CLEAR_H gb_cpu.f &= ~FLAGH // Clear flag H
#define CLEAR_C gb_cpu.f &= ~FLAGC // Clear flag C

#define CLEAR_FLAGS gb_cpu.f = 0

/*----- CPU functions -----*/
void CPU_init(void);
void CPU_cycle(void);

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
typedef struct cpuregisters {
	struct {
		union {
			struct {
				uint8_t f;
				uint8_t a;
			};
			uint16_t af;
		};
	};

	struct {
		union {
			struct {
				uint8_t c;
				uint8_t b;
			};
			uint16_t bc;
		};
	};

	struct {
		union {
			struct {
				uint8_t e;
				uint8_t d;
			};
			uint16_t de;
		};
	};

	struct {
		union {
			struct {
				uint8_t l;
				uint8_t h;
			};
			uint16_t hl;
		};
	};

	bool ime; // interrupt Master Enabled
	uint16_t sp; // Stack Pointer
	uint16_t pc; // Program Counter

	int32_t m; // Machine cycle
}GB_CPU;

GB_CPU gb_cpu;

/*----- Const table -----*/


////////////////////////////
/*----- Instructions -----*/
////////////////////////////

/*----- Load Instructions -----*/


/* 8-Bit */

// LDrr instructions expansion to all possibilities of r.
void LDrr(uint8_t * reg1, uint8_t * reg2);

// LDAr
void LDaa();
void LDab();
void LDac();
void LDad();
void LDae();
void LDah();
void LDal();

// LDbr
void LDba();
void LDbb();
void LDbc();
void LDbd();
void LDbe();
void LDbh();
void LDbl();

// LDcr
void LDca();
void LDcb();
void LDcc();
void LDcd();
void LDce();
void LDch();
void LDcl();

// LDdr
void LDda();
void LDdb();
void LDdc();
void LDdd();
void LDde();
void LDdh();
void LDdl();

// LDer
void LDea();
void LDeb();
void LDec();
void LDed();
void LDee();
void LDeh();
void LDel();

// LDhr
void LDha();
void LDhb();
void LDhc();
void LDhd();
void LDhe();
void LDhh();
void LDhl();

// LDlr
void LDla();
void LDlb();
void LDlc();
void LDld();
void LDle();
void LDlh();
void LDll();

// End

// LDrn instructions expansion to all possibilities of r.
void LDrn(uint8_t * reg);

void LDan(uint8_t * reg);
void LDbn(uint8_t * reg);
void LDcn(uint8_t * reg);
void LDdn(uint8_t * reg);
void LDen(uint8_t * reg);
void LDhn(uint8_t * reg);
void LDln(uint8_t * reg);

// End

// LDr(HL) instructions expansion to all possibilities of r.
void LDrHLm(uint8_t *reg);

void LDaHLm(uint8_t *reg);
void LDbHLm(uint8_t *reg);
void LDcHLm(uint8_t *reg);
void LDdHLm(uint8_t *reg);
void LDeHLm(uint8_t *reg);
void LDhHLm(uint8_t *reg);
void LDlHLm(uint8_t *reg);

// End

// LD(HL)r instructions expansion to all possibilities of r.
void LDHLmr(uint8_t reg);

void LDHLma(uint8_t reg);
void LDHLmb(uint8_t reg);
void LDHLmc(uint8_t reg);
void LDHLmd(uint8_t reg);
void LDHLme(uint8_t reg);
void LDHLmh(uint8_t reg);
void LDHLml(uint8_t reg);

// End

void LDHLmn();
void LDABCm();
void LDADEm();
void LDACm();
void LDCmA();
void LDAnm();
void LDnmA();
void LDAnnm();
void LDnnmA();
void LDAHLIm();
void LDAHLDm();
void LDBCmA();
void LDDEmA();
void LDHLImA();
void LDHLDmA();

// End

/* 16-Bit */

// LDddnn instructions expansion to all possibilities of dd.
void LDddnn(uint16_t *reg);

void LDBCnn(uint16_t *reg);
void LDDEnn(uint16_t *reg);
void LDHLnn(uint16_t *reg);
void LDSPnn(uint16_t *reg);

// End

void LDSPHL();

// PUSHqq instructions expansion to all possibilities of qq.
void PUSHqq(uint16_t reg);

void PUSHBC(uint16_t reg);
void PUSHDE(uint16_t reg);
void PUSHHL(uint16_t reg);
void PUSHAF(uint16_t reg);

// End

// POPqq instructions expansion to all possibilities of qq.
// NOTA
// Different : rdWord is used instead of rdByte.
void POPqq(uint16_t *reg);

void POPBC(uint16_t *reg);
void POPDE(uint16_t *reg);
void POPHL(uint16_t *reg);
void POPAF(uint16_t *reg);

// End

// NOTA
// Different
void LDHLSPe();
void LDnnmSP();

////////////////////////////////
/*----- ALU Instructions -----*/
////////////////////////////////

/* 8-Bit */

// ADDAr instructions expansion to all possibilities of r.
void ADDAr(uint8_t reg);

void ADDAa(uint8_t reg);
void ADDAb(uint8_t reg);
void ADDAc(uint8_t reg);
void ADDAd(uint8_t reg);
void ADDAe(uint8_t reg);
void ADDAh(uint8_t reg);
void ADDAl(uint8_t reg);

// End

void ADDAn();
void ADDAHLm();

// ADC A,s functions. Each one represents a possibility to s
// ADDAr instructions expansion to all possibilities of r.
void ADCAr(uint8_t reg);

void ADCAa(uint8_t reg);
void ADCAb(uint8_t reg);
void ADCAc(uint8_t reg);
void ADCAd(uint8_t reg);
void ADCAe(uint8_t reg);
void ADCAh(uint8_t reg);
void ADCAl(uint8_t reg);

// End

void ADCAn();
void ADCAHLm();

// SUB s functions. Each one represents a possibility to s
// SUBr instructions expansion to all possibilities of r.
void SUBr(uint8_t reg);

void SUBa(uint8_t reg);
void SUBb(uint8_t reg);
void SUBc(uint8_t reg);
void SUBd(uint8_t reg);
void SUBe(uint8_t reg);
void SUBh(uint8_t reg);
void SUBl(uint8_t reg);

// End

void SUBn();
void SUBHLm();

// SBC a, s functions. Each one represents a possibility to s
// SBCr instructions expansion to all possibilities of r.
void SBCAr(uint8_t reg);

void SBCAa(uint8_t reg);
void SBCAb(uint8_t reg);
void SBCAc(uint8_t reg);
void SBCAd(uint8_t reg);
void SBCAe(uint8_t reg);
void SBCAh(uint8_t reg);
void SBCAl(uint8_t reg);

// End

void SBCAn();
void SBCAHLm();

// AND s functions. Each one represents a possibility to s
// ANDr instructions expansion to all possibilities of r.
void ANDr(uint8_t reg);

void ANDa(uint8_t reg);
void ANDb(uint8_t reg);
void ANDc(uint8_t reg);
void ANDd(uint8_t reg);
void ANDe(uint8_t reg);
void ANDh(uint8_t reg);
void ANDl(uint8_t reg);

// End

void ANDn();
void ANDHLm();

// OR s functions. Each one represents a possibility to s
// ORr instructions expansion to all possibilities of r.
void ORr(uint8_t reg);

void ORa(uint8_t reg);
void ORb(uint8_t reg);
void ORc(uint8_t reg);
void ORd(uint8_t reg);
void ORe(uint8_t reg);
void ORh(uint8_t reg);
void ORl(uint8_t reg);

// End

void ORn();
void ORHLm();

// XOR s functions. Each one represents a possibility to s
// XORr instructions expansion to all possibilities of r.
void XORr(uint8_t reg);

void XORa(uint8_t reg);
void XORb(uint8_t reg);
void XORc(uint8_t reg);
void XORd(uint8_t reg);
void XORe(uint8_t reg);
void XORh(uint8_t reg);
void XORl(uint8_t reg);

// End

void XORn();
void XORHLm();

// CP s functions. Each one represents a possibility to s
// CPr instructions expansion to all possibilities of r.
void CPr(uint8_t reg);

void CPa(uint8_t reg);
void CPb(uint8_t reg);
void CPc(uint8_t reg);
void CPd(uint8_t reg);
void CPe(uint8_t reg);
void CPh(uint8_t reg);
void CPl(uint8_t reg);

// End

void CPn();
void CPHLm();

// INCr instructions expansion to all possibilities of r.
void INCr(uint8_t *reg);

void INCa(uint8_t *reg);
void INCb(uint8_t *reg);
void INCc(uint8_t *reg);
void INCd(uint8_t *reg);
void INCe(uint8_t *reg);
void INCh(uint8_t *reg);
void INCl(uint8_t *reg);

// End

void INCHLm();

// DECr instructions expansion to all possibilities of r.
void DECr(uint8_t *reg);

void DECa(uint8_t *reg);
void DECb(uint8_t *reg);
void DECc(uint8_t *reg);
void DECd(uint8_t *reg);
void DECe(uint8_t *reg);
void DECh(uint8_t *reg);
void DECl(uint8_t *reg);

// END

void DECHLm();

/* 16-Bit */
// ADDHLss instructions expansion to all possibilities of ss.
void ADDHLss(uint16_t reg);

void ADDHLBC(uint16_t reg);
void ADDHLDE(uint16_t reg);
void ADDHLHL(uint16_t reg);
void ADDHLSP(uint16_t reg);

// End

void ADDSPe();

// INCss instructions expansion to all possibilities of ss.
void INCss(uint16_t *reg);

void INCBC(uint16_t *reg);
void INCDE(uint16_t *reg);
void INCHL(uint16_t *reg);
void INCSP(uint16_t *reg);

// End

// DECss instructions expansion to all possibilities of ss.
void DECss(uint16_t *reg);

void DECBC(uint16_t *reg);
void DECDE(uint16_t *reg);
void DECHL(uint16_t *reg);
void DECSP(uint16_t *reg);

// End

/*----- Rotate Shift Instructions -----*/
/*----- Function Prototypes -----*/
void RLCA();
void RLA();
void RRCA();
void RRA();

// RLCr instructions expansion to all possibilities of r.
void RLCr(uint8_t *reg);

void RLCa(uint8_t *reg);
void RLCb(uint8_t *reg);
void RLCc(uint8_t *reg);
void RLCd(uint8_t *reg);
void RLCe(uint8_t *reg);
void RLCh(uint8_t *reg);
void RLCl(uint8_t *reg);

// End

void RLCHLm();

// RLr instructions expansion to all possibilities of r.
void RLr(uint8_t *reg);

void RLa(uint8_t *reg);
void RLb(uint8_t *reg);
void RLc(uint8_t *reg);
void RLd(uint8_t *reg);
void RLe(uint8_t *reg);
void RLh(uint8_t *reg);
void RLl(uint8_t *reg);

// End

void RLHLm();

// RRCr instructions expansion to all possibilities of r.
void RRCr(uint8_t *reg);

void RRCa(uint8_t *reg);
void RRCb(uint8_t *reg);
void RRCc(uint8_t *reg);
void RRCd(uint8_t *reg);
void RRCe(uint8_t *reg);
void RRCh(uint8_t *reg);
void RRCl(uint8_t *reg);

// End

void RRCHLm();

// RRr instructions expansion to all possibilities of r.
void RRr(uint8_t *reg);

void RRa(uint8_t *reg);
void RRb(uint8_t *reg);
void RRc(uint8_t *reg);
void RRd(uint8_t *reg);
void RRe(uint8_t *reg);
void RRh(uint8_t *reg);
void RRl(uint8_t *reg);

// End

void RRHLm();

// SLAr instructions expansion to all possibilities of r.
void SLAr(uint8_t *reg);

void SLAa(uint8_t *reg);
void SLAb(uint8_t *reg);
void SLAc(uint8_t *reg);
void SLAd(uint8_t *reg);
void SLAe(uint8_t *reg);
void SLAh(uint8_t *reg);
void SLAl(uint8_t *reg);

// End

void SLAHLm();

// SRAr instructions expansion to all possibilities of r.
void SRAr(uint8_t *reg);

void SRAa(uint8_t *reg);
void SRAb(uint8_t *reg);
void SRAc(uint8_t *reg);
void SRAd(uint8_t *reg);
void SRAe(uint8_t *reg);
void SRAh(uint8_t *reg);
void SRAl(uint8_t *reg);

// End

void SRAHLm();

// SRLr instructions expansion to all possibilities of r.
void SRLr(uint8_t *reg);

void SRLa(uint8_t *reg);
void SRLb(uint8_t *reg);
void SRLc(uint8_t *reg);
void SRLd(uint8_t *reg);
void SRLe(uint8_t *reg);
void SRLh(uint8_t *reg);
void SRLl(uint8_t *reg);

// End

void SRLHLm();

// SWAPr instructions expansion to all possibilities of r.
void SWAPr(uint8_t *reg);

void SWAPa(uint8_t *reg);
void SWAPb(uint8_t *reg);
void SWAPc(uint8_t *reg);
void SWAPd(uint8_t *reg);
void SWAPe(uint8_t *reg);
void SWAPh(uint8_t *reg);
void SWAPl(uint8_t *reg);

// End

void SWAPHLm();

/*---- Bit Operations -----*/
void BITr(uint8_t bit, uint8_t reg);

void BIT0a();
void BIT0b();
void BIT0c();
void BIT0d();
void BIT0e();
void BIT0h();
void BIT0l();

void BIT1a();
void BIT1b();
void BIT1c();
void BIT1d();
void BIT1e();
void BIT1h();
void BIT1l();

void BIT2a();
void BIT2b();
void BIT2c();
void BIT2d();
void BIT2e();
void BIT2h();
void BIT2l();

void BIT3a();
void BIT3b();
void BIT3c();
void BIT3d();
void BIT3e();
void BIT3h();
void BIT3l();

void BIT4a();
void BIT4b();
void BIT4c();
void BIT4d();
void BIT4e();
void BIT4h();
void BIT4l();

void BIT5a();
void BIT5b();
void BIT5c();
void BIT5d();
void BIT5e();
void BIT5h();
void BIT5l();

void BIT6a();
void BIT6b();
void BIT6c();
void BIT6d();
void BIT6e();
void BIT6h();
void BIT7l();

void BIT7a();
void BIT7b();
void BIT7c();
void BIT7d();
void BIT7e();
void BIT7h();
void BIT7l();

// End

// BIT b, (HL) functions. Each one represents a possibility to bit
void BITHLm(uint8_t bit);

void BIT0HLm();
void BIT1HLm();
void BIT2HLm();
void BIT3HLm();
void BIT4HLm();
void BIT5HLm();
void BIT6HLm();
void BIT7HLm();

// End

// SET b, r functions. Each one represents a possibility to a bit and r
void SETr(uint8_t bit, uint8_t *reg);

void SET0a();
void SET0b();
void SET0c();
void SET0d();
void SET0e();
void SET0h();
void SET0l();

void SET1a();
void SET1b();
void SET1c();
void SET1d();
void SET1e();
void SET1h();
void SET1l();

void SET2a();
void SET2b();
void SET2c();
void SET2d();
void SET2e();
void SET2h();
void SET2l();

void SET3a();
void SET3b();
void SET3c();
void SET3d();
void SET3e();
void SET3h();
void SET3l();

void SET4a();
void SET4b();
void SET4c();
void SET4d();
void SET4e();
void SET4h();
void SET4l();

void SET5a();
void SET5b();
void SET5c();
void SET5d();
void SET5e();
void SET5h();
void SET5l();

void SET6a();
void SET6b();
void SET6c();
void SET6d();
void SET6e();
void SET6h();
void SET6l();

void SET7a();
void SET7b();
void SET7c();
void SET7d();
void SET7e();
void SET7h();
void SET7l();

// End

// SET b, (HL) functions. Each one represents a possibility to a bit
void SETHLm(uint8_t bit);

void SET0HLm();
void SET1HLm();
void SET2HLm();
void SET3HLm();
void SET4HLm();
void SET5HLm();
void SET6HLm();
void SET7HLm();

// End

// RES b, r functions. Each one represents a possibility to a bit and r
void RESr(uint8_t bit, uint8_t *reg);

void RES0a();
void RES0b();
void RES0c();
void RES0d();
void RES0e();
void RES0h();
void RES0l();

void RES1a();
void RES1b();
void RES1c();
void RES1d();
void RES1e();
void RES1h();
void RES1l();

void RES2a();
void RES2b();
void RES2c();
void RES2d();
void RES2e();
void RES2h();
void RES2l();

void RES3a();
void RES3b();
void RES3c();
void RES3d();
void RES3e();
void RES3h();
void RES3l();

void RES4a();
void RES4b();
void RES4c();
void RES4d();
void RES4e();
void RES4h();
void RES4l();

void RES5a();
void RES5b();
void RES5c();
void RES5d();
void RES5e();
void RES5h();
void RES5l();

void RES6a();
void RES6b();
void RES6c();
void RES6d();
void RES6e();
void RES6h();
void RES6l();

void RES7a();
void RES7b();
void RES7c();
void RES7d();
void RES7e();
void RES7h();
void RES7l();

// End

// RES b, (HL) functions. Each one represents a possibility to a bit
void RESHLm(uint8_t bit);

void RES0HLm();
void RES1HLm();
void RES2HLm();
void RES3HLm();
void RES4HLm();
void RES5HLm();
void RES6HLm();
void RES7HLm();

// End

/*----- Jump Instructions -----*/
void JPnn();
void JPNZnn();
void JPZnn();
void JPNCnn();
void JPCnn();
void JRe();
/*
  Nota
  Watch all JRcce functions because e can be from -127 to 129.
*/
void JRNZe();
void JRZe();
void JRNCe();
void JRCe();
void JPHLm();

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

// RST instructions expansion to all possibilities of values.
void RST(uint8_t t);
void RST00();
void RST08();
void RST10();
void RST18();
void RST20();
void RST28();
void RST30();
void RST38();

// End

/*----- General-Purpose Arithmetic Operations and CPU Control Instructions -----*/
void DAA();
void CPL();
void NOP();
void CCF();
void SCF();
void DI();
void EI();
void HALT();
void STOP();

#endif