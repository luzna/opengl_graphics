//*********************************************************************
//
//  PLIK è?R”D?OWY:		dywan.cpp
//
//  OPIS:				Program s?u?y do rysowania dywanu Sierpi?skiego.					
//
//  AUTOR:				Weronika Lu?na
//						
//  DATA				2 Listopada 2015 (Versja 1.00).
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 8.1.
//						Kompilator:         Microsoft Visual Studio 2015
//
//  MATERIA£Y			http://www.zsk.ict.pwr.wroc.pl/zsk/dyd/intinz/gk/lab/cw_2_dz/
//	è?R”D?OWE:			
//		
//  U?YTE BIBLIOTEKI	Nie u?ywano.
//  NIESTANDARDOWE
//		
//*********************************************************************

//#include "stdafx.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cstdlib>
#include <ctime>

//-------------------------------------------------------------------
// Rysowanie kwadratu z "dziur?" w oparciu o wspÛ?rz?dne ?rodka i d?ugo?? boku.
// Kolorowanie losowymi barwami. 
//-------------------------------------------------------------------
void DrawRectangle(GLfloat x, GLfloat y, GLfloat a) {
	
	GLfloat R = (std::rand() % 100) / 100.0f;
	GLfloat G = (std::rand() % 100) / 100.0f;
	GLfloat B = (std::rand() % 100) / 100.0f;

	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex2f(x - a / 2.0f, y + a / 2.0f);
	R = (std::rand() % 100) / 100.0f;
	G = (std::rand() % 100) / 100.0f;
	B = (std::rand() % 100) / 100.0f;
	glColor3f(R, G, B);
	glVertex2f(x + a / 2.0f, y + a / 2.0f);
	R = (std::rand() % 100) / 100.0f;
	G = (std::rand() % 100) / 100.0f;
	B = (std::rand() % 100) / 100.0f;
	glColor3f(R, G, B);
	glVertex2f(x + a / 2.0f, y - a / 2.0f);
	R = (std::rand() % 100) / 100.0f;
	G = (std::rand() % 100) / 100.0f;
	B = (std::rand() % 100) / 100.0f;
	glColor3f(R, G, B);
	glVertex2f(x - a / 2.0f, y - a / 2.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(x - a / 6.0f, y + a / 6.0f);
	glVertex2f(x + a / 6.0f, y + a / 6.0f);
	glVertex2f(x + a / 6.0f, y - a / 6.0f);
	glVertex2f(x - a / 6.0f, y - a / 6.0f);
	glEnd();
}

//-------------------------------------------------------------------
// Rysowanie kwadratÛw wokÛ? ?rodka poprzedniego kwadratu.
// Generowanie perturbacji (przez losowanie warto?ci zmiennej alpha).
//-------------------------------------------------------------------
void CalculateCoordinates(GLfloat X, GLfloat Y, GLfloat A) {
	GLfloat a = A / 3.0f;
	GLfloat alpha;
	GLfloat coordinates[8][2] = { { X - a, Y - a },
	{ X - a, Y  },
	{ X - a , Y + a  },
	{ X 	, Y + a  },
	{ X + a , Y + a  },
	{ X + a , Y  },
	{ X + a , Y - a  },
	{ X 	, Y - a  }
	};

	DrawRectangle(X, Y, A);

	for (int i = 0; i < 8; i++) {
		if (a > 2.0f) {
			alpha = ((float)(rand() % 20 - 10)) / 6;
			CalculateCoordinates(coordinates[i][0]+alpha, coordinates[i][1]+alpha, a);
		}
	}
}

//-------------------------------------------------------------------
// Generowanie obrazu.
//-------------------------------------------------------------------
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	CalculateCoordinates(0.0f, 0.0f, 168.0f);

	glFlush();
}

//-------------------------------------------------------------------
// Kolor t?a.
//-------------------------------------------------------------------
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
	glutCreateWindow("Dywan sierpinskiego");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMainLoop();
}