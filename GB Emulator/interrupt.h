#ifndef _INTERRUPT
#define _INTERRUPT

#include <stdint.h>
#include <stdbool.h>

#include "mmu.h"
#include "cpu.h"

/*----- Defines -----*/
// Define handle addresses to each interrupt
#define VBLANK_ADDR 0x40
#define LCD_ADDR 0x48
#define TIM_ADDR 0x50
#define JOY_ADDR 0x60

// Define bits to each interrupt to use with interrupt_request
#define VBLANK_BIT 0x1
#define LCD_BIT 0x2
#define TIM_BIT 0x4
#define JOY_BIT 0x10

/*----- Function Prototypes -----*/
void INT_reqInt(uint8_t bit);
void INT_doInt();

/*----- interrupt struct -----*/
typedef struct gbinterrupt {
	uint8_t interrupt_enable;
	uint8_t interrupt_request;
}GB_INT;

GB_INT GB_int;

#endif