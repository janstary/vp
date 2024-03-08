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
	if (g->G[0]) {
		fprintf(f, "%% center of gravity\n");
		fprintf(f, "() %u %u 10 dot\n", g->G[0]->F1, g->G[0]->F2);
	}
	if (g->G[1]) {
		fprintf(f, "%% the other center of gravity\n");
		fprintf(f, "() %u %u 10 dot\n", g->G[1]->F1, g->G[1]->F2);
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

	fprintf(f, "/dist { dup mul exch dup mul add sqrt } def\n");
	fprintf(f, "\n");

	fprintf(f, "/dot {\n");
	fprintf(f, "\t/r exch def\n");
	fprintf(f, "\tHz2 round /y exch def\n");
	fprintf(f, "\tHz1 round /x exch def\n");
	fprintf(f, "\tnewpath x y r 0 360 arc closepath fill\n");
	fprintf(f, "\tx y moveto r 1 add r 1 add neg rmoveto\n");
	fprintf(f, "\tshow\n} def\n");
	fprintf(f, "\n");

	fprintf(f, "/adict 20 dict def\n");
	fprintf(f, "adict begin /mtrx matrix def end\n");
	fprintf(f, "/arrow {\n\tadict begin\n");
	fprintf(f, "\t/ds exch def\n");
	fprintf(f, "\t/heady exch def /headx exch def\n");
	fprintf(f, "\t/taily exch def /tailx exch def\n");
	fprintf(f, "\ttailx taily ds dot\n");

	fprintf(f, "\t/tailx tailx Hz1 round def\n");
	fprintf(f, "\t/taily taily Hz2 round def\n");
	fprintf(f, "\t/headx headx Hz1 round def\n");
	fprintf(f, "\t/heady heady Hz2 round def\n");
	fprintf(f, "\t/dx headx tailx sub def\n");
	fprintf(f, "\t/dy heady taily sub def\n");
	fprintf(f, "\t/len dx dy dist def\n");
	fprintf(f, "\t/ang dy dx atan def\n");
	fprintf(f, "\t/hb len 20 sub def\n");
	fprintf(f, "\t/th 1 def\n");
	fprintf(f, "\t/ht 3 def\n");

	fprintf(f, "\t/smtx mtrx currentmatrix def\n");
	fprintf(f, "\tnewpath tailx taily translate ang rotate\n");
	fprintf(f, "\t0 th moveto hb th lineto hb ht lineto len 0 lineto\n");
	fprintf(f, "\thb ht neg lineto hb th neg lineto 0 th neg lineto\n");
	fprintf(f, "\tclosepath fill\n");
	fprintf(f, "\tsmtx setmatrix\n");
	fprintf(f, "\tend\n} def\n");
	fprintf(f, "\n");

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

	/* Draw the actual vowels */
	for (g = p->head; g; g = g->next)
		psgroup(g, f);
	fprintf(f, "\n");

	/* Display the group labels */
	fprintf(f, "F1min 50 add Hz1 F2max 100 sub Hz2 translate\n");
	fprintf(f, "/Charis-SIL findfont 15 scalefont setfont\n");
	for (g = p->head; g; g = g->next) {
		psrgb(f, g->color);
		fprintf(f, "0 20 neg translate 0 0 moveto\n");
		fprintf(f, "(%s) show\n", g->label);
	}
	fprintf(f, "\n");

	fprintf(f, "showpage\n");
	fprintf(f, "%%%%Trailer\n");
	fprintf(f, "%%%%Pages: 1\n");
	fprintf(f, "%%%%EOF\n");

	return 0;
}
