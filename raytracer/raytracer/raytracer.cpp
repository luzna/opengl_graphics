#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef float point3[3];

//************************************************************************************* /
// Prototypy u?ywanych funkcji
/*************************************************************************************/

//Funkcja wyznacza wspolrzedne punktu przeciecia promienia i obiektu
int Intersect(point3 p, point3 v);

//Funkcja oblicza kolor piksela dla promienia zaczynajacego sie w punkcie p i biegnacego w kierunku wskazywanym przez wektor v
void Color(point3 p, point3 v, int step);

//Funkcja obliczajaca oswietlenie punktu na powierzchni sfery wedlug modelu Phonga
void Phong(int nr, float *viewerVec);

//Funkcja obliczajaca iloczyn skalarny dwoch wektorow
float dotProduct(point3 p1, point3 p2);

//Funkcja normalizujaca wektor
void Normalization(point3 p);

//Funkcja obliczaj?ca wektor normalny w punkcie
void Normal(int nr);

//Funkcja obliczaj?ca kierunek odbicia promienia w punkcie
void Reflect(point3 v);

//Funkcja pobieraj?ca informacje z pliku
void ReadSceneFromFile(string fileName);

//--------------------------------------------------------------------
//Zmienne globalne
//--------------------------------------------------------------------

// Rozmiar obrazu w pikselach
int     image_size = 400;

//Rozmiar okna obserwatora
float   viewport_size = 20.0;

//Po?o?enie i parametry ?ród?a ?wiat?a
float   light_position[10][3];
float   light_specular[10][3];
float   light_diffuse[10][3];
float   light_ambient[10][3];

//Parametry rysowanej sfery
float   sphere_radius[10];
float   sphere_position[10][3];
float   sphere_specular[10][3];
float   sphere_diffuse[10][3];
float   sphere_ambient[10][3];
float   sphere_specularhininess[10];

//Parametry swiatla rozproszonego (wczytywane z pliku)
point3   global_ambient;

//Parametry sledzonego promienia
point3   starting_point;						//punkt, z ktorego wychodzi promien
point3   starting_directions = { 0.0, 0.0, -1.0 };	//wektor opisujacy kierunek promienia

											//Wektor normalny do powierzchni
point3   normal_vector;
point3   reflection_vector;

//Zmienne pomocnicze
point3	inters;		//wspolrzedne (x,y,z) punktu przeciecia promienia i sfery
point3  inters_color;		//skladowe koloru dla oswietlonego punktu na powierzchni sfery
point3	color;
point3  background_color;			//kolor tla wczytywany z pliku
GLubyte pixel[1][1][3];		//skladowe koloru rysowanego piksela

int     number = 1,
spheres = 0,		//liczba sfer wczytywana z pliku
lights = 0;			//liczba zrodel swiatla wczytywana z pliku

int		limit = 50;			//limit iteracji

//------------------------------------------------------------------------
//Funkcja wyznacza wspolrzedne punktu przeciecia promienia z obiektem
//------------------------------------------------------------------------
int Intersect(point3 p, point3 v) {
	float r, a, b, c, d;
	float length = 1000000000000;
	int status = -1;
	for (int i = 0; i<spheres; i++) {
		a = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		b = 2 * (v[0] * (p[0] - sphere_position[i][0]) + v[1] * (p[1] - sphere_position[i][1]) + v[2] * (p[2] - sphere_position[i][2]));
		c = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] - 2 * (sphere_position[i][0] * p[0] + sphere_position[i][1] * p[1] + sphere_position[i][2] * p[2]) + sphere_position[i][0] * sphere_position[i][0] + sphere_position[i][1] * sphere_position[i][1] + sphere_position[i][2] * sphere_position[i][2] - sphere_radius[i] * sphere_radius[i];
		d = b*b - 4 * a*c;
		if (d >= 0) {
			r = (-b - sqrt(d)) / (2 * a);
			if (r > 0 && r < length) {
				inters[0] = p[0] + r*v[0];
				inters[1] = p[1] + r*v[1];
				inters[2] = p[2] + r*v[2];
				length = sqrt((inters[0] - p[0])*(inters[0] - p[0]) + (inters[1] - p[1])*(inters[1] - p[1]) + (inters[2] - p[2])*(inters[2] - p[2]));
				status = i;
			}
		}
	}
	return status;
}

//-------------------------------------------------------------------------
//Funkcja oblicza kolor piksela dla promienia zaczynajacego sie w punkcie p 
//biegnacego w kierunku wskazywanym przez wektor v
//-------------------------------------------------------------------------
void Color(point3 p, point3 v, int step)
{
	if (step > limit)
		return;

	number = Intersect(p, v);
	if (number >= 0) {
		Normal(number);
		Reflect(v);
		Phong(number, v);
		color[0] += inters_color[0];
		color[1] += inters_color[1];
		color[2] += inters_color[2];
		Color(inters, reflection_vector, step + 1);
	}
	else
		return;
}

//-------------------------------------------------------------------------------
//Funkcja oblicza oswietlenie punktu na powierzchni sfery uzywajac modelu Phonga.
//-------------------------------------------------------------------------------
void Phong(int nr, point3 viewerVec)
{
	point3 reflection_vector;           //wektor kierunku odbicia swiatla
	point3 light_vec;                //wektor wskazujacy zrodel
	float n_dot_l, v_dot_r;			//zmienne pomocnicze

	point3 viewer = { -viewerVec[0], -viewerVec[1], -viewerVec[2] };

	inters_color[0] = 0;
	inters_color[1] = 0;
	inters_color[2] = 0;

	for (int i = 0; i<lights; i++) {
		light_vec[0] = light_position[i][0] - inters[0]; //wektor wskazujacy kierunek zrodla
		light_vec[1] = light_position[i][1] - inters[1];
		light_vec[2] = light_position[i][2] - inters[2];

		Normalization(light_vec);    //normalizacja wektora kierunku swiecenia zrodla           

		n_dot_l = dotProduct(light_vec, normal_vector);

		reflection_vector[0] = 2 * (n_dot_l)*normal_vector[0] - light_vec[0];
		reflection_vector[1] = 2 * (n_dot_l)*normal_vector[1] - light_vec[1];
		reflection_vector[2] = 2 * (n_dot_l)*normal_vector[2] - light_vec[2];

		//obliczenie wektora opisujacego kierunek swiatla odbitego od punktu na powierzchni sfery

		Normalization(reflection_vector); //normalizacja wektora kierunku swiatla odbitego

		v_dot_r = dotProduct(reflection_vector, viewer);

		if (v_dot_r < 0)                  //obserwator nie widzi oswietlanego punktu
			v_dot_r = 0;

		//sprawdzenie czy punkt na powierzchni sfery jest oswietlany przez zrodlo

		if (n_dot_l > 0)    //punkt jest oswietlany,
		{                   //oswietlenie wyliczane jest ze wzorow dla modelu Phonga
			float x = sqrt((light_position[i][0] - inters[0])*(light_position[i][0] - inters[0]) + (light_position[i][1] - inters[1])*(light_position[i][1] - inters[1]) + (light_position[i][2] - inters[2])*(light_position[i][2] - inters[2]));
			inters_color[0] += (1.0 / (1.0 + 0.01*x + 0.001*x*x))*(sphere_diffuse[nr][0] * light_diffuse[i][0] * n_dot_l + sphere_specular[nr][0] * light_specular[i][0] * pow(double(v_dot_r), (double)sphere_specularhininess[nr])) + sphere_ambient[nr][0] * light_ambient[i][0];
			inters_color[1] += (1.0 / (1.0 + 0.01*x + 0.001*x*x))*(sphere_diffuse[nr][1] * light_diffuse[i][1] * n_dot_l + sphere_specular[nr][1] * light_specular[i][1] * pow(double(v_dot_r), (double)sphere_specularhininess[nr])) + sphere_ambient[nr][1] * light_ambient[i][1];
			inters_color[2] += (1.0 / (1.0 + 0.01*x + 0.001*x*x))*(sphere_diffuse[nr][2] * light_diffuse[i][2] * n_dot_l + sphere_specular[nr][2] * light_specular[i][2] * pow(double(v_dot_r), (double)sphere_specularhininess[nr])) + sphere_ambient[nr][2] * light_ambient[i][2];
		}
		//punkt nie jest oswietlany   
		//uwzgledniane jest tylko swiatlo rozproszone
		inters_color[0] += sphere_ambient[nr][0] * global_ambient[0];
		inters_color[1] += sphere_ambient[nr][1] * global_ambient[1];
		inters_color[2] += sphere_ambient[nr][2] * global_ambient[2];
	}
}

//---------------------------------------------
//Funkcja przeprowadza normalizacje wektora.
//---------------------------------------------
void Normalization(point3 p)
{
	float d = 0.0;
	int i;

	for (i = 0; i<3; i++)
		d += p[i] * p[i];

	d = sqrt(d);

	if (d>0.0)
		for (i = 0; i<3; i++)
			p[i] /= d;
}

//-------------------------------------------
//Funkcja oblicza iloczyn skalarny wektorow.
//-------------------------------------------
float dotProduct(point3 p1, point3 p2)
{
	float result = p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2];
	return result;
}

//-----------------------------------------------------
//Funkcja oblicza kierunek odbicia promienia w punkcie
//-----------------------------------------------------
void Reflect(point3 v) {
	float	n_dot_i;
	float   invert[3] = { -v[0], -v[1], -v[2] };

	Normalization(invert);

	n_dot_i = dotProduct(invert, normal_vector);
	reflection_vector[0] = 2 * (n_dot_i)*normal_vector[0] - invert[0];
	reflection_vector[1] = 2 * (n_dot_i)*normal_vector[1] - invert[1];
	reflection_vector[2] = 2 * (n_dot_i)*normal_vector[2] - invert[2];

	Normalization(reflection_vector);
}

//-------------------------------------------
//Funkcja oblicza wektor normalny w punkcie
//-------------------------------------------
void Normal(int number)
{
	normal_vector[0] = inters[0] - sphere_position[number][0];
	normal_vector[1] = inters[1] - sphere_position[number][1];
	normal_vector[2] = inters[2] - sphere_position[number][2];

	Normalization(normal_vector);
}

//------------------------------------------------------
//Funkcja definiuje sposob rzutowania (równoleg?e)
//------------------------------------------------------
void Myinit(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-viewport_size / 2, viewport_size / 2, -viewport_size / 2, viewport_size / 2, -viewport_size / 2, viewport_size / 2);
	glMatrixMode(GL_MODELVIEW);
}

//---------------------------------------------------------
//Funkcja rysuje obraz oswietlonej sceny
//---------------------------------------------------------
void Display(void)
{
	int  x, y;           // pozycja rysowanego piksela "ca?kowitoliczbowa"
	float x_fl, y_fl;    // pozycja rysowanego piksela "zmiennoprzecinkowa"
	int image_size_2;       //polowa rozmiaru obrazu w pikselach

	image_size_2 = image_size / 2;    //obliczenie polowy rozmiaru obrazu w pikselach
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();

	// rysowanie pikseli od lewego górnego naro?nika do prawego dolnego naro?nika
	for (y = image_size_2; y > -image_size_2; y--)
	{
		for (x = -image_size_2; x < image_size_2; x++)
		{
			x_fl = (float)x / (image_size / viewport_size);
			y_fl = (float)y / (image_size / viewport_size);

			//przeliczenie pozycji(x,y) w pikselach na pozycje
			//"zmiennoprzecinkowa" w oknie obserwatora
			starting_point[0] = x_fl;
			starting_point[1] = y_fl;
			starting_point[2] = viewport_size;

			//wyznaczenie poczatku sledzonego promienia dla rysowanego piksela
			color[0] = 0.0;
			color[1] = 0.0;
			color[2] = 0.0;
			//wyznaczenie koloru piksela
			Color(starting_point, starting_directions, 1);
			if (color[0] == 0.0) color[0] = background_color[0];
			if (color[1] == 0.0) color[1] = background_color[1];
			if (color[2] == 0.0) color[2] = background_color[2];

			//konwersja wartosci wyliczonego oswietlenia na liczby od 0 do 255
			color[0] > 1 ? pixel[0][0][0] = 255 : pixel[0][0][0] = color[0] * 255;
			color[1] > 1 ? pixel[0][0][1] = 255 : pixel[0][0][1] = color[1] * 255;
			color[2] > 1 ? pixel[0][0][2] = 255 : pixel[0][0][2] = color[2] * 255;

			glRasterPos3f(x_fl, y_fl, 0);
			//inkrementacja pozycji rastrowej dla rysowania piksela

			glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			//Narysowanie kolejnego piksela na ekranie
		}
	}
	glFlush();
}

//Funkcja pobiera informacje z pliku
void ReadSceneFromFile(string fileName)
{
	string buffer = "";
	float value = 0.0;
	int i = 0;

	fstream file(fileName.c_str(), ios::in);
	if (!file.is_open()) {
		cout << "B??d. Brak pliku o nazwie:(" << fileName << ")" << endl;
		system("PAUSE");
		exit(0);
	}
	while (!file.eof()) {
		file >> buffer;
		//Rozmiar okna
		if (buffer == "dimensions") {
			file >> value;
			image_size = value;
		}
		//Kolor tla
		if (buffer == "background") {
			for (i = 0; i<3; i++) {
				file >> value;
				background_color[i] = value;
			}
		}
		//Parametry swiatla rozproszonego
		if (buffer == "global") {
			for (i = 0; i<3; i++) {
				file >> value;
				global_ambient[i] = value;
			}
		}
		//Parametry rysowanej sfery
		while (buffer == "sphere" && !file.eof()) {
			file >> value;
			sphere_radius[spheres] = value;
			for (i = 0; i<3; i++) {
				file >> value;
				sphere_position[spheres][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				sphere_specular[spheres][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				sphere_diffuse[spheres][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				sphere_ambient[spheres][i] = value;
			}
			file >> value;
			sphere_specularhininess[spheres] = value;
			spheres++;
			file >> buffer;
		}
		//Parametry zrodel swiatla
		while (buffer == "source" && !file.eof()) {
			for (i = 0; i<3; i++) {
				file >> value;
				light_position[lights][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				light_specular[lights][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				light_diffuse[lights][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				light_ambient[lights][i] = value;
			}
			lights++;
			file >> buffer;
		}
	}
	file.close();
}

//-------------------------------------------------------------
//Funkcja glowna
//-------------------------------------------------------------
void main(void)
{
	string file = "scene.txt";


	ReadSceneFromFile(file);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(image_size, image_size);
	glutCreateWindow("Ray Tracing");
	Myinit();
	glutDisplayFunc(Display);
	glutMainLoop();
}