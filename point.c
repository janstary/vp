#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "point.h"

static uint16_t
getnum(char** line)
{
	uint16_t F = 0;
	char* p = NULL;
	const char* e = NULL;
	while (*line && isspace(**line))
		(*line)++;
	if ((p = strsep(line, " \t\n")) == NULL) {
		warnx("premature end of line");
		return 0;
	}
	if ((F = strtonum(p, 1, 8000, &e)) == 0) {
		warnx("'%s' %s", p, e);
		return 0;
	}
	return F;
}

struct point*
mkpoint(int F1, int F2)
{
	struct point* P;
	if ((P = calloc(1, sizeof(struct point))) == NULL)
		return NULL;
	P->F1 = F1;
	P->F2 = F2;
	return P;
}

struct point*
getpoint(char** line)
{
	int F1, F2;
	if ((F1 = getnum(line)) == 0) {
		warnx("invalid F1");
		return NULL;
	}
	if ((F2 = getnum(line)) == 0) {
		warnx("invalid F2");
		return NULL;
	}
	return mkpoint(F1, F2);
}
