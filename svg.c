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
svgpoint(FILE* f, struct point* p, int size, uint32_t color)
{
	if (f == NULL
	||  p == NULL)
		return;
	fprintf(f, "<circle cx='%u' cy='%u' r='%u' fill='#%06x'/>\n",
		p->F1, p->F2, size, color);
}

static void
svgline(FILE* f, struct point* p1, struct point* p2, int s, uint32_t c)
{
	if (f == NULL
	|| p1 == NULL
	|| p2 == NULL)
		return;
	fprintf(f,
		"<line x1='%u' y1='%u' x2='%u' y2='%u'\n"
		"\tstroke='#%06x' stroke-width='%u'/>\n",
		p1->F1, p1->F2, p2->F1, p2->F2, c, s);
}

static void
svgvow(struct vowel* v, FILE* f)
{
	if (v == NULL)
		return;
	fprintf(f, "<g>");
	if (v->label)
		fprintf(f, "<!-- %s --> ", v->label);
	fprintf(f, "\n");
	if (v->V[0])
		svgpoint(f, v->V[0], 5, v->color);
	if (v->V[1]) {
		svgpoint(f, v->V[1], 5, v->color);
		svgline(f, v->V[0], v->V[1], 4, v->color);
	}
	fprintf(f, "</g>\n");
}

static void
svggroup(struct group* g, FILE* f)
{
	struct vowel* v;
	if (g == NULL)
		return;
	fprintf(f, "<g>");
	if (g->label)
		fprintf(f, "<!-- %s -->", g->label);
	fprintf(f, "\n");
	for (v = g->head; v; v = v->next) {
		v->color = g->color;
		svgvow(v, f);
	}
	if (g->G[0])
		svgpoint(f, g->G[0], 10, g->color);
	if (g->G[1])
		svgpoint(f, g->G[1], 10, g->color);
	fprintf(f, "</g>");
	if (g->label)
		fprintf(f, "<!-- %s -->", g->label);
	fprintf(f, "\n\n");
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
		p->max->F1 - p->min->F1, p->max->F2 - p->min->F2);
	fprintf(f, " viewBox='%u %u %u %u'\n",
		p->min->F1, p->min->F2, p->max->F1, p->max->F2);
	fprintf(f, "\txmlns='" SVGXML "'>\n");
	fprintf(f, "\n");

	/* a font that has the glyphs */

	/* axes and ticks  */

	fprintf(f,
		"<rect x='%u' y='%u' width='%u' height='%u'\n"
		"\tfill='white' stroke-width='2' stroke='black'/>\n\n",
		p->min->F1, p->min->F2,
		p->max->F1 - p->min->F1,
		p->max->F2 - p->min->F2);

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
