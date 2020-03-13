#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include "Globals.h"

void uniformParametrization(float *t);
void chordLenghtParametrization(float *t);
void extendedNodalPartition(float* t, float* Nodi);
int locateInternodalInterval(float t, float *Nodi);
void buildCurve(int l, float* Nodi, float punto, float *v, int dim);
