//============================================================================
// Name        : Sketchpad.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Utilities
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stack>
#include <vector>

//my classes
#include "shapes/Line.h"
#include "shapes/Shape.h"
#include "math/mat4.h"
#include "math/vec4.h"

//OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

//OpenAL
#include <AL/al.h>
#include <AL/alut.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////
////    Global Variables
//////////////////////////////////////////////////////////////////////////

//stack for matrix operations
stack<Mat4> matrixStack;

Mat4 projectionMatrix;

vector<Line*> shapeList;
vector<Shape*> shapeList2;

Line* currentShape;

int originX,originY;

int drawType;

//////////////////////////////////////////////////////////////////////////
////    Constants
//////////////////////////////////////////////////////////////////////////

//Modes
const int LINE_MODE = 0;
const int SQUARE_MODE = 1;

//Menu Items
const int MENU_LINES = 0;
const int MENU_SQUARES = 1;
const int MENU_SAVE = 2;
const int MENU_LOAD = 3;

//////////////////////////////////////////////////////////////////////////
////    Function Declarations
//////////////////////////////////////////////////////////////////////////

/*
 * Initializes OpenGL context and window stuff
 */
void initOpenGL(int argc , char** argv);

/*
 * Creates the menus
 */
void initMenus();

/*
 * Runs when an item is selected in the main menu
 */
void mainMenuCallback( int val );

/*
 * Runs every 15ms and updates object positions and
 * checks for scores and collisions
 */
void timerTick( int value );

/*
 * The display callback function, draws the objects
 */
void displayCallback();

/*
 * Saves the layout
 */
void save();

/*
 * loads the layout
 */
void load();

/*
 * Handles mouse events
 */
void mouseButtonHandler( int button, int state, int x, int y);

/*
 * Handles mouse movement
 */
void mouseMoveHandler(int x, int y);

//////////////////////////////////////////////////////////////////////////
////    Main Function
//////////////////////////////////////////////////////////////////////////

int main(int argc , char** argv) {

	initOpenGL(argc,argv);

/*	TVec4<GLfloat>* one = new TVec4<GLfloat>(0.0,0.0,0.0,1.0);
	TVec4<GLfloat>* two = new TVec4<GLfloat>(30.0,0.0,0.0,1.0);
	TVec4<GLfloat>* three = new TVec4<GLfloat>(30.0,60.0,0.0,1.0);
	TVec4<GLfloat>* four = new TVec4<GLfloat>(0.0,60.0,0.0,1.0);

	TVec4<GLfloat>* arr = new TVec4<GLfloat>[4];

	Shape* jjj = new Shape();

	arr[0] = *one;
	arr[1] = *two;
	arr[2] = *three;
	arr[3] = *four;

	jjj->setVertices( 4 , arr );

	jjj->setColors( 4 , arr );

	shapeList2.insert( shapeList2.begin() , jjj );*/



	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, 500, 0, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();


	//Run the program
	glutMainLoop();

	return 0;
	}

//////////////////////////////////////////////////////////////////////////
////    Function Definitions
//////////////////////////////////////////////////////////////////////////

void initOpenGL(int argc , char** argv) {

	glutInit(&argc,argv);

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );

	glutInitWindowSize(500,500);

	glutInitWindowPosition(0, 0);

	glutCreateWindow("Sketchpad");

	glutDisplayFunc( displayCallback );

	projectionMatrix[0][0] = 2.0 / 500.0;
	projectionMatrix[1][1] = - 2.0 / 500.0;
	projectionMatrix[2][2] = - 1.0;
	projectionMatrix[3][3] = 1.0;

	projectionMatrix[0][3] = -1.0;
	projectionMatrix[1][3] = 1.0;
	projectionMatrix[2][3] = 0.0;

	glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

	glewExperimental = GL_TRUE;

	glLineWidth(5.0);

	//glutSetCursor( GLUT_CURSOR_NONE );

	glutMouseFunc(mouseButtonHandler);

	glutMotionFunc(mouseMoveHandler);

	glewInit();

	glutTimerFunc( 15 , timerTick , 0 );

	}

void timerTick( int value ) {

	//redraw the screen
	glutPostRedisplay();

	//come back in 15ms
	glutTimerFunc( 15 , timerTick , 0 );

	}

void displayCallback() {

	//clear the background to black
	glClearColor(1.0, 1.0 , 0.0 , 1.0 );

	glClear(GL_COLOR_BUFFER_BIT);

	//draw all the shapes
	for( unsigned int i = 0 ; i < shapeList.size() ; i++ ) {

		shapeList[i]->draw();

		}

	//draw all the shapes
	for( unsigned int i = 0 ; i < shapeList2.size() ; i++ ) {

		shapeList2[i]->draw();

		}

	//show the changes
	glutSwapBuffers();

	}

void mouseMoveHandler(int x, int y) {

	TVec4<GLfloat> vertices[2];
	TVec4<GLfloat> second( x - originX , originY - y , 0.0 , 1.0 );

	vertices[0] = currentShape->vertices[0];
	vertices[1] = second;

	currentShape->setVertices( 2 , vertices );

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble mv[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);

	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	GLfloat winX = (float)x;
	GLfloat winY = (float)viewport[3] - (float)y;
	GLfloat winZ;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	double worldX,worldY,worldZ;

	gluUnProject(winX,winY,winZ,mv,projection,viewport,&worldX,&worldY,&worldZ);

	}

void mouseButtonHandler( int button, int state, int x, int y) {

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		GLdouble projection[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		GLdouble mv[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);

		GLint viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );

		GLfloat winX = (float)x;
		GLfloat winY = (float)viewport[3] - (float)y;
		GLfloat winZ;
		glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		double worldX,worldY,worldZ;

		gluUnProject(winX,winY,winZ,mv,projection,viewport,&worldX,&worldY,&worldZ);

		TVec4<GLfloat> vertices[2];
		TVec4<GLfloat> colors[2];

		Line* tempLine = new Line();

		TVec4<GLfloat> begin( 0.0 , 0.0 , 0.0 , 1.0 );
		TVec4<GLfloat> end( 0.0 , 0.0 , 0.0 , 1.0 );

		vertices[0] = begin;
		vertices[1] = end;

		tempLine->setVertices( 2 , vertices );

		TVec4<GLfloat> color1( 0.0 , 1.0 , 1.0 , 1.0 );
		TVec4<GLfloat> color2( 0.0 , 1.0 , 1.0 , 1.0 );

		colors[0] = color1;
		colors[1] = color2;

		tempLine->setColors( 2 , colors );

		Mat4 translationMatrix;

		translationMatrix = translationMatrix.I();

		translationMatrix[0][3] = worldX;
		translationMatrix[1][3] = worldY;
		translationMatrix[2][3] = 0.0;

		TVec4<GLfloat> translationVec;

		translationVec[0] = worldX;
		translationVec[1] = worldY;

		tempLine->translationVec = translationVec;

		tempLine->translationMatrix = translationMatrix;

		tempLine->projectionMatrix = projectionMatrix;

		currentShape = tempLine;

		shapeList.insert( shapeList.begin() , tempLine );

		originX = x;
		originY = y;

		}

	else if( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) {

		originX = originX;

		}

	}

void initMenus() {

	glutCreateMenu( mainMenuCallback );

	glutAddMenuEntry( "Lines" , MENU_LINES );

	glutAddMenuEntry( "Squares" , MENU_SQUARES );

	glutAddMenuEntry( "Save" , MENU_SAVE );

	glutAddMenuEntry( "Load" , MENU_LOAD );

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	}

void mainMenuCallback( int val ) {

	if( val == MENU_LINES ) {

		}

	else if( val == MENU_SQUARES ) {

		}

	else if( val == MENU_SAVE ) {

		}

	else if( val == MENU_LOAD ) {

		}

	}

void save() {

	}

void load() {

	}

