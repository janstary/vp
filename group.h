#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdint.h>
#include "vowel.h"

struct cloud {
	float major;
	float minor;
	float angle;
};

struct group {
	int size;
	char* label;
	uint32_t color;
	struct point* min;
	struct point* max;
	struct point* G[2];	/* center(s) of gravity */
	struct cloud* E[2];	/* ellipse(s) around Vs */
	struct vowel* head;
	struct vowel* tail;
	struct group* next;
};

struct group* getgroup(FILE*);

#endif
