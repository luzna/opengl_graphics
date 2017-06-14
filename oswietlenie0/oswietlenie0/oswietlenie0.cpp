//*********************************************************************
//
//  PLIK è?R”D?OWY:		oswietlenie1.cpp
//
//  OPIS:				Program s?u?y do rysowania o?wietlonej sceny 3-D.
//
//  AUTOR:				Weronika Lu?na
//						
//  DATA				11 Stycznia 2016 (Versja 1.00).
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 8.1.
//						Kompilator:         Microsoft Visual Studio 2015
//
//  U?YTE BIBLIOTEKI	Nie u?ywano.
//  NIESTANDARDOWE
//		
//*********************************************************************

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <time.h>

//Definicja typu point3 - punkt w przestrzeni 3D
typedef float point3[3];

const float PI = 3.14159265;

point3 **coordinates;
point3 **norm_coordinates;

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

static GLfloat	phi = 0.0,
theta = 0.0;
static GLfloat	pix2angle_x = 0.0,
pix2angle_y = 0.0;
static GLint status = 0;
static int	x_pos_old = 0,
y_pos_old = 0;
static int	delta_x = 0,
delta_y = 0;

int N = 50;
float verLength = 1.0;
float viewerR = 10.0;

static GLfloat angle[] = { 0.0, 0.0, 0.0 };

//Funkcja wyliczajaca wspolrzedna X
float Calculate_x(float u, float v) {
	float x, a = v*PI;

	x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(a);
	return x;
}

//Funkcja wyliczajaca wspolrzedna Y
float Calculate_y(float u, float v) {
	float y;

	y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
	return y - 5;
}

//Funkcja wyliczajaca wspolrzedna Z
float Calculate_z(float u, float v) {
	float z, a = v*PI;

	z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(a);
	return z;
}

//Obliczenie wspolrzednej X wektora normalnego do powierzchni w punkcie
float Calculate_norm_x(float u, float v) {
	float x, a = v*PI;

	float yu = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
	float yv = 0;
	float zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(a);
	float zv = -PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(a);

	x = (GLfloat)(yu*zv - zu*yv);
	return x;
}

//Obliczenie wspolrzednej Y wektora normalnego do powierzchni w punkcie
float Calculate_norm_y(float u, float v) {
	float y, a = v*PI;

	float xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(a);
	float xv = PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(a);
	float zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(a);
	float zv = -PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(a);

	y = (GLfloat)(zu*xv - xu*zv);
	return y;
}

//Obliczenie wspolrzednej Z wektora normalnego do powierzchni w punkcie
float Calculate_norm_z(float u, float v) {
	float z, a = v*PI;

	float xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(a);
	float xv = PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(a);
	float yu = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
	float yv = 0;

	z = (GLfloat)(xu*yv - yu*xv);
	return z;
}

//Funkcja generujaca siatke puntow, najpierw w 2D, potem w 3D
void Mesh() {
	float stepXY = verLength / N;

	for (int i = 0; i<N + 1; i++) {
		for (int j = 0; j<N + 1; j++) {
			coordinates[i][j][0] = j*stepXY;
			coordinates[i][j][1] = i*stepXY;
		}
	}

	float u, v;
	for (int i = 0; i<N + 1; i++) {
		for (int j = 0; j<N + 1; j++) {
			v = coordinates[i][j][0];
			u = coordinates[i][j][1];
			coordinates[i][j][0] = Calculate_x(u, v);
			coordinates[i][j][1] = Calculate_y(u, v);
			coordinates[i][j][2] = Calculate_z(u, v);

			//Wyliczenie wspolrzednych wektorow normalnych
			float x = Calculate_norm_x(u, v);
			float y = Calculate_norm_y(u, v);
			float z = Calculate_norm_z(u, v);

			if (i < N / 2) {
				norm_coordinates[i][j][0] = x / (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				norm_coordinates[i][j][1] = y / (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				norm_coordinates[i][j][2] = z / (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
			}
			if (i > N / 2) {
				norm_coordinates[i][j][0] = -1.0*x / (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				norm_coordinates[i][j][1] = -1.0*y / (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				norm_coordinates[i][j][2] = -1.0*z / (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
			}
			//Wektory na "szczycie" jajka
			if (i == N / 2) {
				norm_coordinates[i][j][0] = 0;
				norm_coordinates[i][j][1] = 1;
				norm_coordinates[i][j][2] = 0;
			}
			//Wektory na "dnie" jajka
			if (i == 0 || i == N)
			{
				norm_coordinates[i][j][0] = 0;
				norm_coordinates[i][j][1] = -1;
				norm_coordinates[i][j][2] = 0;
			}
		}
	}
}

void Egg(void) {
	Mesh();

	glColor3f(1.0, 1.0, 1.0);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			glBegin(GL_TRIANGLES);
			glNormal3fv(norm_coordinates[i][j + 1]);
			glVertex3fv(coordinates[i][j + 1]);
			glNormal3fv(norm_coordinates[i + 1][j]);
			glVertex3fv(coordinates[i + 1][j]);
			glNormal3fv(norm_coordinates[i + 1][j + 1]);
			glVertex3fv(coordinates[i + 1][j + 1]);
			glEnd();

			glBegin(GL_TRIANGLES);
			glNormal3fv(norm_coordinates[i][j]);
			glVertex3fv(coordinates[i][j]);
			glNormal3fv(norm_coordinates[i + 1][j]);
			glVertex3fv(coordinates[i + 1][j]);
			glNormal3fv(norm_coordinates[i][j + 1]);
			glVertex3fv(coordinates[i][j + 1]);
			glEnd();
		}
	}
}

void Axes(void)
{
	point3  x_min = { -2.0, 0.0, 0.0 };
	point3  x_max = { 2.0, 0.0, 0.0 };

	point3  y_min = { 0.0, -2.0, 0.0 };
	point3  y_max = { 0.0,  2.0, 0.0 };

	point3  z_min = { 0.0, 0.0, -2.0 };
	point3  z_max = { 0.0, 0.0,  2.0 };

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

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (status == 2) {
		viewerR += 0.1* delta_y;
		if (viewerR <= 6.0)
			viewerR = 6.0;
		if (viewerR >= 25.0)
			viewerR = 25.0;
	}

	viewer[0] = viewerR * cos(theta) * cos(phi);
	viewer[1] = viewerR * sin(phi);
	viewer[2] = viewerR * sin(theta) * cos(phi);

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(phi), 0.0);

	Axes();

	glRotatef(angle[0], 1.0, 0.0, 0.0);
	glRotatef(angle[1], 0.0, 1.0, 0.0);
	glRotatef(angle[2], 0.0, 0.0, 1.0);

	Egg();

	glFlush();

	glutSwapBuffers();
}

void spinEgg()
{
	angle[0] -= 0.5;
	if (angle[0] > 360.0) angle[0] -= 360.0;
	angle[1] -= 0.5;
	if (angle[1] > 360.0) angle[1] -= 360.0;
	angle[2] -= 0.5;
	if (angle[2] > 360.0) angle[2] -= 360.0;

	glutPostRedisplay();
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Definicja materia≥u z jakiego zrobiony jest przedmiot
	//-------------------------------------------------------
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspÛ≥czynniki ka =[kar,kag,kab] dla úwiat≥a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspÛ≥czynniki kd =[kdr,kdg,kdb] úwiat≥a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspÛ≥czynniki ks =[ksr,ksg,ksb] dla úwiat≥a odbitego                

	GLfloat mat_shininess = { 100.0 };
	// wspÛ≥czynnik n opisujπcy po≥ysk powierzchni


	// Definicja ürÛd≥a úwiat≥a
	//-------------------------------------------------------
	GLfloat light_position[] = { 5.0, 5.0, 10.0, 1.0 };
	// po≥oøenie ürÛd≥a

	GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	// sk≥adowe intensywnoúci úwiecenia ürÛd≥a úwiat≥a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	// sk≥adowe intensywnoúci úwiecenia ürÛd≥a úwiat≥a powodujπcego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk≥adowe intensywnoúci úwiecenia ürÛd≥a úwiat≥a powodujπcego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk≥adowa sta≥a ds dla modelu zmian oúwietlenia w funkcji 
	// odleg≥oúci od ürÛd≥a

	GLfloat att_linear = { 0.05f };
	// sk≥adowa liniowa dl dla modelu zmian oúwietlenia w funkcji 
	// odleg≥oúci od ürÛd≥a

	GLfloat att_quadratic = { 0.001f };
	// sk≥adowa kwadratowa dq dla modelu zmian oúwietlenia w funkcji
	// odleg≥oúci od ürÛd≥a

	// Ustawienie patrametrÛw materia≥u 
	//-------------------------------------------------------
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	// Ustawienie parametrÛw ürÛd≥a úwiat≥a
	//-------------------------------------------------------
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie opcji systemu oúwietlania sceny 
	//-------------------------------------------------------
	glShadeModel(GL_SMOOTH); // w≥aczenie ≥agodnego cieniowania
	glEnable(GL_LIGHTING);   // w≥aczenie systemu oúwietlenia sceny 
	glEnable(GL_LIGHT0);     // w≥πczenie ürÛd≥a o numerze 0
	glEnable(GL_DEPTH_TEST); // w≥πczenie mechanizmu z-bufora 
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	pix2angle_x = 360.0*0.1 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0*0.1 / (float)vertical;

	glMatrixMode(GL_PROJECTION);
	// Prze≥πczenie macierzy bieøπcej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bieøπcej

	gluPerspective(70.0, 1.0, 1.0, 30.0);
	// Ustawienie parametrÛw dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoúci okna okna widoku (viewport) w zaleønoúci
	// relacji pomiÍdzy wysokoúciπ i szerokoúciπ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze≥πczenie macierzy bieøπcej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bieøπcej
}

void Mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
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

void main(void)
{
	//Ziarno losowosci
	srand((unsigned)time(NULL));

	coordinates = new point3*[N + 1];
	for (int i = 0; i<N + 1; i++) {
		coordinates[i] = new point3[N + 1];
	}

	//losowo?? kolorÛw
	norm_coordinates = new point3*[N + 1];
	for (int i = 0; i < N + 1; i++) {
		norm_coordinates[i] = new point3[N + 1];
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 600);

	glutCreateWindow("Oswietlone, ruchome jajko");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);

	MyInit();

	glutMouseFunc(Mouse);

	glutMotionFunc(Motion);

	glutIdleFunc(spinEgg);

	glutMainLoop();

	for (int i = 0; i < N + 1; i++) {
		delete[] coordinates[i];
		delete[] norm_coordinates[i];
		coordinates[i] = 0;
		norm_coordinates[i] = 0;
	}
	delete[] coordinates;
	delete[] norm_coordinates;
	coordinates = 0;
	norm_coordinates = 0;
}