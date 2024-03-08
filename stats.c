#include <stdint.h>
#include <stdio.h>

#include "vowel.h"
#include "group.h"

float
cov(struct group* g, int i, int j)
{
	/*
	float sum;
	struct vowel* v;
	for (sum = 0, v = g->head; v ; v = v->next)
		sum += (v->F[i] - g->grav[i]) * (v->F[j] - g->grav[j]);
	return sum / g->size;
	*/
	return 0;
}

void
matrix(struct group* g)
{
	int i, j;
	float M[2][2];
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			M[i][j] = cov(g, i, j);
			printf("M(%d, %d) = %f\n", i, j, M[i][j]);
		}
	}
}
