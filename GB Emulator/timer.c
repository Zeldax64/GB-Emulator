#include "timer.h"

/*
	There are 2 counters: TIMA and DIVR
	DIVR works indepedent of the running program.
	TIMA must be set by the program
*/

#define DIVR 0xFF0r // Divider Register
#define TIMA 0xFF05 // Timer Counter: 8-byte register
#define TMA 0xFF06  // Timer modulo
#define TMC 0xFF07  // Timer Controller: 3-bit register

#define CLOCKSPEED 4194304
#define MCYCLESPEED 1048576

/*---- Variables -----*/
uint16_t clkmain = 0;
uint16_t clksub = 0;
uint16_t clkdiv = 0;
uint16_t regdiv = 0;
uint16_t regtima = 0;
uint16_t regtma = 0;
uint16_t regtac = 0;

void inc() {
	// Increment by the last opcode's time
	clksub += cpu.m;

	// No opcode takes longer than 4M-times,
	// so we need only check for overflow once
	/*
		There are some instructions which have more than 4M-times:
		LNnnSP: 5
		CALLnn: 6
		CALLcc: 6
		RETcc:  5

	*/
	if (clksub >= 4) {
		clkmain++;	 // Increments TIMA (Timer counter)
		clksub -= 4; // Decrements clksub (Timer Clock)

		// The DIV register increments at 1/16th
		// the rate, so keep a count of this
		clkdiv++;	// Increments Divider Counter's clock (Divider is a indepent timer)
		if (clkdiv == 16) {
			/*
				if regdiv is a uint8_t this code maybe be used
				regdiv++;
				clkdiv = 0;
			*/
			regdiv = (regdiv + 1) & 255; 
			clkdiv = 0;
		}
	}

	// Check whether a step needs to be made in the timer 
	check();
}

uint8_t threshold;

/*
	A division is applyed to threshold and it's checked
	if clkmain is >= threshold.
*/
void check() {
	if (regtac & 4) {	//regtac & 0b100: check wether timer is on
		switch (regtac & 3) {
			case 0: threshold = 64; break; // 4k
			case 1: threshold = 1;  break; // 256k
			case 2: threshold = 4;  break; // 64k
			case 3: threshold = 16; break; // 16k
		}
		if (clkmain >= threshold) {
			step();
		}
	}
}


/*
int timercounter;
uint32_t clockfrequency;

bool isClockEnabled() {
	return rdByte(TMC) & 0x2 ? true : false;
}

void updateTimer(int cycles) {
	doDividerRegister(cycles);
	// The clock must be enabled to update the clock

	if (isClockEnabled()) {
		timercounter -= cycles;
		// Enough cpuclock cycles have happened to update the tiemr
		if (timercounter <= 0) {
			//reset timerTracer to the correct value
			setClockFreq();
			// Timer about to overflow

			if (rdByte(TIMA) == 255) {
				wrByte(TIMA, rdByte(TMA));
				requestInterupt(2);
			}
			else{
				wrByte(TIMA, rdByte(TIMA) + 1);
			}
		}
	}
}

*/