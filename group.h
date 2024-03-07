#ifndef _GROUP_H_
#define _GROUP_H_

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
	unsigned F1min;
	unsigned F1max;
	unsigned F2min;
	unsigned F2max;
	unsigned gx,gy;
	struct ellipse* e;
	struct vowel* head;
	struct vowel* tail;
	struct group* next;
};

struct group* getgroup(FILE*);
void gravity(struct group*);
void ellipse(struct group*);

#endif
