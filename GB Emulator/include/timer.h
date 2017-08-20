#ifndef _TIMER
#define _TIMER

#include <stdbool.h>
#include <stdint.h>

#include "mmu.h"
#include "interrupt.h"

/*----- Defines -----*/
#define TIM_DIVR 0xFF04 // Divider Register
#define TIM_TIMA 0xFF05 // Timer Counter: 8-byte register
#define TIM_TMA 0xFF06  // Timer modulo
#define TIM_TMC 0xFF07  // Timer Controller: 3-bit register

/*----- Function prototypes -----*/
void TIM_updateTimers(int16_t cycles);
void TIM_updateDIVR(int16_t cycles);
uint8_t TIM_getClockFreq();
void TIM_setClockFreq();

/*----- Timer struct -----*/
typedef struct gbtimer {
	int16_t timer_counter;
	int16_t divider_counter;
	uint8_t divr;
	uint8_t tima;
	uint8_t tma;
	uint8_t tmc;
}GB_TIM;

/*---- Variables -----*/
GB_TIM GB_tim;


#endif

