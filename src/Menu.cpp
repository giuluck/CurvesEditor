#include "pch.h"
#include "Menu.h"

void resetValues() {
	selectedPoint = -1;
	tangentMode = 0;
	weightMode = 0;
	multiplicityMode = 0;
}

void modeMenu(int n) {
	resetValues();
	insertionMode = n;
	glutPostRedisplay();
}

void parametrizationMenu(int n) {
	parametrization = n;
	glutPostRedisplay();
}

void hermiteMenu(int n) {
	resetValues();
	curveMode = 1;
	tangentMode = n;
	glutPostRedisplay();
}

void bezierMenu(int n) {
	resetValues();
	curveMode = 2;
	weightMode = n;
	glutPostRedisplay();
}

void splineMenu(int n) {
	resetValues();
	curveMode = 3;
	switch (n) {
	case 1:
		coincidence = 1 - coincidence;
		break;
	case 2:
		multiplicityMode = 1 - multiplicityMode;
		break;
	case 3:
		weightMode = 1 - weightMode;
		break;
	}
	glutPostRedisplay();
}

void menu(int value) {
	if (value == 0) {
		showGrid = 1 - showGrid;
	}
}