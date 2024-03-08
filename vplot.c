#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "point.h"
#include "vowel.h"
#include "group.h"
#include "vplot.h"

static void
adjplot(struct vplot* p, struct group* g)
{
	if (p == NULL || p == NULL)
		return;
	if (p->min == NULL) {
		p->min = mkpoint(g->min->F1, g->min->F2);
	} else {
		if (g->min->F1 < p->min->F1)
			p->min->F1 = g->min->F1;
		if (g->min->F2 < p->min->F2)
			p->min->F2 = g->min->F2;
	}
	if (p->max == NULL) {
		p->max = mkpoint(g->max->F1, g->max->F2);
	} else {
		if (g->max->F1 > p->max->F1)
			p->max->F1 = g->max->F1;
		if (g->max->F2 > p->max->F2)
			p->max->F2 = g->max->F2;
	}
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
