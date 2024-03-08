#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "vowel.h"
#include "group.h"
#include "vplot.h"

int
adjplot(struct vplot* p, struct group* g)
{
	if (p == NULL || p == NULL)
		return -1;
	if (g->min[0] < p->F1min || p->F1min == 0)
		p->F1min = g->min[0];
	if (g->max[0] > p->F1max)
		p->F1max = g->max[0];
	if (g->min[1] < p->F2min || p->F2min == 0)
		p->F2min = g->min[1];
	if (g->max[1] > p->F2max)
		p->F2max = g->max[1];
	return 0;
}

int
addgroup(struct group* g, struct vplot *p)
{
	if (g == NULL || p == NULL)
		return -1;
	if (p->head == NULL) {
		p->head = g;
		p->tail = g;
		p->size = 1;
	} else {
		p->tail->next = g;
		p->tail = g;
		p->size++;
	}
	adjplot(p, g);
	return 0;
}
