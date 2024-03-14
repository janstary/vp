#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "point.h"
#include "vowel.h"
#include "group.h"
#include "stats.h"

struct matrix {
	float M[2][2];  /* the matrix */
	float e[2][2];  /* eigenvectors */
	float l[2];     /* eigenvalues */
};

void
gravity(struct group* g)
{
	int i = 0;
	float G1, G2;
	struct vowel* v;
	if (g == NULL)
		return;
	if (g->size == 0)
		return;
	for (i = 0; i < 2; i++) {
		if (g->head->V[i] == NULL)
			continue;
		for (G1 = G2 = 0, v = g->head; v && v->V[i]; v = v->next) {
			G1 += v->V[i]->F1;
			G2 += v->V[i]->F2;
		}
		G1 /= g->size;
		G2 /= g->size;
		/* The formant values are in the hundreds or thousands.
		 * Rounding them to the nearest integer is no big deal. */
		g->G[i] = mkpoint((int) G1, (int) G2);
		/*printf("%d %d\n", g->G[i]->F1, g->G[i]->F2);*/
	}
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
	float B, D;
	if (b == 0) {
		/* diagonal */
		m->l[0] = a;
		m->l[1] = d;
	} else {
		B = (a + d);
		D = B*B - 4 * (a*d-b*b);
		m->l[0] = 0.5 * (B + sqrt(D));
		m->l[1] = 0.5 * (B - sqrt(D));
	}
	/*printf("eivals %f %f\n", m->l[0], m->l[1]);*/
}

static void
eivect(struct matrix* m)
{
	int i;
	if (m->M[0][1] == 0) {
		/* diagonal */
		for (i = 0; i < 2; i++) {
			m->e[i][0] = m->M[i][0];
			m->e[i][1] = m->M[i][1];
		}
	} else {
		for (i = 0; i < 2; i++) {
			/*
			printf("%f\t%f\n%f\t%f\n",
				m->M[0][0] - m->l[i],
				m->M[0][1], m->M[0][1],
				m->M[1][1] - m->l[i]);
			*/
			m->e[i][0] = +1.0 *  m->M[0][1];
			m->e[i][1] = -1.0 * (m->M[0][0] - m->l[i]);
		}
	}
	/*printf("eigen (%f, %f)\n", m->e[i][0], m->e[i][1]);*/
	/* FIXME scale to cover the actual range */
}

static struct matrix*
matrix(struct group* g, int i)
{
	struct vowel* v;
	struct matrix* m;
	m = calloc(1, sizeof(struct matrix));
	for (v = g->head; v ; v = v->next) {
m->M[0][0] += (v->V[i]->F1-g->G[i]->F1)*(v->V[i]->F1-g->G[i]->F1);
m->M[0][1] += (v->V[i]->F1-g->G[i]->F1)*(v->V[i]->F2-g->G[i]->F2);
m->M[1][1] += (v->V[i]->F2-g->G[i]->F2)*(v->V[i]->F2-g->G[i]->F2);
	}
	m->M[0][0] /= g->size;
	m->M[0][1] /= g->size;
	m->M[1][1] /= g->size;
	/*
	printf("matrix\n%f\t%f\n%f\t%f\n",
		m->M[0][0], m->M[0][1], m->M[0][1], m->M[1][1]);
	*/
	eivals(m);
	eivect(m);
	return m;
}

void
ellipse(struct group* g)
{
	int i;
	struct matrix* m;
	for (i = 0; i < 2; i++) {
		if (g->G[i] == NULL)
			continue;
		m = matrix(g, i);
		g->E[i] = calloc(1, sizeof(struct cloud));
		/* FIXME: scale to actual range */
		g->E[i]->minor = 0.7 * (g->max->F1 - g->min->F1);
		g->E[i]->major = 0.7 * (g->max->F2 - g->min->F2);
		g->E[i]->angle = atan(m->e[0][1] / m->e[0][0]);
		g->E[i]->angle *= 180 / M_PI;
		/*
		printf("angle %f\n", g->E[i]->angle);
		printf("major %f\n", g->E[i]->major);
		printf("minor %f\n", g->E[i]->minor);
		m->e[0][0] /= norm(m->e[0]);
		m->e[0][1] /= norm(m->e[0]);
		m->e[0][0] *= 0.5 * (g->max->F1 - g->min->F1);
		m->e[0][1] *= 0.5 * (g->max->F1 - g->min->F1);
		m->e[1][0] /= norm(m->e[1]);
		m->e[1][1] /= norm(m->e[1]);
		m->e[1][0] *= 0.5 * (g->max->F2 - g->min->F2);
		m->e[1][1] *= 0.5 * (g->max->F2 - g->min->F2);
		*/
		free(m);
	}
}
