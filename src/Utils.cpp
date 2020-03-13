#include "pch.h"
#include "Utils.h"

void uniformParametrization(float *t) {
	float step = 1.0f / float(points.size() - 1);
	for (unsigned int i = 0; i < points.size(); i++) {
		t[i] = i * step;
	}
}

void chordLenghtParametrization(float *t) {
	t[0] = 0;
	for (unsigned int i = 1; i < points.size(); i++) {
		float distx = points.at(i).x - points.at(i - 1).x;
		float disty = points.at(i).y - points.at(i - 1).y;
		t[i] = t[i - 1] + sqrt((distx*distx) + (disty*disty));
	}
	for (unsigned int i = 1; i < points.size(); i++) {
		t[i] = t[i] / t[points.size() - 1];
	}
}

void extendedNodalPartition(float* t, float* Nodi) {
	int i;
	int k = points.size() - order;
	int cont = 2;

	if (coincidence == 0) {
		Nodi[order - 1] = 0;
		float stepN = t[2] - t[1];
		for (int i = order - 2; i >= 0; i--)
			Nodi[i] = Nodi[i + 1] - stepN;
	} else {
		for (int i = 0; i < order; i++)
			Nodi[i] = 0;
	}

	for (i = order; i < order + k; i++) {
		Nodi[i] = t[cont];
		cont++;
	}

	if (coincidence == 0)
	{
		float stepN = t[2] - t[1];
		Nodi[order + k] = 1;
		for (int i = order + k + 1; i < 2 * order + k; i++)
			Nodi[i] = Nodi[i - 1] + stepN;
	} else {
		for (int i = order + k; i < 2 * order + k; i++)
			Nodi[i] = 1;
	}

	if (curveMode == 3 && int(points.size()) > 2 * (order - 1)) {

		for (i = 0; i < order; i++) {
			Nodi[i] = 0;
		}

		int cont = order;
		for (int i = 2; i < int(points.size()) - 2; i++) {
			for (int j = 0; j < multiplicities.at(i); j++) {
				Nodi[cont] = t[i];
				cont++;
			}
		}

		for (int i = 0; i < order; i++) {
			Nodi[cont + i] = 1;
		}
	}
}

int locateInternodalInterval(float t, float *Nodi) {
	int a = order - 1;
	int b;

	if (curveMode == 3) {
		b = multiplicity;
	} else {
		b = points.size();
	}

	while (b - a > 1) {
		int c = (a + b) / 2;
		if (t < Nodi[c]) {
			b = c;
		} else {
			a = c;
		}
	}

	return a;
}

void buildCurve(int l, float* Nodi, float punto, float *v, int dim) {
	float d1, d2, beta;
	for (int i = 0; i < dim; i++) {
		v[i] = 0;
	}

	v[l] = 1;

	for (int i = 0; i < order - 1; i++) {
		float tmp = 0.0;

		for (int j = l - i; j <= l; j++) {
			d1 = punto - Nodi[j];
			d2 = Nodi[i + j + 1] - punto;
			beta = v[j] / (d1 + d2);
			v[j - 1] = d2 * beta + tmp;
			tmp = d1 * beta;
		}
		v[l] = tmp;
	}
}