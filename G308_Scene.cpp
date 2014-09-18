/*
 * G308_Scene.cpp
 *
 *  Created on: 16/09/2014
 *      Author: reuben
 */


#include <stdlib.h>
#include <GL/glut.h>
#include "define.h"
#include <math.h>
#include <map>
#include "G308_Geometry.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <string.h>
#include <stdio.h>

using namespace std;

GLuint g_mainWnd;
GLuint g_nWinWidth  = G308_WIN_WIDTH; //window width
GLuint g_nWinHeight = G308_WIN_HEIGHT; //window height
int num_models; //number of models/argument count
char* selected = "";

// Global Variables
G308_Geometry* sphere = NULL; //our first model to render
G308_Geometry* bunny = NULL; //our second model to render
G308_Geometry* teapot = NULL; //our third model to render
G308_Geometry* torus = NULL; //our seventh model to render
G308_Geometry* table = NULL; //our fifth model to render
G308_Geometry* box = NULL; //our sixth model to render
glm::vec3 cameraRotation;
glm::vec3 eye = {25.0f, 15.0f, 50.0f};
glm::vec3 focus = {0.0f, 0.0f, 0.0f};

GLfloat direction[4] = {0.0f, 0.0f, 1.0f, 0.0f}; //light direction for manipulating with mouse
GLfloat diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //diffuse intensity
GLfloat ambient[4] = {0.0f, 0.0f, 0.0f, 1.0f}; //ambient intensity
GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //speculary intensity

GLfloat no_mat[4] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat mat_ambient[4] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat mat_ambient_color[4] = {0.8f, 0.8f, 0.2f, 1.0f};
GLfloat mat_diffuse[4] = {0.1f, 0.5f, 0.8f, 1.0f};
GLfloat mat_specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat no_shininess = 0.0f;
GLfloat low_shininess = 5.0f;
GLfloat high_shininess = 100.0f;
GLfloat mat_emission[4] = {0.3f, 0.2f, 0.2f, 0.0f};

glm::vec3 scale = {1.0f,1.0f,1.0f}; //scale of model for manipulating with mouse scroll

//File IO methods
void loadError(int);
void loadAllObjects(char**);
void cleanup();
void checkMinObjects();
void checkMaxObjects();
void pickByColor(int, int);
void selectByPixel(G308_Geometry*, unsigned char *);
void colorObject(G308_Geometry*);

//Main methods
void G308_SetLight();
void G308_SetCamera();
void G308_Mouse(int,int,int,int);
void G308_Keyboard(unsigned char key, int x, int y);
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
		sphere->cid = {0,200,0};
		sphere->name = "sphere";
		sphere->translation = {1,1,15};
		sphere->scale = {1,1,1};
	}

	if(num_models > 2) {
		bunny = new G308_Geometry;
		bunny->ReadOBJ(argv[2]); // 1) read OBJ function
		bunny->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		bunny->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
		bunny->cid = {200,200,200};
		bunny->name = "bunny";
		bunny->translation = {6,1,10};
		bunny->scale = {1,1,1};
	}
	if(num_models > 3) {
		teapot = new G308_Geometry;
		teapot->ReadOBJ(argv[3]); // 1) read OBJ function
		teapot->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		teapot->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
		teapot->cid = {0,50,200};
		teapot->name = "teapot";
		teapot->translation = {0,1,2};
		teapot->scale = {1,1,1};
	}

	if(num_models > 4) {
		torus = new G308_Geometry;
		torus->ReadOBJ(argv[4]); // 1) read OBJ function
		torus->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		torus->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
		torus->cid = {255,0,0};
		torus->name = "torus";
		torus->translation = {10,1,14};
		torus->scale = {1,1,1};
	}

	if(num_models > 5) {
		table = new G308_Geometry;
		table->ReadOBJ(argv[5]); // 1) read OBJ function
		table->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		table->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
		table->cid = {50,0,50};
		table->name = "table";
		table->translation = {9,-1,9};
		table->scale = {1.578, 1.578, 1.578};

	}

	if(num_models > 6) {
		box = new G308_Geometry;
		box->ReadOBJ(argv[6]); // 1) read OBJ function
		box->CreateGLPolyGeometry(); // 2) create GL Geometry as polygon
		box->CreateGLWireGeometry(); // 3) create GL Geometry as wireframe
		box->cid = {255,255,0};
		box->name = "box";
		box->translation = {11,3,3};
		box->scale = {0.9, 0.9, 0.9};
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
	cameraRotation.x = 0;
	cameraRotation.y = 0;
	cameraRotation.z = 0;
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
    glutKeyboardFunc(G308_Keyboard);
    glutMouseFunc(G308_Mouse);
	G308_SetLight();
	G308_SetCamera();

	loadAllObjects(argv);


	glutMainLoop();

	//clean up wavefront objects
	cleanup();
    return 0;
}



G308_Geometry* getSelectedObj() {

	if(strcmp(selected,"sphere") == 0) {
		return sphere;
	} else if(strcmp(selected,"teapot") == 0) {
		return teapot;
	} else if(strcmp(selected,"box") == 0) {
		return box;
	} else if(strcmp(selected,"table") == 0) {
		return table;
	} else if(strcmp(selected,"torus") == 0) {
		return torus;
	} else if(strcmp(selected,"bunny") == 0) {
		return bunny;
	}
	return NULL;

}

void setObjColor(G308_Geometry* obj) {
	glColor3f(
			(float)obj->cid.r  / 255.0f,
			(float)obj->cid.g  / 255.0f,
			(float)obj->cid.b  / 255.0f
			);
}

void colorObject(G308_Geometry* obj) {
	if(strcmp(selected,obj->name) == 0)
		glColor3f(1.0,0.0,1.0);
	else
		setObjColor(obj);
}

void translateObject(G308_Geometry* obj) {
	glTranslatef(
			obj->translation.x,
			obj->translation.y,
			obj->translation.z
			);
}

void scaleObject(G308_Geometry* obj) {
	glScalef(
		obj->scale.x,
		obj->scale.y,
		obj->scale.z
		);
}

void renderSingleObject(G308_Geometry* obj) {
	glPushMatrix();
		//translate obj
		translateObject(obj);
		//scale obj
		scaleObject(obj);
		//color obj
		colorObject(obj);
		//draw obj
		obj->RenderGeometry(); //render sphere image
	glPopMatrix();
}

void renderAllObjects() {
	renderSingleObject(sphere);
	renderSingleObject(bunny);
	renderSingleObject(teapot);
	renderSingleObject(torus);
	renderSingleObject(table);
	renderSingleObject(box);
}

// Display function
void G308_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE); //enabled GL normalize to automagically normalize vectors

	//check these values for each object
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	renderAllObjects();

	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_SMOOTH);

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


void G308_Keyboard(unsigned char key, int x, int y)
{
	if(strcmp(selected,"") != 0) {
		G308_Geometry* obj = getSelectedObj();
		switch (key) {
			case 'a':
				printf("here");
//				cameraRotation.y +=1;
				glRotatef(1,0,1,0);
				break;
			case 'd':
				glRotatef(-1,0,1,0);
				break;
			case 'w':
				glRotatef(1,1,0,0);
				break;
			case 's':
				glRotatef(-1,1,0,0);
				break;
			case 'q':
				glRotatef(1,0,0,1);
				break;
			case 'e':
				glRotatef(-1,0,0,1);
			case '8':
				obj->translation.z -= 1;
				break;
			case '2':
				obj->translation.z += 1;
				break;
			case '4':
				obj->translation.x -= 1;
				break;
			case '6':
				obj->translation.x += 1;
				break;
			case '+':
				obj->translation.y += 1;
				break;
			case '-':
				obj->translation.y -= 1;
				break;
			case '*':
				obj->scale.x *= 1.1;
				obj->scale.y *= 1.1;
				obj->scale.z *= 1.1;
				break;
			case '/':
				obj->scale.x *= 0.9;
				obj->scale.y *= 0.9;
				obj->scale.z *= 0.9;
				break;
			case '0':
				//dump position and scale properties to console
				sphere->toString();
				teapot->toString();
				bunny->toString();
				table->toString();
				box->toString();
				torus->toString();
		}
	}

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
//		toggleObjects();
		if(button == 0) {
			pickByColor(x, y);
		}
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
	gluLookAt(eye.x, eye.y, eye.z, focus.x, focus.y, focus.z, 0.0, 1.0, 0.0);
}

void pickByColor(int x, int y) {
	//turn off texturing, lighting and fog
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_LIGHT0);
		unsigned char pixel[3];
		renderAllObjects();
//		glEnable(GL_SCISSOR_TEST);
//		glScissor(x, y, 1, 1);
		glReadPixels(x, g_nWinHeight - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE,
				pixel);
		glEnable(GL_LIGHT0);
//		glDisable(GL_SCISSOR_TEST);
		//find currently select object by pixel color signature
		selectByPixel(bunny,pixel);
		selectByPixel(sphere,pixel);
		selectByPixel(teapot,pixel);
		selectByPixel(torus,pixel);
		selectByPixel(table,pixel);
		selectByPixel(box,pixel);


}

void selectByPixel(G308_Geometry* obj, unsigned char * pixel) {

	if ((obj->cid.r) == (pixel[0]) && (obj->cid.g) == (pixel[1])
					&& (obj->cid.b) == (pixel[2])) {
			selected = obj->name;
	}

}


