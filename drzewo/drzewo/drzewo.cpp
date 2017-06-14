//*********************************************************************
//
//  PLIK ŹRÓDłOWY:		Source.cpp
//
//  OPIS:				Program służy do rysowania układu odwzorowań iterowanych				
//						(zbioru punktów na płąszczyźnie).
//
//  AUTOR:				Weronika Luźna
//						
//  DATA				2 Listopada 2015 (Versja 1.00).
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 8.1.
//						Kompilator:         Microsoft Visual Studio 2015
//
//  MATERIA£Y			http://www.zsk.ict.pwr.wroc.pl/zsk/repository/dydaktyka/gk/zadania_domowe/zadania_2.pdf
//	ŹRÓDŁOWE:			
//		
//  UŻYTE BIBLIOTEKI	Nie używano.
//  NIESTANDARDOWE
//		
//*********************************************************************


//#include "stdafx.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>
#include<iostream>
using namespace std;

struct position {
	GLfloat x;
	GLfloat y;
};

//-------------------------------------------------------------------
// Tabela współczynników odwzorowań.
//-------------------------------------------------------------------
GLfloat ratio[4][6] = {
	{ -0.67f, -0.02f, 0.00f, -0.18f, 0.81f, 10.00f },
	{ 0.40f, 0.40f, 0.00f, -0.10f, 0.40f, 0.00f },
	{ -0.40f, -0.40f, 0.00f, -0.10f, 0.40f, 0.00f },
	{ -0.10f, 0.00f, 0.00f, 0.44f, 0.44f, -2.00f },
};

//-------------------------------------------------------------------
// Obliczanie współrzędnych punktu dla wylosowanego odwzorowania.
//-------------------------------------------------------------------
position CalculateCoordinates(GLfloat x, GLfloat y) {
	
	int phi = (rand() % 4);

	position pos;
	
	pos.x = ratio[phi][0] * x + ratio[phi][1] * y + ratio[phi][2];
	pos.y = ratio[phi][3] * x + ratio[phi][4] * y + ratio[phi][5];

	return pos;
}

//-------------------------------------------------------------------
// Generowanie obrazu.
//-------------------------------------------------------------------
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 1.0f, 0.0f);
	
	GLfloat x = rand();
	GLfloat	y = rand();
	
	glBegin(GL_POINTS);
	for (int i = 0; i < 100000000; i++) {
		glVertex2f(x, y);
		
		position pos = CalculateCoordinates(x, y);
		x = pos.x;
		y = pos.y;
	}
	glEnd();

	glFlush();
}

void MyInit(void) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

//-------------------------------------------------------------------
// Skalowanie obrazu z zachowaniem proporcji obiektu.
//-------------------------------------------------------------------
void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	GLfloat AspectRatio;

	if (vertical == 0)
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void) {
	srand(time(NULL));
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Iterated function system");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMainLoop();
}