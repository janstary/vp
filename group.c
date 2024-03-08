#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "vowel.h"
#include "group.h"

static void
adjby(struct group* g, int32_t* F)
{
	if (F[0] < g->min[0] || g->min[0] == 0)
		g->min[0] = F[0];
	if (F[0] > g->max[0])
		g->max[0] = F[0];
	if (F[1] < g->min[1] || g->min[1] == 0)
		g->min[1] = F[1];
	if (F[1] > g->max[1])
		g->max[1] = F[1];
}

static void
adjgroup(struct group* g, struct vowel* v)
{
	if (g == NULL || v == NULL)
		return;
	adjby(g, v->F);
	if (v->F[2] && v->F[3])
		adjby(g, v->F+2);
}

void
gravity(struct group* g)
{
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
}

void
ellipse(struct group* g)
{
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	if ((g->e = calloc(1, sizeof(struct ellipse))) == NULL)
		err(1, NULL);
	/* FIXME center of gravity is a focus */
	/* FIXME major and minor axis, slope */
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
