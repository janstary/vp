#ifndef _GROUP_H_
#define _GROUP_H_

#include "vowel.h"

struct group {
	int size;
	char* label;
	unsigned F1min;
	unsigned F1max;
	unsigned F2min;
	unsigned F2max;
	struct vowel* head;
	struct vowel* tail;
	struct group* next;
};

struct group* getgroup(FILE*);

#endif
