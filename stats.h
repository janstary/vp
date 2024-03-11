#ifndef _STATS_H_
#define _STATS_H_

#include "group.h"

struct matrix {
	float M[2][2];
	float e[2][2];
	float l[2];
};

void gravity(struct group*);
void ellipse(struct group*);

#endif
