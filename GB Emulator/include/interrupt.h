#ifndef _INTERRUPT
#define _INTERRUPT

/*----- Includes -----*/
/* General Includes */
#include <stdint.h>
#include <stdbool.h>

/* Emulator Incldues */
#include "bit.h"
#include "mmu.h"
#include "cpu.h"

/*----- Defines -----*/
// Interupt register addresses
#define INT_REQ 0xFF0F
#define INT_ENA 0xFFFF

// Define handle addresses to each interrupt
#define VBLANK_ADDR 0x40
#define LCD_ADDR 0x48
#define TIM_ADDR 0x50
#define JOY_ADDR 0x60

// Define bits to each interrupt to use with an interrupt request
#define VBLANK_BIT 0x1
#define LCD_BIT 0x2
#define TIM_BIT 0x4
#define JOY_BIT 0x10

/*----- Function Prototypes -----*/
void INT_serviceInt(uint8_t bit);
void INT_reqInt(uint8_t bit);
void INT_doInt();

/*----- interrupt struct -----*/
typedef struct gbinterrupt {
	uint8_t int_req; // 0XFF0F - Interrupt Request
	uint8_t int_ena; // 0xFFFF - Interrupt Enable
}GB_INT;

GB_INT gb_int;

#endif