// CurvesEditor.cpp

#include "pch.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Menu.h"
#include "Hermite.h"
#include "Bezier.h"
#include "Spline.h"

int width = 500, height = 500;
int editorID, infoID;
int menuID, modeID, parametrizationID, hermiteID, bezierID, splineID;

void createMenu(void) {
	modeID = glutCreateMenu(modeMenu);
	glutAddMenuEntry("Inserisci", 0);
	glutAddMenuEntry("Sposta", 1);
	glutAddMenuEntry("Elimina", 2);

	parametrizationID = glutCreateMenu(parametrizationMenu);
	glutAddMenuEntry("Uniforme", 0);
	glutAddMenuEntry("Lunghezza delle Corde", 1);

	hermiteID = glutCreateMenu(hermiteMenu);
	glutAddMenuEntry("Calcola Curva", 0);
	glutAddMenuEntry("Modifica Tangenti", 1);

	bezierID = glutCreateMenu(bezierMenu);
	glutAddMenuEntry("Calcola Curva", 0);
	glutAddMenuEntry("Modifica Pesi", 1);

	splineID = glutCreateMenu(splineMenu);
	glutAddMenuEntry("Calcola Curva", 0);
	glutAddMenuEntry("Coincidenza Estremi", 1);
	glutAddMenuEntry("Modifica Molteplicita'", 2);
	glutAddMenuEntry("Modifica Pesi", 3);

	menuID = glutCreateMenu(menu);
	glutAddMenuEntry("Disegna Griglia", 0);
	glutAddSubMenu("Modalita' Inserimento", modeID);
	glutAddSubMenu("Parametrizzazione", parametrizationID);
	glutAddSubMenu("Curve di Hermite", hermiteID);
	glutAddSubMenu("Curve di Bezier", bezierID);
	glutAddSubMenu("Curve Spline Approssimanti", splineID);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mouseMotion(int xMouse, int yMouse) {
	Point2D newPoint;
	newPoint.x = float(xMouse);
	newPoint.y = float(height - yMouse);

	if (selectedPoint >= 0) {
		if (insertionMode == 1) {
			points.at(selectedPoint) = newPoint;
		}
	}

	glutPostRedisplay();
}

void mouseInput(int button, int state, int xMouse, int yMouse) {
	Point2D newPoint;
	newPoint.x = float(xMouse);
	newPoint.y = float(height - yMouse);
	float distance;
	float toll = 30;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (insertionMode == 1 || tangentMode == 1 || multiplicityMode == 1 || weightMode == 1) { // SELECTION
			if (points.size() > 0) {
				selectedPoint = 0;
				distance = sqrt((points.at(0).x - newPoint.x)*(points.at(0).x - newPoint.x) + (points.at(0).y - newPoint.y)*(points.at(0).y - newPoint.y));
				for (unsigned int i = 1; i < points.size(); i++) {
					float distanzaProssimoPunto = sqrt((points.at(i).x - newPoint.x)*(points.at(i).x - newPoint.x) + (points.at(i).y - newPoint.y)*(points.at(i).y - newPoint.y));
					if (distanzaProssimoPunto < distance) {
						selectedPoint = i;
						distance = distanzaProssimoPunto;
					}
				}
			}
			if (multiplicityMode == 1 && (selectedPoint == 0 || selectedPoint == (int(points.size() - 1)))) {
				selectedPoint = -1;
			}
			if (distance > toll) {
				selectedPoint = -1;
			}
		} else if (insertionMode == 0) { // INSERTION
			points.push_back(newPoint);
			TensionBiasContinuity newparameters = { 0.0, 0.0, 0.0 };
			parameters.push_back(newparameters);
			multiplicities.push_back(1);
			weights.push_back(1.0);
		} else if (insertionMode == 2) { // DELETION
			if (points.size() > 0) {
				selectedPoint = 0;
				distance = sqrt((points.at(0).x - newPoint.x)*(points.at(0).x - newPoint.x) + (points.at(0).y - newPoint.y)*(points.at(0).y - newPoint.y));
				for (unsigned int i = 1; i < points.size(); i++) {
					float distanzaProssimoPunto = sqrt((points.at(i).x - newPoint.x)*(points.at(i).x - newPoint.x) + (points.at(i).y - newPoint.y)*(points.at(i).y - newPoint.y));
					if (distanzaProssimoPunto < distance) {
						selectedPoint = i;
						distance = distanzaProssimoPunto;
					}
				}
			}
			if (distance > toll) {
				selectedPoint = -1;
				points.pop_back();
				parameters.pop_back();
				multiplicities.pop_back();
				weights.pop_back();
			}
			else {
				points.erase(points.begin() + selectedPoint);
				parameters.erase(parameters.begin() + selectedPoint);
				multiplicities.erase(multiplicities.begin() + selectedPoint);
				weights.erase(weights.begin() + selectedPoint);
			}
		}
	}

	glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}

	if (tangentMode == 1) {
		switch (key) {
		case 't':
			chosenTBC = 0;
			break;
		case 'c':
			chosenTBC = 1;
			break;
		case 'b':
			chosenTBC = 2;
			break;
		}
	}

	if (multiplicityMode == 1 && selectedPoint != -1) {
		switch (key) {
		case '1':
			multiplicities.at(selectedPoint - 1) = 1;
			break;
		case '2':
			multiplicities.at(selectedPoint - 1) = 2;
			break;
		case '3':
			multiplicities.at(selectedPoint - 1) = 3;
			break;
		}
	}
	
	if (weightMode == 1 && selectedPoint != -1) {
		switch (key) {
		case '-':
			weights[selectedPoint] = weights[selectedPoint] > 0.2f ? weights[selectedPoint] - 0.1f : 0.1f;
			break;
		case '+':
			weights[selectedPoint] += 0.1f;
			break;
		}
	}

	glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y) {

	if (tangentMode == 1 && selectedPoint != -1) {
		switch (key) {
		case GLUT_KEY_UP: 
			modifyTCBParameters(true);
			break;
		case GLUT_KEY_DOWN:
			modifyTCBParameters(false);
			break;
		}
	}

	glutPostRedisplay();
}


void drawPolys() {
	if ((curveMode == 1 || curveMode == 2) && points.size() > 2) {
		drawBernsteinPolys();
	} else if (curveMode == 3 && int(points.size()) >= order) {
		drawSplinePolys();
	}
}

void drawScene(void) {
	// EDITOR WINDOW
	glutSetWindow(editorID);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xAAAA);
	
	// GRID
	glLineWidth(1.0);
	if (showGrid) {
		glColor3f(0.3f, 0.3f, 0.3f);
		float dis = 50;
		for (float j = 0; j < width; j += dis) {
			glBegin(GL_LINE_STRIP);
			glVertex3f(j, 0, 0);
			glVertex3f(j, float(height), 0);
			glEnd();
		}
		for (float j = 0; j < height; j += dis) {
			glBegin(GL_LINE_STRIP);
			glVertex3f(0, j, 0);
			glVertex3f(float(width), j, 0);
			glEnd();
		}
	}

	// LINES
	glLineWidth(2.0);
	glLineStipple(1, 0xF0F0);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.3f, 0.3f);
	for (unsigned int i = 0; i < points.size(); i++) {
		glVertex3f(points.at(i).x, points.at(i).y, 0.0);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	// CURVES
	glColor3f(1.0, 0.0, 0.0);
	if (curveMode == 1 && points.size() > 2) {
		drawHermiteCurve();
	} 
	else if (curveMode == 2 && points.size() > 2) {
		drawBezierCurve();
	}
	if (curveMode == 3 && int(points.size()) >= order) {
		drawSplineCurve();
	}

	// POINTS
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.3f, 0.3f);
	for (unsigned int i = 0; i < points.size(); i++) {
		if (selectedPoint == i) {
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(points.at(i).x, points.at(i).y, 0.0f);
			glColor3f(1.0f, 0.3f, 0.3f);
		} else {
			glVertex3f(points.at(i).x, points.at(i).y, 0.0f);
		}
	}
	glEnd();

	// INFO WINDOW
	glutSetWindow(infoID);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	
	string strPoints = "Numero di Punti: ";
	strPoints.append(intToString(int(points.size())));
	bitmapOutput(50, 470, 0, strPoints, GLUT_BITMAP_HELVETICA_18);

	string strCurve = "Tipo di Curva: ";
	switch (curveMode) {
	case 1:
		strCurve.append("Hermite");
		break;
	case 2:
		strCurve.append("Bezier");
		break;
	case 3:
		strCurve.append("Spline");
		break;
	}
	bitmapOutput(250, 470, 0, strCurve, GLUT_BITMAP_HELVETICA_18);

	glColor3f(1.0, 1.0, 0.0);
	drawPolys();

	if (insertionMode == 1 || tangentMode == 1 || weightMode == 1 || multiplicityMode == 1) {
		if (selectedPoint == -1) {
			bitmapOutput(100, 50, 0, "Seleziona un Punto", GLUT_BITMAP_HELVETICA_18);
		}
		else if (insertionMode == 1) {
			bitmapOutput(100, 50, 0, "Sposta il Punto", GLUT_BITMAP_HELVETICA_18);
		} else if (tangentMode == 1) {
			string strT = "T: ";
			strT.append(floatToString(parameters.at(selectedPoint).t));
			bitmapOutput(100, 50, 0, strT, GLUT_BITMAP_HELVETICA_18);
			string strB = "B: ";
			strB.append(floatToString(parameters.at(selectedPoint).b));
			bitmapOutput(200, 50, 0, strB, GLUT_BITMAP_HELVETICA_18);
			string strC = "C: ";
			strC.append(floatToString(parameters.at(selectedPoint).c));
			bitmapOutput(300, 50, 0, strC, GLUT_BITMAP_HELVETICA_18);
		}
		else if (weightMode == 1) {
			string strWeight = "Peso: ";
			strWeight.append(floatToString(weights.at(selectedPoint)));
			bitmapOutput(100, 50, 0, strWeight, GLUT_BITMAP_HELVETICA_18);
		}
		else if (multiplicityMode == 1) {
			string strMultiplicity = "Molteplicita': ";
			strMultiplicity.append(intToString(multiplicities.at(selectedPoint - 1)));
			bitmapOutput(100, 50, 0, strMultiplicity, GLUT_BITMAP_HELVETICA_18);
		}
	} else {
		if (insertionMode == 0) {
			bitmapOutput(100, 50, 0, "Inserisci un Punto", GLUT_BITMAP_HELVETICA_18);
		} else {
			bitmapOutput(100, 50, 0, "Elimina l'Ultimo Punto", GLUT_BITMAP_HELVETICA_18);
		}
	}

	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Main routine.
int main(int argc, char **argv)
{
	GLboolean GlewInitResult;
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(700, 100);
	editorID = glutCreateWindow("Curves Editor");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	glutMouseFunc(mouseInput);
	glutMotionFunc(mouseMotion);
	glutSpecialFunc(specialKeyInput);
	glutKeyboardFunc(keyInput);
	
	createMenu();

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	infoID = glutCreateWindow("Info");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
	glutMainLoop();

	return 0;
}