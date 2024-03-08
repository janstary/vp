#include <stdint.h>
#include <stdio.h>

static uint32_t colors[] = {
	0x1B9E77,
	0xD95F02,
	0x7570B3,
	0xE7298A,
	0x66A61E,
	0xE6AB02,
	0xA6761D,
	0x666666,
};

#define numcol (sizeof(colors) / sizeof(uint32_t))

uint32_t
rgb(int c)
{
	return colors[c % numcol];
}
