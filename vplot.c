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
	if (g->F1min < p->F1min || p->F1min == 0)
		p->F1min = g->F1min;
	if (g->F1max > p->F1max)
		p->F1max = g->F1max;
	if (g->F2min < p->F2min || p->F2min == 0)
		p->F2min = g->F2min;
	if (g->F2max > p->F2max)
		p->F2max = g->F2max;
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
