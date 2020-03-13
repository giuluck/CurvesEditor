#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <sstream>
#include <vector>

using namespace std;

constexpr double PI = 3.14159265;

typedef struct { 
	float x;
	float y; 
} Point2D;

typedef struct {
	float t;
	float b;
	float c;
} TensionBiasContinuity;

// GLOBAL VARIABLES
extern int showGrid;
extern int evalPoint;
extern int curveMode;

// POINTS HANDLING
extern vector<Point2D> points;
extern int insertionMode;
extern int selectedPoint;

// HERMITE
extern vector<TensionBiasContinuity> parameters;

// SPLINE
extern int order;
extern int parametrization;
extern int dim;
extern int coincidence;
extern float* nodes;

// TANGENTS
extern int tangentMode;
extern int chosenTBC;

// WEIGHTS
extern int weightMode;
extern vector<float> weights;

// MULTIPLICITIES
extern int multiplicityMode;
extern int multiplicity;
extern vector<int> multiplicities;

// UTILS
string intToString(int value);
string floatToString(float value);
void bitmapOutput(int x, int y, int z, string str, void *font);