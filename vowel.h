#ifndef _VOWEL_H_
#define _VOWEL_H_

struct vowel {
	char* label;
	uint32_t color;
	unsigned F1, F2;
	unsigned G1, G2;
/* F1 and F2 are formants of the vowel,
 * or the first target in a diphthong.
 * G1 and G2 are either zero, or the formants
 * of the second target in a diphthong. */
	struct vowel* next;
};

struct vowel* getvow(char*);
void prvow(struct vowel*);

#endif

