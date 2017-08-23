#ifndef _BIT
#define _BIT

/*
	General bit operations.
*/

#include <stdbool.h>
#include <stdint.h>

/*----- Defines -----*/
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

uint8_t testBit(uint8_t var, uint8_t bit);
uint8_t setBit(uint8_t var, uint8_t bit);
uint8_t resetBit(uint8_t var, uint8_t bit);


#endif