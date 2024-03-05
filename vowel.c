#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <err.h>

#include "vowel.h"

void
prvow(struct vowel* v)
{
	if (v) {
		printf("%u\t%u", v->F1, v->F2);
		if (v->G1 && v->G2)
			printf("\t%u\t%u", v->G1, v->G2);
		putchar('\n');
	}
}

static struct vowel*
mkvow(char* label, uint16_t F1, uint16_t F2, uint16_t G1, uint16_t G2)
{
	struct vowel* v = NULL;
	if (F1 == 0 || F2 == 0)
		return NULL;
	if ((v = calloc(1, sizeof(struct vowel))) == NULL)
		err(1, NULL);
	v->label = label;
	v->F1 = F1;
	v->F2 = F2;
	v->G1 = G1;
	v->G2 = G2;
	return v;
}

uint16_t
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

struct vowel*
getvow(char* line)
{
	char* l = NULL;
	char* p = NULL;
	uint16_t F1 = 0, F2 = 0;
	uint16_t G1 = 0, G2 = 0;
	if (line == NULL || *line == '\0')
		return NULL;
	while (isspace(*line))
		line++;
	if (!isdigit(*line)) {
		p = strsep(&line, " \t\n");
		l = strdup(p);
	}
	if ((F1 = getnum(&line)) == 0) {
		warnx("invalid F1");
		return NULL;
	}
	if ((F2 = getnum(&line)) == 0) {
		warnx("invalid F1");
		return NULL;
	}
	if (line == NULL) {
		/* a monophthong ends here */
		return mkvow(l, F1, F2, 0, 0);
	}
	if ((G1 = getnum(&line)) == 0) {
		warnx("invalid G1");
		return NULL;
	}
	if ((G2 = getnum(&line)) == 0) {
		warnx("invalid G2");
		return NULL;
	}
	return mkvow(l, F1, F2, G1, G2);
}
