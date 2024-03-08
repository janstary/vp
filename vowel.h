#ifndef _VOWEL_H_
#define _VOWEL_H_

#include <stdint.h>

/* The first two F[] values are formants of the vowel,
 * or of the first target if the vowel is a diphthong.
 * The second two are either zero, or the formants
 * of the second target in a diphthong. */

struct vowel {
	char* label;
	int32_t F[4];
	uint32_t color;
	struct vowel* next;
};

struct vowel* getvow(char*);

#endif

