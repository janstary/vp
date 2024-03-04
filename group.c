#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "vowel.h"
#include "group.h"

static int
adjgroup(struct group* g, struct vowel* v)
{
	if (g == NULL || v == NULL)
		return -1;
	if (v->F1 < g->F1min || g->F1min == 0)
		g->F1min = v->F1;
	if (v->F1 > g->F1max)
		g->F1max = v->F1;
	if (v->F2 < g->F2min || g->F2min == 0) {
		g->F2min = v->F2;
	}
	if (v->F2 > g->F2max)
		g->F2max = v->F2;
	if (v->G1 && v->G2) {
		if (v->G1 < g->F1min || g->F1min == 0)
			g->F1min = v->G1;
		if (v->G1 > g->F1max)
			g->F1max = v->G1;
		if (v->G2 < g->F2min || g->F2min == 0)
			g->F2min = v->G2;
		if (v->G2 > g->F2max)
			g->F2max = v->G2;
	}
	return 0;
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

void
prgroup(struct group* g)
{
	struct vowel* v;
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	if (g->label)
		printf("# %s\n", g->label);
	for (v = g->head; v; v = v->next)
		prvow(v);
	putchar('\n');
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
				for (p = line+1; p && *p && isspace(*p); p++)
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
