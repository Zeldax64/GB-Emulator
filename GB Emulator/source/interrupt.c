#include "interrupt.h"

/*----- Functions -----*/

void INT_serviceInt(uint8_t bit) {
	gb_cpu.ime = false;
	gb_int.int_req ^= bit; // Turn off the bit request of an interrupt

	gb_cpu.sp -= 2;
	wrWord(gb_cpu.sp, gb_cpu.pc);

	switch (bit) {
		case BIT0: gb_cpu.pc = VBLANK_ADDR; break;
		case BIT1: gb_cpu.pc = LCD_ADDR; break;
		case BIT2: gb_cpu.pc = TIM_ADDR; break;
		case BIT4: gb_cpu.pc = JOY_ADDR; break;
	}


}

void INT_reqInt(uint8_t bit) {
	gb_int.int_req |= bit;
}

void INT_doInt() {
	if (gb_cpu.ime == true) { // Is IME on?
		if (gb_int.int_req > 0) { // Is there any interrupt request?
			for (uint8_t bit = 0; bit <= BIT5; bit <<= 1) { // Bit verification
				if (gb_int.int_req & bit) { // Who requested the interrupt?
					if (gb_int.int_ena & bit) { // Is interrupt enabled for this device?
						INT_serviceInt(bit);
					}
				}
			}
		}
	}
}

