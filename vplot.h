#ifndef _VPLOT_H_
#define _VPLOT_H_

#include "group.h"

struct vplot {
	int size;
	char* label;
	unsigned F1min;
	unsigned F1max;
	unsigned F2min;
	unsigned F2max;
	struct group* head;
	struct group* tail;
};

int addgroup(struct group*, struct vplot*);

#endif
