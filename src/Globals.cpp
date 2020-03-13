#include "pch.h"
#include "Globals.h"

// GLOBAL VARIABLES
int showGrid = 0;
int evalPoint = 500;
int curveMode = 1;


// POINTS HANDLING
vector<Point2D> points;
int insertionMode;
int selectedPoint = -1;

// HERMITE
vector<TensionBiasContinuity> parameters;

// SPLINE
int order = 4;
int parametrization = 0;
int dim = 0;
int coincidence = 1;
float* nodes;

// TANGENTS
int tangentMode = 0;
int chosenTBC = 0;

// WEIGHTS
int weightMode = 0;
vector<float> weights;

// MULTIPLICITIES
int multiplicityMode = 0;
int multiplicity = 0;
vector<int> multiplicities;

// UTILS
string intToString(int value) {
	stringstream strstream;
	strstream << value;
	return strstream.str();
}

string floatToString(float value) {
	stringstream strstream;
	strstream << value;
	return strstream.str();
}

void bitmapOutput(int x, int y, int z, string str, void *font)
{
	glRasterPos3f(float(x), float(y), 0);
	for (unsigned i = 0; i < str.length(); i++)
		glutBitmapCharacter(font, str[i]);
}