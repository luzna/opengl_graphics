//*********************************************************************
//
//  PLIK è?R”D?OWY:		Source.cpp
//
//  OPIS:				Program s?u?y do rysowania ?a?cucha utworzonego z torusÛw.
//
//  AUTOR:				Weronika Lu?na
//						
//  DATA				7 Grudnia 2015 (Versja 1.00).
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 8.1.
//						Kompilator:         Microsoft Visual Studio 2015
//
//  MATERIA£Y			http://www.zsk.ict.pwr.wroc.pl/zsk/repository/dydaktyka/gk/zadania_domowe/zadania_3.pdf
//	è?R”D?OWE:			
//		
//  U?YTE BIBLIOTEKI	Nie u?ywano.
//  NIESTANDARDOWE
//		
//*********************************************************************

#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

typedef float point3[3];

// inicjalizacja polozenia obserwatora 
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k?ty obrotu

const int n = 20;
const GLfloat R = 1;
const GLfloat r = R/4;
const float PI = 3.1415;
point3 coordinates[n][n];
point3 colors[n][n];
int model = 1; // 1- punkty, 2- siatka, 3 - wype?nione trÛjk?ty

//-------------------------------------------------------------------
// Obliczanie wspÛ?rz?dnych siatki torusa i generowanie siatki kolorÛw.
//-------------------------------------------------------------------
void CalculateCoordinates()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			float u = i / (n - 1.0);
			float v = j / (n - 1.0);
			coordinates[i][j][0] = ((R + r*cos(2 * PI*v))*cos(2 * PI*u));
			coordinates[i][j][1] = ((R + r*cos(2 * PI*v))*sin(2 * PI*u));
			coordinates[i][j][2] = r*sin(2 * PI*v);

			colors[i][j][0] = ((float)rand()) / RAND_MAX;
			colors[i][j][1] = ((float)rand()) / RAND_MAX;
			colors[i][j][2] = ((float)rand()) / RAND_MAX;
		}
	}
}

//-------------------------------------------------------------------
// Osie wspÛ?rz?dnych.
//-------------------------------------------------------------------
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

//-------------------------------------------------------------------
// Rysowanie torusa w trzech wybieralnych modelach.
//-------------------------------------------------------------------
void Torus()
{
	switch (model) {
	case 1:
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				glVertex3fv(coordinates[i][j]);
		glEnd();
		break;
	case 2:
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		for (int i = 0; i < n - 1; i++)
			for (int j = 0; j < n - 1; j++)
			{
				glVertex3fv(coordinates[i][j]);
				glVertex3fv(coordinates[(i)+1][j]);
				glVertex3fv(coordinates[i][j]);
				glVertex3fv(coordinates[i][(j + 1)]); 
				glVertex3fv(coordinates[i][j]);
				glVertex3fv(coordinates[(i + 1)][(j + 1)]);
			}
		glEnd();
		break;
	case 3:
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < n - 1; i++)
			for (int j = 0; j < n - 1; j++)
			{
				glColor3fv(colors[i][j]);
				glVertex3fv(coordinates[i][j]);

				glColor3fv(colors[i + 1][j]);;
				glVertex3fv(coordinates[(i)+1][j]); 

				glColor3fv(colors[i][j + 1]);
				glVertex3fv(coordinates[i][(j + 1)]); 

				glColor3fv(colors[i][j + 1]);
				glVertex3fv(coordinates[i][(j + 1)]);

				glColor3fv(colors[i + 1][j]);;
				glVertex3fv(coordinates[(i)+1][j]);

				glColor3fv(colors[i + 1][j + 1]);
				glVertex3fv(coordinates[(i + 1)][(j + 1)]); 
			}
		glEnd();
	}

}

void Mesh() {
	
	Torus();
	glTranslatef(3*R/2,0,0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
}

//-------------------------------------------------------------------
// Rysowanie ?a?cucha.
//-------------------------------------------------------------------
void Chain() {
	
	Torus();
	glTranslatef(3 * R / 2, 0, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
	glTranslatef(3 * R / 2, 0, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
	glTranslatef(3 * R / 2, R/2, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(30.0, 0.0, 1.0, 0.0);
	Torus();
	glTranslatef(3 * R / 2, 0, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
	glTranslatef(R, R, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(60.0, 0.0, 1.0, 0.0);
	Torus();
	glTranslatef(3 * R / 2, R / 2, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(30.0, 0.0, 1.0, 0.0);
	Torus();
	glTranslatef(3 * R / 2, R/2, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(30.0, 0.0, 1.0, 0.0);
	Torus();

}

void RenderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Axes();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	glTranslatef(-5* R, 0, 0);
	/*for (int i = 0; i < 15; i++)
		Mesh();*/
	Chain();
	glFlush();
	glutSwapBuffers();
}
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}

void spin()
{

theta[0] -= 0.05;
if (theta[0] > 360.0) theta[0] -= 360.0;

theta[1] -= 0.05;
if (theta[1] > 360.0) theta[1] -= 360.0;

theta[2] -= 0.05;
if (theta[2] > 360.0) theta[2] -= 360.0;

glutPostRedisplay(); //od?wie?enie zawarto?ci aktualnego okna
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Kolor czyszcπcy (wype≥nienia okna) ustawiono na czarny
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	gluPerspective(70, 1.0, 1.0, 30.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
                  
	glLoadIdentity();
}

int main(void)

{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(900, 600);
	

	glutCreateWindow("Torus chain");

	CalculateCoordinates();
	glutIdleFunc(spin);
	glutKeyboardFunc(keys);

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	
	return 0;
}