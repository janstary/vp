#ifndef _VOWEL_H_
#define _VOWEL_H_

#include <stdint.h>

struct vowel {
	char* label;
	uint32_t color;
	int32_t F1, F2;
	int32_t G1, G2;
/* F1 and F2 are formants of the vowel,
 * or the first target in a diphthong.
 * G1 and G2 are either zero, or the formants
 * of the second target in a diphthong. */
	struct vowel* next;
};

struct vowel* getvow(char*);
void prvow(struct vowel*);

#endif

