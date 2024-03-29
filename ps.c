#include <stdint.h>
#include <stdio.h>

#include "vowel.h"
#include "group.h"
#include "vplot.h"
#include "color.h"
#include "ps.h"

static void
psvow(struct vowel* v, FILE* f)
{
	if (v == NULL)
		return;
	fprintf(f, "(%s) ", v->label ? v->label: "");
	fprintf(f, "%u %u ", v->V[0]->F1, v->V[0]->F2);
	if (v->V[1])
		fprintf(f, "%u %u 5 arrow", v->V[1]->F1, v->V[1]->F2);
	else
		fprintf(f, "5 dot");
	fprintf(f, "\n");
}

static void
psrgb(FILE *f, uint32_t rgb) {
	fprintf(f, "%f %f %f setrgbcolor\n",
		((rgb & 0xff0000) >> 16) / 255.0,
		((rgb & 0x00ff00) >>  8) / 255.0,
		((rgb & 0x0000ff) >>  0) / 255.0);
}

static void
psgroup(struct group* g, FILE* f)
{
	int i;
	struct vowel* v;
	if (g == NULL)
		return;
	fprintf(f, "\n");
	if (g->label)
		fprintf(f, "%% %s\n", g->label);
	g->color = rgb(g->color);
	psrgb(f, g->color);
	for (v = g->head; v; v = v->next)
		psvow(v, f);
	for (i = 0; i < 2; i++) {
		if (g->G[i]) {
			fprintf(f, "() %u %u 10 dot\n",
				g->G[i]->F1, g->G[i]->F2);
		}
		if (g->E[i]) {
			fprintf(f, "%f %f %f %d %d ellipse\n",
			g->E[i]->major, g->E[i]->minor, g->E[i]->angle,
			g->G[i]->F1, g->G[i]->F2);
		}
	}
}

int
pswrite(struct vplot* p, FILE* f)
{
	int hzw, hzh;
	struct group* g = NULL;

	if (p == NULL || f == NULL)
		return -1;

	hzw = p->max->F1 - p->min->F1;
	hzh = p->max->F2 - p->min->F2;

	fprintf(f,
		"%%!PS-Adobe-3.0\n"
		"%%%%DocumentMedia: VP %d %d 0 () ()\n"
		"%%%%EndComments\n"
		"%%%%BeginSetup\n"
		"%%%%BeginFeature: *PageSize VP\n"
		"<</PageSize [%d %d]>>setpagedevice\n"
		"%%%%EndFeature\n"
		"%%%%EndSetup\n"
		"%%%%Page: 1 1\n\n",
		hzw, hzh, hzw, hzh);

	/* We can translate back down to F1 neg F2 neg
	 * and have the dots be placed inside the actual range. */

	fprintf(f, "/F1min { %4u } def\n", p->min->F1);
	fprintf(f, "/F1max { %4u } def\n", p->max->F1);
	fprintf(f, "/F2min { %4u } def\n", p->min->F2);
	fprintf(f, "/F2max { %4u } def\n", p->max->F2);
	fprintf(f, "\n");

	fprintf(f, "/hzw { %d } def\n", hzw);
	fprintf(f, "/hzh { %d } def\n", hzh);
	fprintf(f, "\n");

	/* the plotting routines */

	fprintf(f, "/norm "
		"{ dup mul exch dup mul add sqrt } def\n\n");

	fprintf(f, "/dot {\n"
		"\t/r exch def\n"
		"\tround /y exch def\n"
		"\tround /x exch def\n"
		"\tnewpath x y r 0 360 arc closepath fill\n"
		"\tx y moveto r 1 add r 1 add neg rmoveto\n"
		"\tshow\n} def\n\n");

	fprintf(f, "/adict 20 dict def\n"
		"adict begin /mtrx matrix def end\n"
		"/arrow {\n\tadict begin\n"
		"\t/ds exch def\n"
		"\t/heady exch def /headx exch def\n"
		"\t/taily exch def /tailx exch def\n"
		"\ttailx taily ds dot\n"
		"\t/tailx tailx round def\n"
		"\t/taily taily round def\n"
		"\t/headx headx round def\n"
		"\t/heady heady round def\n"
		"\t/dx headx tailx sub def\n"
		"\t/dy heady taily sub def\n"
		"\t/len dx dy norm def\n"
		"\t/ang dy dx atan def\n"
		"\t/hb len 20 sub def\n"
		"\t/th 1 def\n"
		"\t/ht 3 def\n"
		"\t/smtx mtrx currentmatrix def\n"
		"\tnewpath tailx taily translate ang rotate\n"
		"\t0 th moveto hb th lineto hb ht lineto len 0 lineto\n"
		"\thb ht neg lineto hb th neg lineto 0 th neg lineto\n"
		"\tclosepath fill\n"
		"\tsmtx setmatrix\n"
		"\tend\n} def\n\n");

	fprintf(f, "/ellipse {\n"
		"\tgsave\n"
		"\ttranslate rotate scale\n"
		"\t1 50 div setlinewidth\n"
		/* FIXME line width should be constant;
		 * we know the ratio: scale back to 1 */
		"\tnewpath 0 0 1 0 360 arc stroke\n"
		"\tgrestore\n"
		"\t} def\n\n");

	/* a font that has the glyphs */

	fprintf(f, "/Charis-SIL findfont 20 scalefont setfont\n");
	fprintf(f, "\n");

	/* axes and ticks  */

	fprintf(f, "newpath\n");
	fprintf(f, "5 5 moveto\n");
	fprintf(f, "hzw 5 sub 0 rlineto\n");
	fprintf(f, "0 hzh 10 sub rlineto\n");
	fprintf(f, "hzw 5 sub neg 0 rlineto\n");
	fprintf(f, "closepath stroke\n");
	fprintf(f, "\n");

	fprintf(f, "/tick 4 string def\n\n");

	fprintf(f, "F1min 100 add 100 F1max 100 sub {\n");
	fprintf(f, "\tdup F1min sub \n");
	fprintf(f, "\tnewpath 5 moveto 0 5 rlineto 2 3 rmoveto\n");
	fprintf(f, "\tgsave 90 rotate tick cvs show grestore stroke\n");
	fprintf(f, "} for\n");

	fprintf(f, "F2min 100 add 100 F2max 100 sub {\n");
	fprintf(f, "\tdup F2min sub \n");
	fprintf(f, "\tnewpath 5 exch moveto 5 0 rlineto 2 -2 rmoveto\n");
	fprintf(f, "\ttick cvs show stroke\n");
	fprintf(f, "} for\n\n");

	/* Shift the origin to the minimal point
	 * and scale to fit if needed FIXME*/
	fprintf(f, "F1min neg F2min neg translate 1.0 dup scale\n");

	/* Draw the actual vowel groups */
	for (g = p->head; g; g = g->next)
		psgroup(g, f);
	fprintf(f, "\n");

	/* Display the group labels */
	fprintf(f, "F1min 50 add F2max 100 sub translate\n");
	fprintf(f, "/Charis-SIL findfont 15 scalefont setfont\n");
	for (g = p->head; g; g = g->next) {
		if (g->label) {
			psrgb(f, g->color);
			fprintf(f, "0 20 neg translate 0 0 moveto\n");
			fprintf(f, "(%s) show\n", g->label);
		}
	}
	fprintf(f, "\n");

	fprintf(f, "showpage\n");
	fprintf(f, "%%%%Trailer\n");
	fprintf(f, "%%%%Pages: 1\n");
	fprintf(f, "%%%%EOF\n");

	return 0;
}
