#include <stdint.h>
#include <stdio.h>

#include "vowel.h"
#include "group.h"

void
matrix(struct group* g)
{
	struct vowel* v;
	float M[2][2] = { { 0, 0}, {0, 0} };
	for (v = g->head; v ; v = v->next) {
		M[0][0] +=
		(v->V[0]->F1 - g->G[0]->F1) * (v->V[0]->F1 - g->G[0]->F1);
		M[0][1] +=
		(v->V[0]->F1 - g->G[0]->F1) * (v->V[0]->F2 - g->G[0]->F2);
		M[1][1] +=
		(v->V[0]->F2 - g->G[0]->F2) * (v->V[0]->F2 - g->G[0]->F2);
	}
	M[0][0] /= g->size;
	M[0][1] /= g->size;
	M[1][0]  = M[0][1];
	M[1][1] /= g->size;
	/*
	printf("%f\t%f\n%f\t%f\n", M[0][0], M[0][1], M[1][0], M[1][1]);
	*/
}
