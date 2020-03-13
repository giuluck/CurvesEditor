#include "pch.h"
#include "Spline.h"

void DeBoor(float *t, float *Nodi, float *sx, float *sy) {
	float tstep = 1.0f / float(evalPoint - 1);
	float tg;
	int l, i = 0, j, ti, tmj;
	int k = 0;
	Point2D *c = new Point2D[multiplicity];
	float *w = new float[multiplicity];

	int cont = 0;
	c[cont].x = weights.at(i)*points.at(i).x;
	c[cont].y = weights.at(i)*points.at(i).y;
	w[cont] = weights.at(i);
	for (tg = 0; tg <= 1.0; tg += tstep) {
		l = locateInternodalInterval(tg, Nodi);
		cont = 0;
		for (i = 1; i < int(points.size()); i++) {
			for (int j = 0; j < multiplicities.at(i - 1); j++) {
				cont++;
				c[cont].x = weights.at(i)*points.at(i).x;
				c[cont].y = weights.at(i)*points.at(i).y;
				w[cont] = weights.at(i);
			}
		}

		//ciclo che varia i livelli
		for (j = 1; j < order; j++) {			//Questo ciclo varia sui LIVELLI
			for (i = l - order + j + 1; i <= l; i++) {			//Questo ciclo va sui nodi di una riga, in tutto m-j
				ti = i;
				tmj = i + order - j;
				float den = Nodi[tmj] - Nodi[ti];

				c[i].x = ((c[i].x * (tg - Nodi[ti])) + (c[i - 1].x*(Nodi[tmj] - tg))) / (den);
				c[i].y = ((c[i].y * (tg - Nodi[ti])) + (c[i - 1].y*(Nodi[tmj] - tg))) / (den);
				w[i] = (w[i] * (tg - Nodi[ti]) + w[i - 1] * (Nodi[tmj] - tg)) / den;
			}
		}

		sx[k] = c[l].x / w[l];   //Se in De Casteljeau  si prendeva la prima cella, in De Boor si prende l'ultima
		sy[k] = c[l].y / w[l];
		k++;
	}
}

void drawSplinePolys() {

	float dtg = 1.0f / 299.0f;
	float tg = 0;
	float *somme = new float[300];

	//Dichiarazione matrice
	float **A = new float*[300];
	for (int i = 0; i < 300; i++) {
		A[i] = new float[dim];
	}

	for (int k = 0; k < 300; k++, tg += dtg) {
		somme[k] = 0;
		int l = locateInternodalInterval(tg, nodes);
		buildCurve(l, nodes, tg, A[k], dim);

		int cont = 0;
		for (int i = 0; i < dim; i++) {
			if (i != 0 && nodes[i] != nodes[i - 1]) {
				cont++;
			}
			somme[k] += weights.at(cont)*A[k][i];
		}
	}

	int cont = 0;
	for (int j = 0; j < dim; j++) {
		if (j != 0 && nodes[j] != nodes[j - 1]) {
			cont++;
		}
		for (int k = 0; k < 300; k++) {
			A[k][j] = (weights.at(cont)*A[k][j]) / somme[k];
		}
	}

	for (int j = 0; j < dim; j++) {
		tg = 0.0;
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k < 300; k++, tg += dtg) {
			float grafX = 100 + tg * 300;
			float grafY = 150 + A[k][j] * 300;
			glVertex3d(grafX, grafY, 0);
		}
		glEnd();
	}
}

void drawSplineCurve() {

	float *t = new float[points.size()];

	if (parametrization == 0) {
		uniformParametrization(t);
	}
	else {
		chordLenghtParametrization(t);
	}

	multiplicity = 0;
	for (unsigned int i = 0; i < multiplicities.size(); i++) {
		multiplicity += multiplicities.at(i);
	}

	dim = multiplicity;
	int dimPartNodEstesa = order + multiplicity;

	nodes = new float[dimPartNodEstesa];
	extendedNodalPartition(t, nodes);

	float *sx = new float[evalPoint];
	float *sy = new float[evalPoint];

	DeBoor(t, nodes, sx, sy);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < evalPoint - 1; i++) {
		glVertex3d(sx[i], sy[i], 0);
	}
	glEnd();

	delete[]sx;
	delete[]sy;
}