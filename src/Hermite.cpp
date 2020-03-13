#include "pch.h"
#include "Hermite.h"

int dx(int i, float* t, float tens, float bias, float cont) {
	if (i == 0) {
		return int(0.5*(1 - tens)*(1 - bias)*(1 - cont)*(points.at(i + 1).x - points.at(i).x) / (t[i + 1] - t[i]));
	}
	if (i == points.size() - 1) {
		return int(0.5*(1 - tens)*(1 + bias)*(1 + cont)*(points.at(i).x - points.at(i - 1).x) / (t[i] - t[i - 1]));
	}
	if (i % 2 == 0) {
		return int(0.5*(1 - tens)*(1 + bias)*(1 + cont)*(points.at(i).x - points.at(i - 1).x) / (t[i] - t[i - 1]) +
			0.5*(1 - tens)*(1 - bias)*(1 - cont)*(points.at(i + 1).x - points.at(i).x) / (t[i + 1] - t[i]));
	}
	else {
		return int(0.5*(1 - tens)*(1 + bias)*(1 - cont)*(points.at(i).x - points.at(i - 1).x) / (t[i] - t[i - 1]) +
			0.5*(1 - tens)*(1 - bias)*(1 + cont)*(points.at(i + 1).x - points.at(i).x) / (t[i + 1] - t[i]));
	}
}

int dy(int i, float* t, float tens, float bias, float cont) {
	if (i == 0) {
		return int(0.5*(1 - tens)*(1 - bias)*(1 - cont)*(points.at(i + 1).y - points.at(i).y) / (t[i + 1] - t[i]));
	}
	if (i == points.size() - 1) {
		return int(0.5*(1 - tens)*(1 + bias)*(1 + cont)*(points.at(i).y - points.at(i - 1).y) / (t[i] - t[i - 1]));
	}
	if (i % 2 == 0) {
		return int(0.5*(1 - tens)*(1 + bias)*(1 + cont)*(points.at(i).y - points.at(i - 1).y) / (t[i] - t[i - 1]) +
			0.5*(1 - tens)*(1 - bias)*(1 - cont)*(points.at(i + 1).y - points.at(i).y) / (t[i + 1] - t[i]));
	}
	else {
		return int(0.5*(1 - tens)*(1 + bias)*(1 - cont)*(points.at(i).y - points.at(i - 1).y) / (t[i] - t[i - 1]) +
			0.5*(1 - tens)*(1 - bias)*(1 + cont)*(points.at(i + 1).y - points.at(i).y) / (t[i + 1] - t[i]));
	}
}

void hermiteInterpolation(float* t, float* px, float* py) {
	float passotg = 1.0f / float(evalPoint - 1);
	float tg, tgmapp, ampiezza;
	int index = 0;
	int is = 0;
	for (tg = 0; tg <= 1; tg += passotg) {

		if (tg > t[is + 1]) {
			is++;
		}
		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		TensionBiasContinuity actualParameters = { parameters.at(is).t + parameters.at(is + 1).t, parameters.at(is).b + parameters.at(is + 1).b, parameters.at(is).c + parameters.at(is + 1).c };

		px[index] = float(points.at(is).x * PHI0(tgmapp) +
					  dx(is, t, actualParameters.t, actualParameters.c, actualParameters.b) * PHI1(tgmapp) * ampiezza +
					  points.at(is + 1).x * PSI0(tgmapp) +
					  dx(is + 1, t, actualParameters.t, actualParameters.c, actualParameters.b) * PSI1(tgmapp) * ampiezza);
		py[index] = float(points.at(is).y * PHI0(tgmapp) +
					  dy(is, t, actualParameters.t, actualParameters.c, actualParameters.b) * PHI1(tgmapp) * ampiezza +
					  points.at(is + 1).y * PSI0(tgmapp) +
					  dy(is + 1, t, actualParameters.t, actualParameters.c, actualParameters.b) * PSI1(tgmapp) * ampiezza);
		index++;
	}

	if (tangentMode == 1) {
		Point2D p1, p2;

		for (unsigned int i = 0; i < points.size(); i++) {
			p1.x = points[i].x + dx(i, t, parameters.at(i).t, parameters.at(i).b, parameters.at(i).c) / 15;
			p1.y = points[i].y + dy(i, t, parameters.at(i).t, parameters.at(i).b, parameters.at(i).c) / 15;

			p2.x = points[i].x - dx(i, t, parameters.at(i).t, parameters.at(i).b, parameters.at(i).c) / 15;
			p2.y = points[i].y - dy(i, t, parameters.at(i).t, parameters.at(i).b, parameters.at(i).c) / 15;

			glBegin(GL_LINE_LOOP);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(p1.x, p1.y, 0.0);
			glVertex3f(p2.x, p2.y, 0.0);
			glColor3f(1.0, 0.0, 0.0);
			glEnd();
		}
	}
}

void modifyTCBParameters(bool up) {
	if (chosenTBC == 0) {
		if (up && parameters.at(selectedPoint).t < 1.5) {
			parameters.at(selectedPoint).t += 0.125;
		}
		else if (!up && parameters.at(selectedPoint).t > -1.5) {
			parameters.at(selectedPoint).t -= 0.125;
		}
	}
	else if (chosenTBC == 1) {
		if (up && parameters.at(selectedPoint).c < 1.5) {
			parameters.at(selectedPoint).c += 0.125;
		}
		else if (!up && parameters.at(selectedPoint).c > -1.5) {
			parameters.at(selectedPoint).c -= 0.125;
		}
	}
	else if (chosenTBC == 2) {
		if (up && parameters.at(selectedPoint).b < 1.5) {
			parameters.at(selectedPoint).b += 0.125;
		}
		else if (!up && parameters.at(selectedPoint).b > -1.5) {
			parameters.at(selectedPoint).b -= 0.125;
		}
	}
}

void drawHermiteCurve() {
	float *t = new float[points.size()];

	if (parametrization == 0) {
		uniformParametrization(t);
	} else {
		chordLenghtParametrization(t);
	}

	float* px = new float[evalPoint];
	float* py = new float[evalPoint];

	hermiteInterpolation(t, px, py);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < evalPoint-1; i++) {
		glVertex3d(px[i], py[i], 0);
	}
	glEnd();

	delete[]px;
	delete[]py;
}