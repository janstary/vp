#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "vowel.h"
#include "group.h"
#include "stats.h"

static void
adjbypoint(struct group* g, struct point* p)
{
	if (g == NULL || p == NULL)
		return;
	if (g->min == NULL) {
		g->min = mkpoint(p->F1, p->F2);
	} else {
		if (p->F1 < g->min->F1)
			g->min->F1 = p->F1;
		if (p->F2 < g->min->F2)
			g->min->F2 = p->F2;
	}
	if (g->max == NULL) {
		g->max = mkpoint(p->F1, p->F2);
	} else {
		if (p->F1 > g->max->F1)
			g->max->F1 = p->F1;
		if (p->F2 > g->max->F2)
			g->max->F2 = p->F2;
	}
}

static void
adjgroup(struct group* g, struct vowel* v)
{
	if (g == NULL || v == NULL)
		return;
	adjbypoint(g, v->V[0]);
	adjbypoint(g, v->V[1]);
}

void
gravity(struct group* g)
{
	/*
	int i;
	float grav[4];
	struct vowel* v;
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	for (i = 0; i < 4; i++) {
		for (grav[i] = 0, v = g->head; v; v = v->next)
			grav[i] += v->F[i];
		g->grav[i] = grav[i] / g->size;
	}
	*/
}

void
ellipse(struct group* g)
{
	/*
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	if ((g->e = calloc(1, sizeof(struct ellipse))) == NULL)
		err(1, NULL);
	*/
}

static int
addvow(struct vowel* v, struct group* g)
{
	if (v == NULL || g == NULL)
		return -1;
	if (g->head == NULL) {
		g->head = v;
		g->tail = v;
		g->size = 1;
	} else {
		g->tail->next = v;
		g->tail = v;
		g->size++;
	}
	adjgroup(g, v);
	return 0;
}

struct group*
getgroup(FILE* ifp)
{
	char* p = NULL;
	struct vowel* v = NULL;
	struct group* g = NULL;
	char* line = NULL;
	size_t cap = 0;
	ssize_t len;
	if (feof(ifp))
		return NULL;
	if ((g = calloc(1, sizeof(struct group))) == NULL)
		err(1, NULL);
	while ((len = getline(&line, &cap, ifp)) > 0) {
		line[--len] = '\0';
		if (*line == '\0') {
			if (g->size == 0) {
				continue;
			} else {
				free(line);
				return g;
			}
		}
		if (*line == '#') {
			if (g->label == NULL) {
				for (p=line+1; p && *p && isspace(*p); p++)
					;
				if (*p)
					g->label = strdup(p);
			}
			continue;
		}
		v = getvow(line);
		addvow(v, g);
	}
	free(line);
	return g;
}
