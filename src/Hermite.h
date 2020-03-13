#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Globals.h"
#include "Utils.h"

#define PHI0(t) ( 2.0f * t * t * t - 3.0f * t * t + 1.0f )
#define PHI1(t) ( 1.0f * t * t * t - 2.0f * t * t + 1.0f * t )
#define PSI0(t)  ( -2.0f * t * t * t + 3.0f * t * t )
#define PSI1(t)  ( 1.0f * t * t * t - 1.0f * t * t )

void modifyTCBParameters(bool up);
void drawHermiteCurve();
