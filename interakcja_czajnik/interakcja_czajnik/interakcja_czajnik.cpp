//*********************************************************************
//
//  PLIK è?R”D?OWY:		interakcja_czajnik.cpp
//
//  OPIS:				Program s?u?y do rysowania sceny 3-D				
//						z mo?liwo?ci? interakcji u?ytkownika.
//
//  AUTOR:				Weronika Lu?na
//						
//  DATA				21 Grudnia 2015 (Versja 1.00).
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 8.1.
//						Kompilator:         Microsoft Visual Studio 2015
//
//  MATERIA£Y			http://www.zsk.ict.pwr.wroc.pl/zsk/dyd/intinz/gk/lab/cw_4_dz/
//	è?R”D?OWE:			
//		
//  U?YTE BIBLIOTEKI	Nie u?ywano.
//  NIESTANDARDOWE
//		
//*********************************************************************

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

static GLfloat	theta_y = 0.0,	
theta_x = 0.0;
static GLfloat	pix2angle_x,	// przelicznik pikseli na stopnie
pix2angle_y;
static GLint	status = 0;		// stan klawiszy myszy
								
static int	x_pos_old = 0,		// poprzednia pozycja kursora myszy
y_pos_old = 0;
static int	delta_x = 0,		
delta_y = 0;        

void Axes(void) {
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };

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

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	if (status == 1) {	// jeúli lewy klawisz myszy wciúniÍty
		theta_x += delta_x*pix2angle_x;		// modyfikacja kπta obrotu o kπt proporcjonalny
		theta_y += delta_y*pix2angle_y;		// do rÛønicy po≥oøeÒ kursora myszy
	}
	else if (status == 2) {	
		viewer[2] += delta_y;	
		if (viewer[2] <= 4.0)	// ograniczenie zblizenia
			viewer[2] = 4.0;
		if (viewer[2] >= 30)	// ograniczenie oddalenia
			viewer[2] = 30;
	}

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	Axes();

	glRotatef(theta_x, 0.0, 1.0, 0.0); 
	glRotatef(theta_y, 1.0, 0.0, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);

	glutWireTeapot(3.0);

	glFlush();
	glutSwapBuffers();
}

void MyInit(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	pix2angle_x = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0 / (float)vertical;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80, 1.0, 1.0, 30.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;        // jako pozycji poprzedniej
		status = 1;         // wciúniÍty zosta≥ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;		// przypisanie aktualnie odczytanej pozycji kursora
							// jako pozycji poprzedniej
		status = 2;			//wciúniÍty zosta≥ prawy klawisz myszy
	}
	else
		status = 0;         // nie zosta≥ wciúniÍty øaden klawisz
}

void Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;    // obliczenie rÛønicy po≥oøenia kursora myszy
	x_pos_old = x;	// podstawienie bieøacego po≥oøenia jako poprzednie

	delta_y = y - y_pos_old;	// obliczenie rÛønicy po≥oøenia kursora myszy
	y_pos_old = y;	// podstawienie bieøacego po≥oøenia jako poprzednie

	glutPostRedisplay();	// przerysowanie obrazu sceny
}

void main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	glutCreateWindow("Rzutowanie perspektywiczne, prosta interakcja");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}