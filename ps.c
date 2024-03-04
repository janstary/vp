#include <stdint.h>
#include <stdio.h>

#include "vowel.h"
#include "group.h"
#include "vplot.h"
#include "color.h"
#include "ps.h"

/* Embed the Charis or Doulos font in the resulting PS file
 * in case the system does not have them. If possible,
 * embed just the characters we actualy use. */

/*
FIXME landscape A4, or a BBox of the actual range
FIXME origin at the NE corner, axes going down and left
FIXME axes and ticks in Hz and Bark
*/

static void
psvow(struct vowel* v, FILE* f)
{
	if (v == NULL)
		return;
	fprintf(f, "(%s) ", v->label ? v->label: "");
	fprintf(f, "%4u %4u ", v->F1, v->F2);
	if (v->G1 && v->G2)
		fprintf(f, "%4u %4u arrow", v->G1, v->G2);
	else
		fprintf(f, "dot");
	fprintf(f, "\n");
}

static void
psgroup(struct group* g, FILE* f, uint32_t rgb)
{
	struct vowel* v;
	if (g == NULL)
		return;
	fprintf(f, "\n");
	if (g->label)
		fprintf(f, "%% %s\n", g->label);
	fprintf(f, "%f %f %f setrgbcolor\n",
		((rgb & 0x000000ff) >>  0) / 255.0,
		((rgb & 0x0000ff00) >>  8) / 255.0,
		((rgb & 0x00ff0000) >> 16) / 255.0);
	for (v = g->head; v; v = v->next)
		psvow(v, f);
}

int
pswrite(struct vplot* p, FILE* f)
{
	unsigned n = 0;
	struct group* g = NULL;
	if (p == NULL || f == NULL)
		return -1;

	/* PS itself will do the scaling, given the limits.
	 * Also, we can simply translate back down to F1 neg F2 neg
	 * and have the dots be placed inside the actual range. */

	fprintf(f, "/F1min { %4u } def\n", p->F1min);
	fprintf(f, "/F1max { %4u } def\n", p->F1max);
	fprintf(f, "/F2min { %4u } def\n", p->F2min);
	fprintf(f, "/F2max { %4u } def\n", p->F2max);
	fprintf(f, "\n");

	/* Compute the ratio of Hz to paper/bbox size
	 * so that we can use the Hz coordinates. */

	fprintf(f, "/mm  { 72 mul 258 div 10 mul } def\n");
	fprintf(f, "/Hz1 { 210 mm F1max F1min sub div mul } def\n");
	fprintf(f, "/Hz2 { 297 mm F2max F2min sub div mul } def\n");
	fprintf(f, "/hzw { F1max F1min sub Hz1 } def\n");
	fprintf(f, "/hzh { F2max F2min sub Hz2 } def\n");
	fprintf(f, "\n");

	/* the plotting routines */

	fprintf(f, "/dist { dup mul exch dup mul add sqrt } def\n");
	fprintf(f, "\n");

	fprintf(f, "/dot {\n");
	fprintf(f, "\tHz2 round exch\n");
	fprintf(f, "\tHz1 round exch\n");
	fprintf(f, "\texch dup 3 1 roll\n");
	fprintf(f, "\texch dup 3 1 roll\n");
	fprintf(f, "\tnewpath 5 0 360 arc closepath fill\n");
	fprintf(f, "\tmoveto 6 -6 rmoveto\n");
	fprintf(f, "\tshow\n} def\n");
	fprintf(f, "\n");

	fprintf(f, "/adict 20 dict def\n");
	fprintf(f, "adict begin /mtrx matrix def end\n");
	fprintf(f, "/arrow {\n\tadict begin\n");
	fprintf(f, "\t/heady exch def /headx exch def\n");
	fprintf(f, "\t/taily exch def /tailx exch def\n");
	fprintf(f, "\ttailx taily dot\n");

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

	fprintf(f, "/smtx mtrx currentmatrix def\n");
	fprintf(f, "\tnewpath tailx taily translate ang rotate\n");
	fprintf(f, "0 th moveto hb th lineto hb ht lineto len 0 lineto\n");
	fprintf(f, "hb ht neg lineto hb th neg lineto 0 th neg lineto\n");
	fprintf(f, "closepath fill\n");
	fprintf(f, "\tsmtx setmatrix\n");
	fprintf(f, "\tend\n} def\n");
	fprintf(f, "\n");

	/* FIXME make sure we have the glyphs */
	fprintf(f, "/Charis-SIL findfont ");
	fprintf(f, "10 Hz1 scalefont ");
	fprintf(f, "setfont\n");
	fprintf(f, "\n");

	/* axes and ticks  */
	/* FIXME Hz and Bark */

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
	for (n = 0, g = p->head; g; n++, g = g->next)
		psgroup(g, f, rgb((1.0 * (n+1)) / p->size));

	/* Display the group labels */
	fprintf(f, "F1min Hz1 F2max Hz2 translate\n");
	fprintf(f, "/Charis-SIL findfont 20 Hz1 scalefont setfont\n");
	/*
	50 -50 20 1 mul sub moveto 0.57 0.10 0.20 setrgbcolor (i) show
	50 -50 20 2 mul sub moveto 0.13 0.20 0.40 setrgbcolor (e) show
	50 -50 20 3 mul sub moveto 0.70 0.30 0.60 setrgbcolor (goat) show
	*/

	fprintf(f, "\nshowpage\n");
	return 0;
}
