#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "point.h"
#include "vowel.h"

static struct vowel*
mkvow(char* label, struct point* P1, struct point* P2)
{
	struct vowel* v = NULL;
	if ((v = calloc(1, sizeof(struct vowel))) == NULL)
		err(1, NULL);
	v->label = label;
	v->V[0] = P1;
	v->V[1] = P2;
	return v;
}

struct vowel*
getvow(char* line)
{
	char* l = NULL;
	char* p = NULL;
	struct point* P1;
	struct point* P2;
	if (line == NULL || *line == '\0')
		return NULL;
	while (isspace(*line))
		line++;
	if (!isdigit(*line)) {
		p = strsep(&line, " \t\n");
		l = strdup(p);
	}
	if ((P1 = getpoint(&line)) == NULL) {
		warnx("invalid point\n");
		return NULL;
	}
	if (line == NULL) {
		/* monophthong ends here */
		return mkvow(l, P1, NULL);
	}
	if ((P2 = getpoint(&line)) == NULL) {
		warnx("invalid point\n");
		return NULL;
	}
	return mkvow(l, P1, P2);
}
