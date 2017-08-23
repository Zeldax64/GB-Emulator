#include "bit.h"

uint8_t testBit(uint8_t var, uint8_t bit) {
	return (var & bit) ? 1 : 0;
}

uint8_t setBit(uint8_t var, uint8_t bit) {
	return var |= bit;
}

uint8_t resetBit(uint8_t var, uint8_t bit) {
	return var &= (~bit);
}
