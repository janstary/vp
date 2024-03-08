#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdint.h>
#include "vowel.h"

struct ellipse {
	unsigned x, y;
	unsigned a, b;
	float s;
};

struct group {
	int size;
	char* label;
	uint32_t color;
	unsigned min[2];
	unsigned max[2];
	int32_t grav[4];
	struct ellipse* e;
	struct vowel* head;
	struct vowel* tail;
	struct group* next;
};

struct group* getgroup(FILE*);
void gravity(struct group*);
void ellipse(struct group*);

#endif
