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
#include "svg.h"
#include "ps.h"

#define PS  1
#define PDF 2
#define SVG 3

#define D(F) ((F - (F % 100)) - 100)
#define U(F) ((F - (F % 100)) + 200)

static void
usage(void)
{
	fprintf(stderr, "usage: vp [-aegl] [[input] [output]]\n");
}

int
main(int argc, char** argv)
{
	int c;
	int otype = PS;
	char *suf = NULL;
	struct group* g = NULL;
	struct vplot* p = NULL;
	char *ifile = NULL;
	char *ofile = NULL;
	FILE *ifp = stdin;
	FILE *ofp = stdout;
	size_t len = 0;

	while ((c = getopt(argc, argv, "aegl")) != -1) switch (c) {
	}
	argc -= optind;
	argv += optind;

	if (argc > 2) {
		usage();
		return 1;
	}
	if (argc > 0 && (ifile = argv[0]))  {
		if ((ifp = fopen(ifile, "r")) == NULL)
			err(1, "%s", ifile);
	}
	if (argc > 1 && (ofile = argv[1]) && strcmp(ofile, "-")) {
		if ((suf = rindex(ofile, '.')) == NULL)
			errx(1, "%s: output format unknown", ofile);
		if ((strcmp(suf, ".ps")) == 0) {
			otype = PS;
		} else if ((strcmp(suf, ".svg")) == 0) {
			otype = SVG;
		} else {
			errx(1, "%s: output format unknown", ofile);
		}
		if ((ofp = fopen(ofile, "w")) == NULL)
			err(1, "%s", ofile);
	} else if (ifile && strcmp(ifile, "-")) {
		ofile = calloc(1, 4 + (len = strlen(ifile)));
		strlcpy(ofile, ifile, len);
		if ((suf = rindex(ofile, '.')) == NULL)
			suf = ofile + len;
		strlcpy(suf, ".ps", 4);
		if ((ofp = fopen(ofile, "w")) == NULL)
			err(1, "%s", ofile);
	}

	if ((p = calloc(1, sizeof(struct vplot))) == NULL)
		err(1, NULL);

	while ((g = getgroup(ifp)))
		addgroup(g, p);

	/* adjust the canvas */
	p->F1min = D(p->F1min);
	p->F1max = U(p->F1max);
	p->F2min = D(p->F2min);
	p->F2max = U(p->F2max);

	switch (otype) {
		case PS:
			pswrite(p, ofp);
			break;
		case SVG:
			svgwrite(p, ofp);
			break;
		default:
			break;
	}

	fclose(ifp);
	fclose(ofp);

	return 0;
}
