#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdint.h>
#include "vowel.h"

struct cloud {
	struct point c;
	float min, maj;
	float slope;
};

struct group {
	int size;
	char* label;
	uint32_t color;
	struct point* min;
	struct point* max;
	struct point* G[2];
	struct cloud* E[2];
	struct vowel* head;
	struct vowel* tail;
	struct group* next;
};

struct group* getgroup(FILE*);
void gravity(struct group*);
void ellipse(struct group*);

#endif
