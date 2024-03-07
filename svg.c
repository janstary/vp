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
	fprintf(f, "<circle cx='%u' cy='%u' r='6' fill='orange'/>",
		v->F1, v->F2);
	/*
	fprintf(f, "(%s) ", v->label ? v->label: "");
	if (v->G1 && v->G2)
		fprintf(f, "%4u %4u arrow", v->G1, v->G2);
<line x1="50" y1="50" x2="200" y2="200" stroke="blue" stroke-width="4" />
	else
		fprintf(f, "dot");
	*/
	fprintf(f, "\n");
}

/*
static void
svgrgb(FILE *f, uint32_t rgb) {
	fprintf(f, "%f %f %f setrgbcolor\n",
		((rgb & 0xff0000) >> 16) / 255.0,
		((rgb & 0x00ff00) >>  8) / 255.0,
		((rgb & 0x0000ff) >>  0) / 255.0);
}
*/

static void
svggroup(struct group* g, FILE* f, uint32_t rgb)
{
	struct vowel* v;
	if (g == NULL)
		return;
	if (g->label)
		fprintf(f, "<!-- %s -->\n", g->label);
	fprintf(f, "<g>\n");
	/*svgrgb(f, rgb);*/
	for (v = g->head; v; v = v->next)
		svgvow(v, f);
	fprintf(f, "</g>\n\n");
}

int
svgwrite(struct vplot* p, FILE* f)
{
	unsigned n = 0;
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
	for (n = 0, g = p->head; g; n++, g = g->next) {
		svggroup(g, f, rgb(n));
	}
	fprintf(f, "\n");

	/* Display the group labels */

	fprintf(f, "</svg>\n");
	return 0;
}
