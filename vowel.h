#ifndef _VOWEL_H_
#define _VOWEL_H_

#include <stdint.h>
#include "point.h"

struct vowel {
	char* label;
	uint32_t color;
	struct point* V[2];
	struct vowel* next;
};

struct vowel* getvow(char*);

#endif

