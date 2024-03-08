#ifndef _POINT_H_
#define _POINT_H_

#include <stdint.h>

struct point {
	int F1;
	int F2;
};

struct point* mkpoint(int, int);
struct point* getpoint(char**);

#endif

