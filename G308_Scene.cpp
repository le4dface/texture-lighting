/*
 * G308_Scene.cpp
 *
 *  Created on: 16/09/2014
 *      Author: reuben
 */

#include <string>
#include <stdlib.h>
#include <GL/glut.h>
#include "define.h"
#include <math.h>
#include <map>
#include "G308_Geometry.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace std;

GLuint g_mainWnd;
GLuint g_nWinWidth  = G308_WIN_WIDTH; //window width
GLuint g_nWinHeight = G308_WIN_HEIGHT; //window height
int num_models; //number of models/argument count

// Global Variables
G308_Geometry* sphere = NULL; //our first model to render
G308_Geometry* bunny = NULL; //our second model to render
G308_Geometry* teapot = NULL; //our third model to render
G308_Geometry* torus = NULL; //our seventh model to render
G308_Geometry* table = NULL; //our fifth model to render
G308_Geometry* box = NULL; //our sixth model to render


GLfloat direction[4] = {0.0f, 0.0f, 1.0f, 0.0f}; //light direction for manipulating with mouse
GLfloat diffuse[4] = {0.9f, 0.3f, 0.7f, 1.0f}; //diffuse intensity
GLfloat ambient[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //ambient intensity
GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //speculary intensity

glm::vec3 scale = {1.0f,1.0f,1.0f}; //scale of model for manipulating with mouse scroll

//File IO methods
void loadError(int);
void loadAllObjects(char**);
void cleanup();
void checkMinObjects();
void checkMaxObjects();

//Main methods
void G308_SetLight();
void G308_SetCamera();
void G308_Mouse(int,int,int,int);
void G308_Keyboard(int,int,int);
void G308_Mouse(int,int,int,int);
void G308_Display();
void G308_Reshape(int,int);


void loadError(int num) {
	printf("not enough arguments: %d. requires 7", num);
	exit(EXIT_FAILURE);
}
void loadAllObjects(char** argv) {
	printf("num models %d\n", num_models);
	if (num_models > 1) {
		sphere = new G308_Geometry;
		sphere->ReadOBJ(argv[1]); // 1) read OBJ function
		sphere->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		sphere->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
	}

	if(num_models > 2) {
		bunny = new G308_Geometry;
		bunny->ReadOBJ(argv[2]); // 1) read OBJ function
		bunny->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		bunny->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
	}
	if(num_models > 3) {
		teapot = new G308_Geometry;
		teapot->ReadOBJ(argv[3]); // 1) read OBJ function
		teapot->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		teapot->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
	}

	if(num_models > 4) {
		torus = new G308_Geometry;
		torus->ReadOBJ(argv[4]); // 1) read OBJ function
		torus->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		torus->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
	}

	if(num_models > 5) {
		table = new G308_Geometry;
		table->ReadOBJ(argv[5]); // 1) read OBJ function
		table->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		table->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
	}

	if(num_models > 6) {
		box = new G308_Geometry;
		box->ReadOBJ(argv[6]); // 1) read OBJ function
		box->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		box->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
	}

}
void cleanup() {

	if (sphere != NULL)
		delete sphere;

	if (bunny != NULL)
		delete bunny;

	if (teapot != NULL)
		delete teapot;

	if (torus != NULL)
		delete torus;

	if (table != NULL)
		delete table;

	if (box != NULL)
		delete box;

	glFlush();
}


void checkMinObjects() {
	//commented out for test
	if (num_models < 2) {
		printf("Obj filename expected\n. something.obj\n");
		exit(EXIT_FAILURE);
	}
}

void checkMaxObjects() {
	//no more than 4 models
	if (num_models > 7) {
		printf("No more than 7 models in this rendering!");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv)
{
	//get number of models entered
	num_models = argc;
	//commented out for test
	checkMinObjects();
	//no more than 4 models
	checkMaxObjects();

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("COMP308 Assignment 3");

    glutDisplayFunc(G308_Display);
    glutReshapeFunc(G308_Reshape);
    glutSpecialFunc(G308_Keyboard);
    glutMouseFunc(G308_Mouse);

	loadAllObjects(argv);

	G308_SetLight();
	G308_SetCamera();
	glutMainLoop();

	//clean up wavefront objects
	cleanup();
    return 0;
}

void renderAllObjects() {
	glPushMatrix();
		sphere->RenderGeometry(); //render sphere image
	glPopMatrix();

	glPushMatrix();
		if (num_models > 2) bunny->RenderGeometry(); //render bunny image
	glPopMatrix();

	glPushMatrix();
		if (num_models > 3) teapot->RenderGeometry(); //render teapot image
	glPopMatrix();

	glPushMatrix();
		if (num_models > 4) torus->RenderGeometry(); //render torus image
	glPopMatrix();

	glPushMatrix();
		if (num_models > 5) table->RenderGeometry(); //render table image
	glPopMatrix();

	glPushMatrix();
		if (num_models > 6) box->RenderGeometry(); //render box image
	glPopMatrix();
}

// Display function
void G308_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE); //enabled GL normalize to automagically normalize vectors
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	//check these values for each object
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0f,0.0f,0.0f); /* set object color as red */
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0f );

	renderAllObjects();

	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

// Reshape function
void G308_Reshape(int w, int h)
{
    if (h == 0) h = 1;
	g_nWinWidth = w;
	g_nWinHeight = h;
    glViewport(0, 0, g_nWinWidth, g_nWinHeight);
}

// Set Light
void G308_SetLight()
{
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_LIGHT0);
}

void updateTransform(int key) {
	switch (key) {
	case 27:
		break;
	case 100:
		glRotatef(1.0f,0.0f,1.0f,0.0f);
		break;
	case 102:
		glRotatef(-1.0f,0.0f,1.0f,0.0f);
		break;
	case 101:
		glRotatef(1.0f,1.0f,0.0f,0.0f);
		break;
	case 103:
		glRotatef(-1.0f,1.0f,0.0f,0.0f);
		break;
	}
}


void G308_Keyboard(int key, int x, int y)
{
	updateTransform(key);
    G308_Display();
	glutPostRedisplay();
}

void toggleObjects() {
	sphere->toggleMode(); //toggle each model
	bunny->toggleMode();
	teapot->toggleMode();
	torus->toggleMode();
	table->toggleMode();
	box->toggleMode();
}

void G308_Mouse(int button, int state, int x, int y){

	if(button == GLUT_LEFT_BUTTON  && state == GLUT_DOWN){
		toggleObjects();
	}
	if(button == 3) { //wheel up
		glScalef(1.1f,1.1f,1.1f);
	} else if(button == 4) { //wheel down
		glScalef(0.9f,0.9f,0.9f);
	}
	G308_Display();
	glutPostRedisplay();

}
// Set Camera Position
void G308_SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 40.0, -2.0, 2.0, -25.0, 0.0, 1.0, 0.0);

}
