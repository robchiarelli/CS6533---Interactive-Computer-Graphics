#define WINDOWS     1 /* Set to 1 for Windows, 0 else */
#define UNIX_LINUX  0 /* Set to 1 for Unix/Linux, 0 else */

#if WINDOWS
#include <windows.h>
#include <GL/glut.h>
#endif
#if UNIX_LINUX
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700
//Change k if user's computer is particularly fast or slow
#define k 15000
#define SCALE 1600

using namespace std;

void file_in();
void display();
void myinit();
void idle();
void draw_circle(int x, int y, int r);
void circle_point(int x, int y, int xOffset, int yOffset);
int scale_func(int i);

//only works as a float
float j = 1;
string state;
vector<vector<int>> coordinates;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(XOFF, YOFF);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CS6533 Assignment 1");
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	//Prompt the user for which program to run
	cout << "Which program would you like to see?\n\n"
		"(1) Draw one circle with set coordinates\n"
		"(2) Draw multiple circles with coordinates from a file\n"
		"(3) Draw multiple animated circles with coordinates from a file\n\n"
		"Enter the number of your choice: ";
	cin >> state;

	//Prompt the use for circle coordinates and radius
	if (state == "1") {
		int x, y, r;
		cout << "Please provide only integer values.\n"
			"Input an x coordinate: ";
		cin >> x;
		cout << "Input a y coordinate: ";
		cin >> y;
		cout << "Input a radius: ";
		cin >> r;
		coordinates.push_back({ scale_func(x), scale_func(y), scale_func(r) });
	}
	//Only read from the file if (2) or (3) is selected
	else {
		file_in();
	}

	myinit();
	glutMainLoop();
}

void file_in(void)
{
	//Read circle data from input file
	ifstream text_file("input_circles.txt");
	if (text_file.is_open()) {
		string line;
		//Skip the first line (program does not need number of circles)
		getline(text_file, line);
		while (getline(text_file, line)) {
			int x, y, r;
			//Convert single line of coordinates to a string stream and assign them to circle values
			istringstream ss(line);
			ss >> x >> y >> r;
			coordinates.push_back({ scale_func(x), scale_func(y), scale_func(r) });
		}
	}
	else
		cerr << "Error: Unable to open file.";
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 0.84, 0);
	glPointSize(1.0);

	glLoadIdentity();
	glTranslatef((WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2), 0.0f);

	glBegin(GL_POINTS);

	//Draw each circle that was placed in the coordinates vector
	//Does not require circle count provided in input_circles
	for (size_t i = 0; i < coordinates.size(); i++) {
		draw_circle(coordinates[i][0], coordinates[i][1], coordinates[i][2]);
	}

	glEnd();

	glFlush();

	glutSwapBuffers();
}

void myinit()
{
	glClearColor(0.0, 0.0, 0.92, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
	//Change the size of the circle once per frame if running (3)
	if (state == "3") {
		j++;
		if (j > k)
			j = 0;
	}
	//Keep the circle size constant if running (1) or (2)
	else if (state == "2" || state == "1")
		j = k;

	//Refresh the screen
	glutPostRedisplay();
}

void draw_circle(int x, int y, int r)
{
	//Change the radius to accomodate the size of the growing circle
	//As seen above, j / k = 1 if program (3) was not chosen
	r *= j / k;
	
	int xOffset = 0;
	int yOffset = r;
	
	//Only stays as dStart until the first iteration of the following loop
	int dStart = 1 - r;
	circle_point(x, y, xOffset, yOffset);
	while (xOffset < yOffset) {
		xOffset++;
		if (dStart < 0)
			dStart += 2 * xOffset + 1;
		else {
			yOffset--;
			dStart += 2 * (xOffset - yOffset) + 2;
		}
		circle_point(x, y, xOffset, yOffset);
	}
}

void circle_point(int x, int y, int xOffset, int yOffset)
{
	glVertex2i(x + xOffset, y + yOffset);
	glVertex2i(x - xOffset, y + yOffset);
	glVertex2i(x + xOffset, y - yOffset);
	glVertex2i(x - xOffset, y - yOffset);
	glVertex2i(x + yOffset, y + xOffset);
	glVertex2i(x - yOffset, y + xOffset);
	glVertex2i(x + yOffset, y - xOffset);
	glVertex2i(x - yOffset, y - xOffset);
}

int scale_func(int i)
{
	if (WINDOW_WIDTH > WINDOW_HEIGHT)
		i *= WINDOW_HEIGHT;
	else
		i *= WINDOW_WIDTH;
	i /= SCALE;
	return i;
}