#include "maths.h"

int pown(int base, unsigned int exponent) {
	if (exponent == 0) return 1;

	int powsmall = pown(base, exponent >> 1);

	return powsmall * powsmall * ((exponent % 2 == 1) ? base : 1);
}