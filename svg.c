#include <stdint.h>
#include <stdio.h>

#include "vowel.h"
#include "group.h"
#include "vplot.h"
#include "color.h"
#include "svg.h"

#define SVGVER "-//W3C//DTD SVG 1.1//EN"
#define SVGURI "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd"
#define SVGXML "http://www.w3.org/2000/svg"

static void
svgvow(struct vowel* v, FILE* f)
{
	if (v == NULL)
		return;
	fprintf(f, "<circle cx='%u' cy='%u' r='6' fill='#%06x'/>\n",
		v->F1, v->F2, v->color);
	if (v->label) {
		/*fprintf(f, "(%s) ", v->label);*/
	}
	if (v->G1 && v->G2) {
		fprintf(f, "<line x1='%u' y1='%u' x2='%u' y2='%u'\n",
			v->F1, v->F2, v->G1, v->G2);
		fprintf(f, "\tstroke='#%06x' stroke-width='4'/>\n",
			v->color);
	}
}

static void
svggroup(struct group* g, FILE* f)
{
	struct vowel* v;
	if (g == NULL)
		return;
	if (g->label)
		fprintf(f, "<!-- %s -->\n", g->label);
	fprintf(f, "<g>\n");
	for (v = g->head; v; v = v->next) {
		v->color = g->color;
		svgvow(v, f);
	}
	fprintf(f, "</g>\n\n");
}

int
svgwrite(struct vplot* p, FILE* f)
{
	struct group* g = NULL;
	if (p == NULL || f == NULL)
		return -1;

	fprintf(f, "<?xml version='1.0'");
	fprintf(f, " encoding='UTF-8' standalone='yes'?>\n");
	fprintf(f, "<!DOCTYPE svg PUBLIC\n");
	fprintf(f, "\t'" SVGVER "'\n");
	fprintf(f, "\t'" SVGURI "'>\n");
	fprintf(f, "<svg width='%u' height='%u'",
		p->F1max - p->F1min, p->F2max - p->F2min);
	fprintf(f, " viewBox='%u %u %u %u'\n",
		p->F1min, p->F2min, p->F1max, p->F2max);
	fprintf(f, "\txmlns='" SVGXML "'>\n");
	fprintf(f, "\n");

	/* a font that has the glyphs */

	/* axes and ticks  */

	fprintf(f, "<rect x='%u' y='%u' width='%u' height='%u'\n",
		p->F1min, p->F2min, p->F1max-p->F1min, p->F2max-p->F2min);
	fprintf(f, "\tfill='white' stroke-width='1' stroke='black'/>\n");
	fprintf(f, "\n");

	/* FIXME ticks, in HZ and Bark */

	/* Draw the actual vowels */
	for (g = p->head; g; g = g->next) {
		g->color = rgb(g->color);
		svggroup(g, f);
	}

	/* Display the group labels */

	fprintf(f, "</svg>\n");
	return 0;
}
