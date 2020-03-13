#include "pch.h"
#include "Bezier.h"

void bezierInterpolation(float *cx, float *cy) {
	float tstep = 1.0f / float(evalPoint - 1);
	float tg;
	int k = 0;
	Point2D *c = new Point2D[points.size()];
	float *w = new float[points.size()];

	for (tg = 0; tg <= 1 + tstep; tg += tstep) {

		for (unsigned int i = 0; i < points.size(); i++) {
			c[i].x = weights.at(i)*points.at(i).x;
			c[i].y = weights.at(i)*points.at(i).y;
			w[i] = weights.at(i);
		}

		for (unsigned int j = 1; j < points.size(); j++) {
			for (unsigned int i = 0; i < points.size() - j; i++) {
				c[i].x = c[i].x*(1 - tg) + tg*c[i + 1].x;
				c[i].y = c[i].y*(1 - tg) + tg*c[i + 1].y;
				w[i] = w[i] * (1 - tg) + tg*w[i + 1];
			}

		}
		cx[k] = c[0].x / w[0];
		cy[k] = c[0].y / w[0];
		k++;
	}
}

void bernsteinEvaluation(float ti, float*b)
{
	for (unsigned int i = 0; i <= points.size(); i++)
		b[i] = 0;

	b[points.size()] = 1.0;

	for (unsigned int i = 1; i < points.size(); i++)
	{
		float d1b = 0;
		for (unsigned int j = 0; j < points.size(); j++)
		{
			b[j] = (1 - ti)*b[j + 1] + d1b;
			d1b = ti*b[j + 1];
		}
		b[points.size()] = d1b;
	}

}

void drawBernsteinPolys() {
	int i;
	float dtg = 1.0f / float(300 - 1);
	float tg = 0;
	float *somme = new float[300];

	float **A = new float*[300];
	for (i = 0; i < 300; i++) {
		A[i] = new float[points.size() + 1];
	}

	for (int k = 0; k < 300; k++, tg += dtg) {
		somme[k] = 0;
		bernsteinEvaluation(tg, A[k]);

		for (unsigned int i = 0; i < points.size(); i++) {
			somme[k] += weights.at(i)*A[k][i + 1];
		}
	}

	for (unsigned int j = 0; j < points.size(); j++) {
		for (unsigned int k = 0; k < 300; k++) {
			A[k][j] = (weights.at(j)*A[k][j + 1]) / somme[k];
		}
	}

	for (unsigned int j = 0; j <= points.size(); j++) {
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

void drawBezierCurve() {
	float* cx = new float[evalPoint];
	float* cy = new float[evalPoint];

	bezierInterpolation(cx, cy);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < evalPoint; i++) {
		glVertex3d(cx[i], cy[i], 0);
	}
	glEnd();

	delete[] cx;
	delete[] cy;
}