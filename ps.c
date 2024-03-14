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
			fprintf(f, "%d %d %f %f %f ellipse\n",
			g->G[i]->F1, g->G[i]->F2, g->E[i]->angle,
			g->E[i]->major, g->E[i]->minor);
		}
	}
}

int
pswrite(struct vplot* p, FILE* f)
{
	struct group* g = NULL;
	if (p == NULL || f == NULL)
		return -1;

	fprintf(f, "%%!PS-Adobe-3.0\n");
	fprintf(f, "%%%%Orientation: Portrait\n");
	fprintf(f, "%%%%DocumentMedia: A4 595 841 0 () ()\n");
	fprintf(f, "%%%%DocumentNeededResources: font Times-Roman\n");
	fprintf(f, "%%%%DocumentSuppliedResources: procset vowels\n");
	fprintf(f, "%%%%EndComments\n");
	fprintf(f, "%%%%BeginProlog\n");
	fprintf(f, "%%%%BeginResource: procset vowels 10170 10170\n");
	fprintf(f, "/f0 { /Times-Roman fs selectfont } def\n");
	fprintf(f, "/f1 { /Times-Bold fs selectfont } def\n");
	fprintf(f, "%%%%EndResource\n");
	fprintf(f, "%%%%EndProlog\n");
	fprintf(f, "%%%%BeginSetup\n");
	fprintf(f, "%%%%BeginFeature: *PageSize Letter\n");
	fprintf(f, "<</PageSize [612 790]>>setpagedevice\n");
	fprintf(f, "%%%%EndFeature\n");
	fprintf(f, "%%%%EndSetup\n");
	fprintf(f, "%%%%Page: 1 1\n");
	fprintf(f, "\n");

	/* PS itself will do the scaling, given the limits.
	 * Also, we can simply translate back down to F1 neg F2 neg
	 * and have the dots be placed inside the actual range. */

	fprintf(f, "/F1min { %4u } def\n", p->min->F1);
	fprintf(f, "/F1max { %4u } def\n", p->max->F1);
	fprintf(f, "/F2min { %4u } def\n", p->min->F2);
	fprintf(f, "/F2max { %4u } def\n", p->max->F2);
	fprintf(f, "\n");

	/* Compute the ratio of Hz to paper/bbox size
	 * so that we can use the Hz coordinates. */

	fprintf(f, "/Hz1 { 595 F1max F1min sub div mul } def\n");
	fprintf(f, "/Hz2 { 841 F2max F2min sub div mul } def\n");
	fprintf(f, "/hzw { F1max F1min sub Hz1 } def\n");
	fprintf(f, "/hzh { F2max F2min sub Hz2 } def\n");
	fprintf(f, "\n");

	/* the plotting routines */

	fprintf(f, "/norm "
		"{ dup mul exch dup mul add sqrt } def\n\n");

	fprintf(f, "/dot {\n"
		"\t/r exch def\n"
		"\tHz2 round /y exch def\n"
		"\tHz1 round /x exch def\n"
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
		"\t/tailx tailx Hz1 round def\n"
		"\t/taily taily Hz2 round def\n"
		"\t/headx headx Hz1 round def\n"
		"\t/heady heady Hz2 round def\n"
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
		"\t/min exch def /maj exch def /ang exch def\n"
		"\t/cy exch def /cx exch def\n"
		"\tgsave\n"
		"\tcx Hz1 cy Hz2 translate\n"
		"\tang rotate maj min scale\n"
		"\t1 25 Hz1 div setlinewidth\n"
		"\tnewpath 0 0 1 0 360 arc stroke\n"
		"\tgrestore\n"
		"\t} def\n\n");

	/* a font that has the glyphs */

	fprintf(f, "/Charis-SIL findfont 20 Hz2 scalefont setfont\n");
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
	fprintf(f, "\tdup F1min sub Hz1\n");
	fprintf(f, "\tnewpath 5 moveto 0 5 rlineto 2 3 rmoveto\n");
	fprintf(f, "\tgsave 90 rotate tick cvs show grestore stroke\n");
	fprintf(f, "} for\n");

	fprintf(f, "F2min 100 add 100 F2max 100 sub {\n");
	fprintf(f, "\tdup F2min sub Hz2\n");
	fprintf(f, "\tnewpath 5 exch moveto 5 0 rlineto 2 -2 rmoveto\n");
	fprintf(f, "\ttick cvs show stroke\n");
	fprintf(f, "} for\n");

	/* Shift the origin to the minimal point */
	fprintf(f, "\nF1min Hz1 neg F2min Hz2 neg translate\n");

	/* Draw the actual vowel groups */
	for (g = p->head; g; g = g->next)
		psgroup(g, f);
	fprintf(f, "\n");

	/* Display the group labels */
	fprintf(f, "F1min 50 add Hz1 F2max 100 sub Hz2 translate\n");
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
