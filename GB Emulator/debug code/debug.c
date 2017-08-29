#include "debug.h"

/*----- Functions -----*/

/*
	Compare two 1 byte arrays starting the given address.
	@param ar1: pointer to the begin of a 1 byte array 
	@param ar2: pointer to the begin of a 1 byte array
	@param end: number of elements to be compared
	@return true if arrays are equal and false if not
*/
bool cmpArray8(uint8_t *ar1, uint8_t *ar2, uint32_t end) {
	for (uint32_t i = 0; i < end; i++) {
		if (*(ar1 + i) != *(ar2 + i)) {
			//if (*ar1 != *ar2) {

			printf("Error in value %d\nar1: %d   ar2: %d\n", i, *(ar1 + i), *(ar2 + i));
			return false;
		}
		//ar1++; ar2++;
	}
	printf("Identical arrays!\n");
	return true;
}