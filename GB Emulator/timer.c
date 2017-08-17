#include "timer.h"

/*
	There are 2 counters: TIMA and DIVR
	DIVR (Clock divider) works indepedent of the running program.
	TIMA must be set by the program
*/

/*
	NOTA
	I'm working with machine cycles instead of clock cycles. Thus, all counter values are divided by 4
*/


/*----- Functions -----*/

void TIM_updateTimers(int16_t cycles) {
	TIM_updateDIVR(cycles); // Update Clock divider register

	if (tim.tmc & 0x4) { // Is counter on?
		tim.timer_counter -= cycles; 
		if (tim.timer_counter <= 0) {
			TIM_setClockFreq(); // Reset timer_counter
			if (tim.tima == 255) { // If overflow
				tim.tima = tim.tma; // Timer register = timer modulo 
				//requestInterrupt(2);
			}
			else {
				tim.tima++; // Increment timer register
			}
		}
	}
}

void TIM_updateDIVR(int16_t cycles) {
	tim.divider_counter += cycles;
	if (tim.divider_counter >= 64) {
		/*
			NOTA
			I'm using -= instead of = and 64 instead of 255 (machine cycle instead of clk cycle) 
		*/
		tim.divider_counter -= 64;
		tim.divr++;
	}
}

uint8_t TIM_getClockFreq() {
	return tim.tmc & 0x3;
}

void TIM_setClockFreq() {
	switch (TIM_getClockFreq()) {
	case 0: tim.timer_counter = 256; break; // Freq 4096 Hz
	case 1: tim.timer_counter = 4; break;	// Freq 262144 Hz
	case 2: tim.timer_counter = 16; break;	// Freq 65536 Hz
	case 3: tim.timer_counter = 64; break;  // Freq 16382 Hz
	}
}

