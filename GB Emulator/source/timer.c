#include "timer.h"

/*
	There are 2 counters: TIMA and DIVR
	DIVR (Clock divider) works indepedent of the running program.
	TIMA must be set by the program
*/

/*
	NOTA
	I'm working with machine cycles instead of clock cycles. Thus all counter values are divided by 4
*/


/*----- Functions -----*/

void TIM_updateTimers(int16_t cycles) {
	TIM_updateDIVR(cycles); // Update Clock divider register

	if (gb_tim.tmc & 0x4) { // Is counter on?
		gb_tim.timer_counter -= cycles; 
		if (gb_tim.timer_counter <= 0) {
			TIM_setClockFreq(); // Reset timer_counter
			if (gb_tim.tima == 255) { // If overflow
				gb_tim.tima = gb_tim.tma; // Timer register = timer modulo 
				INT_reqInt(TIM_BIT);
			}
			else {
				gb_tim.tima++; // Increment timer register
			}
		}
	}
}

void TIM_updateDIVR(int16_t cycles) {
	gb_tim.divider_counter += cycles;
	if (gb_tim.divider_counter >= 64) {
		/*
			NOTA
			I'm using -= instead of = and 64 instead of 255 (machine cycle instead of clk cycle) 
		*/
		gb_tim.divider_counter -= 64;
		gb_tim.divr++;
	}
}

uint8_t TIM_getClockFreq() {
	return gb_tim.tmc & 0x3;
}

void TIM_setClockFreq() {
	switch (TIM_getClockFreq()) {
	case 0: gb_tim.timer_counter = 256; break; // Freq 4096 Hz
	case 1: gb_tim.timer_counter = 4; break;   // Freq 262144 Hz
	case 2: gb_tim.timer_counter = 16; break;  // Freq 65536 Hz
	case 3: gb_tim.timer_counter = 64; break;  // Freq 16382 Hz
	}
}

