#ifndef _VPLOT_H_
#define _VPLOT_H_

#include "group.h"

struct vplot {
	int size;
	char* label;
	struct point* min;
	struct point* max;
	struct group* head;
	struct group* tail;
};

int addgroup(struct group*, struct vplot*);

#endif
