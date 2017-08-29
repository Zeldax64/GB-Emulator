#include "debugcpu.h"

/*----- Functions -----*/
void debugInstruction(uint16_t op);


struct InstructionSetDebug {
	char* inst;
};

const static struct InstructionSetDebug instruction_debug[0x100];

const struct InstructionSetDebug instruction_debug[0x100] = {
	// 0x00
	{"NOP"},
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
