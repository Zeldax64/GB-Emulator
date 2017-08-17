#include "interrupt.h"

bool testBit(uint8_t val, uint8_t bit);
void INT_serviceInt(uint8_t interrupt);

/*----- Functions -----*/

bool testBit(uint8_t val, uint8_t bit) {
	return (val & bit) ? 1 : 0;
}

void INT_serviceInt(uint8_t bit) {
	GB_cpu.ime = false;
	GB_int.interrupt_request ^= bit; // Turn off the bit request of an interrupt

	GB_cpu.sp -= 2;
	wrWord(GB_cpu.sp, GB_cpu.pc);

	switch (bit) {
		case BIT0: GB_cpu.pc = VBLANK_ADDR; break;
		case BIT1: GB_cpu.pc = LCD_ADDR; break;
		case BIT2: GB_cpu.pc = TIM_ADDR; break;
		case BIT4: GB_cpu.pc = JOY_ADDR; break;
	}


}

void INT_reqInt(uint8_t bit) {
	GB_int.interrupt_request |= bit;
}

void INT_doInt() {
	if (GB_cpu.ime == true) { // Is IME on?
		if (GB_int.interrupt_request > 0) { // Is there any interrupt request?
			for (uint8_t bit = 0; bit <= BIT5; bit <<= 1) { // Bit verification
				if (GB_int.interrupt_request & bit) { // Who requested a interrupt?
					if (testBit(GB_int.interrupt_enable, bit)) { // Is interrupt enabled for this device?
						INT_serviceInt(bit);
					}
				}
			}
		}
	}
}

