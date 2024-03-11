#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "point.h"
#include "vowel.h"
#include "group.h"
#include "stats.h"

static void
gpoint(struct group* g, int i)
{
	float G1 = 0;
	float G2 = 0;
	struct vowel* v;
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	if (g->head->V[i] == NULL)
		return;
	for (v = g->head; v && v->V[i]; v = v->next) {
		G1 += v->V[i]->F1;
		G2 += v->V[i]->F2;
	}
	G1 /= g->size;
	G2 /= g->size;
	g->G[i] = mkpoint((int) G1, (int) G2);
}

void
gravity(struct group* g)
{
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	gpoint(g, 0);
	gpoint(g, 1);
}

static struct matrix*
matrix(struct group* g)
{
	struct vowel* v;
	struct matrix* m;
	m = calloc(1, sizeof(struct matrix));
	for (v = g->head; v ; v = v->next) {
	m->M[0][0] += (v->V[0]->F1-g->G[0]->F1)*(v->V[0]->F1-g->G[0]->F1);
	m->M[0][1] += (v->V[0]->F1-g->G[0]->F1)*(v->V[0]->F2-g->G[0]->F2);
	m->M[1][1] += (v->V[0]->F2-g->G[0]->F2)*(v->V[0]->F2-g->G[0]->F2);
	}
	m->M[0][0] /= g->size;
	m->M[0][1] /= g->size;
	m->M[1][1] /= g->size;
	/*
	printf("M is \n%f\t%f\n%f\t%f\n\n",
		m->M[0][0], m->M[0][1], m->M[0][1], m->M[1][1]);
	*/
	return m;
}

static float
norm(float* e)
{
	return sqrt(e[0] * e[0] + e[1] * e[1]);
}

static void
eivals(struct matrix* m)
{
	float a = m->M[0][0];
	float b = m->M[0][1];
	float d = m->M[1][1];
	float B = (a + d);
	float D = B*B - 4 * (a*d-b*b);
	m->l[0] = 0.5 * (B + sqrt(D));
	m->l[1] = 0.5 * (B - sqrt(D));
}

static void
eivect(struct matrix* m)
{
	int i;
	for (i = 0; i < 2; i++) {
		/*
		printf("%f\t%f\n%f\t%f\n",
			m->M[0][0] - m->l[i],
			m->M[0][1], m->M[0][1],
			m->M[1][1] - m->l[i]);
		*/
		m->e[i][0] = +1.0 *  m->M[0][1];
		m->e[i][1] = -1.0 * (m->M[0][0] - m->l[i]);
		/*printf("(%f, %f)\n", m->e[i][0], m->e[i][1]);*/
		m->l[i] /= norm(m->e[i]);
		m->e[i][0] *= m->l[i];
		m->e[i][1] *= m->l[i];
		/*printf("(%f, %f)\n", m->e[i][0], m->e[i][1]);*/
	}
}

void
ellipse(struct group* g)
{
	struct matrix* m;
	m = matrix(g);
	eivals(m);
	eivect(m);
	if (g->G[0]) {
		g->E[0] = calloc(1, sizeof(struct cloud));
		g->E[0]->a = mkpoint(m->e[0][0], m->e[0][1]);
		g->E[0]->b = mkpoint(m->e[1][0], m->e[1][1]);
		/*
		printf("(%d,%d)\n", g->E[0]->a->F1, g->E[0]->a->F2);
		printf("(%d,%d)\n", g->E[0]->b->F1, g->E[0]->b->F2);
		*/
	}
}
