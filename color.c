#include <stdint.h>
#include <stdio.h>

/* A vowel plot consists of N groups of vowels.
 * These groups should be distinguished by colors.
 * What N colors should that be? (see e.g. gnuplot)
 * A simple way is to cut up the visible spectrum into N parts.
 * Graphics output such as PS uses RGB for collor specification,
 * so here is a translation of a wavelength to RGB.
 * Technically, the input is a fraction of the visible spectrum
 * normalized to [0,1], so that callers can simply say group/N;
 * the RGB output is three bytes of the returned uint32_t. */

uint32_t
rgb(float c)
{
	/* FIXME */
	return
		((uint32_t) (0x0000ff * c/2.0)) |
		((uint32_t) (0x00ff00 * c/2.5)) |
		((uint32_t) (0xff0000 * c/3.0));
}

/* FIXME Also have a scale of gray,
 * and let vp(1) recognize -m for monotone */
uint32_t
gray(float c)
{
	return
		((uint32_t) (0x0000ff * c)) |
		((uint32_t) (0x00ff00 * c)) |
		((uint32_t) (0xff0000 * c));
}
