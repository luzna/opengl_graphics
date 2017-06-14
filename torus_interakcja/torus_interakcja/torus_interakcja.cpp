//*********************************************************************
//
//  PLIK �?R�D?OWY:		Source.cpp
//
//  OPIS:				Program s?u?y do rysowania ?a?cucha utworzonego z torus�w.
//
//  AUTOR:				Weronika Lu?na
//						
//  DATA				7 Grudnia 2015 (Versja 1.00).
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 8.1.
//						Kompilator:         Microsoft Visual Studio 2015
//
//  MATERIA�Y			http://www.zsk.ict.pwr.wroc.pl/zsk/repository/dydaktyka/gk/zadania_domowe/zadania_3.pdf
//	�?R�D?OWE:			
//		
//  U?YTE BIBLIOTEKI	Nie u?ywano.
//  NIESTANDARDOWE
//		
//*********************************************************************

#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h>


typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 30.0 };
// inicjalizacja po�o�enia obserwatora

static GLfloat theta = 0.0;   // k�t obrotu obiektu
static GLfloat phi = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
							   // 0 - nie naci�ni�to �adnego klawisza
							   // 1 - naci�ni�ty zosta� lewy klawisz

static int y_pos_old = 0;
static int x_pos_old = 0;
static int zoom_pos_old = 0;
// poprzednia pozycja kursora myszy

static int delta_x = 0;
static int delta_y = 0;
static int delta_zoom = 0;
// r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� kursora myszy

static int zoom = 25;

float p = 0;
float h = 0;
float q = 0;
const int n = 20;
const int R = 3;
const int r = 1;
const float PI = 3.1415;
point3 coordinates[n][n];
point3 colors[n][n];
int model = 3; // 1- punkty, 2- siatka, 3 - wype?nione tr�jk?ty


			   /*************************************************************************************/

			   // Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych

			   // Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;				// przypisanie aktualnie odczytanej pozycji kursora
									// jako pozycji poprzedniej
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		y_pos_old = y;
		status = 2;
	}
	else
		status = 0;          // nie zosta� wci�ni�ty �aden klawisz
}


void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (h < 0.5) {
			h += 0.1;
			q += 1;
		}
		break;
	case GLUT_KEY_DOWN:
		if (h > -0.5) {
			h -= 0.1;
			q -= 1;
		}
		break;
	case GLUT_KEY_LEFT:
		if (p > -0.5) {
			p -= 0.1;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (p < 0.5) {
			p += 0.1;
		}
		break;
	}

	glutPostRedisplay();
}


/*************************************************************************************/

// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie

	delta_y = y - y_pos_old;
	y_pos_old = y;

	delta_zoom = y - zoom_pos_old;
	zoom_pos_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

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

void Chain() {

	Torus();
	glTranslatef((3 * R / 2)+p, 0+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
	glTranslatef((3 * R / 2)+p, 0+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
	glTranslatef((3 * R / 2)+p, (R / 2)+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(30.0+q, 0.0, 1.0, 0.0);
	Torus();
	glTranslatef((3 * R / 2)+p, 0+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	Torus();
	glTranslatef(R+p, R+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(60.0+q, 0.0, 1.0, 0.0);
	Torus();
	glTranslatef((3 * R / 2)+p, (R / 2)+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(30.0+q, 0.0, 1.0, 0.0);
	Torus();
	glTranslatef((3 * R / 2)+p, (R / 2)+h, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glRotated(30.0+q, 0.0, 1.0, 0.0);
	Torus();

}

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  pocz?tek i koniec obrazu osi y

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

/*************************************************************************************/

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba
// przerysowa� scen�)



void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(phi), 0.0);
	// Zdefiniowanie po�o�enia obserwatora

	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej 

	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		theta += 0.01*delta_x*pix2angle;
		phi += 0.01*delta_y*pix2angle;
		// modyfikacja k�ta obrotu o kat proporcjonalny
	}										 // do r�nicy po�o�e� kursora myszy

	else if (status == 2)
	{
		zoom += delta_zoom;
	}

	viewer[0] = zoom * cos(theta)*cos(phi);
	viewer[1] = zoom * sin(phi);
	viewer[2] = zoom * sin(theta)*cos(phi);


	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia�y

	//glutWireTeapot(3.0);
	// Narysowanie czajnika 
	glTranslatef(-5 * R, 0, 0);

	Chain();

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();



}
/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania

void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	gluPerspective(70.0, 1.0, 1.0, 30.0);

	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Rzutowanie perspektywiczne");
	CalculateCoordinates();
	glutKeyboardFunc(keys);
	glutSpecialFunc(SpecialInput);

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzeba przerysowania okna

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna                       



	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}



/*************************************************************************************/