#include <stdint.h>
#include <stdio.h>

static uint32_t colors[] = {
	0x30123b,
	0x466be3,
	0x28bceb,
	0x32f298,
	0xa4fc3c,
	0xeecf3a,
	0xfb7e21,
	0xd02f05,
	0x7a0403,
};

#define numcol (sizeof(colors) / sizeof(uint32_t))

uint32_t
rgb(unsigned c)
{
	return colors[c % numcol];
}
