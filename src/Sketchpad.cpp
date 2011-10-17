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
#include "shapes/Shape.h"
#include "shapes/SingleLine.h"
#include "shapes/Polygon.h"
#include "shapes/FilledPolygon.h"
#include "shapes/Rect.h"
#include "shapes/FilledRect.h"
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

vector<Shape*> shapeList;
vector<Shape*> shapeList2;

Shape* currentShape;

int originX,originY;

int drawMode;
int sketchpadMode;

GLuint drawMenu;
GLuint selectMenu;
GLuint colorsMenu;

TVec4<GLfloat> currentColor;

//////////////////////////////////////////////////////////////////////////
////    Constants
//////////////////////////////////////////////////////////////////////////

//Drawing Modes
const int LINE_MODE = 0;
const int POLYLINE_MODE = 1;
const int POLYGON_MODE = 2;
const int RECT_MODE = 3;
const int FILLED_POLYGON_MODE = 4;
const int FILLED_RECT_MODE = 5;
const int POINT_MODE = 6;

//Sketchpad modes
const int SELECT_MODE = 0;
const int DRAW_MODE = 1;
const int EDIT_MODE = 2;
const int COPY_MODE = 3;
const int CUT_MODE = 4;
const int TRANSLATE_MODE = 5;
const int ROTATE_MODE = 6;
const int SCALE_MODE = 7;

//Menu Items
const int MENU_SWITCH_SELECT = 12;
const int MENU_SWITCH_DRAW = 13;

const int MENU_DRAW_LINE = 0;
const int MENU_DRAW_POLYGON = 1;
const int MENU_DRAW_FILLED_POLYGON = 2;
const int MENU_DRAW_RECT = 3;
const int MENU_DRAW_FILLED_RECT = 4;

const int MENU_COLOR_RED = 0;
const int MENU_COLOR_GREEN = 1;
const int MENU_COLOR_BLUE = 2;
const int MENU_COLOR_CYAN = 3;
const int MENU_COLOR_MAGENTA = 4;
const int MENU_COLOR_YELLOW = 5;
const int MENU_COLORS = 6;

//Color constants
const TVec4<GLfloat> COLOR_RED = TVec4<GLfloat>(1.0,0.0,0.0,1.0);
const TVec4<GLfloat> COLOR_GREEN = TVec4<GLfloat>(0.0,1.0,0.0,1.0);
const TVec4<GLfloat> COLOR_BLUE = TVec4<GLfloat>(0.0,0.0,1.0,1.0);
const TVec4<GLfloat> COLOR_YELLOW = TVec4<GLfloat>(1.0,1.0,0.0,1.0);
const TVec4<GLfloat> COLOR_CYAN = TVec4<GLfloat>(0.0,1.0,1.0,1.0);
const TVec4<GLfloat> COLOR_MAGENTA = TVec4<GLfloat>(1.0,0.0,1.0,1.0);

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
void drawMenuCallback( int val );
void colorsMenuCallback( int val );
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


//Event function includes
#include "draw/SingleLineFunctions.h"
#include "draw/PolygonFunctions.h"
#include "draw/RectFunctions.h"

//////////////////////////////////////////////////////////////////////////
////    Main Function
//////////////////////////////////////////////////////////////////////////

int main(int argc , char** argv) {

	initOpenGL(argc,argv);

	initMenus();

	currentColor = COLOR_CYAN;

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

	glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

	glewExperimental = GL_TRUE;

	glLineWidth(5.0);

	glutMouseFunc(mouseButtonHandler);

	glutPassiveMotionFunc(mouseMoveHandler);

	glewInit();

	glOrtho(0.0,500.0,500.0,0.0,-1.0,1.0);

	glutKeyboardFunc( polygonDrawEnterHandler );

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
	glClearColor(0.0, 0.0 , 0.0 , 1.0 );

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

	if( !currentShape )
		return;

	if( drawMode == LINE_MODE)
		singleLineDrawModeHandler( x , y);

	else if ( drawMode == POLYGON_MODE || drawMode == FILLED_POLYGON_MODE )
		polygonDrawMoveHandler(x,y);

	else if( drawMode == RECT_MODE || drawMode == FILLED_RECT_MODE )
		rectDrawMoveHandler(x,y);


	}


void mouseButtonHandler( int button, int state, int x, int y) {

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		if( drawMode == LINE_MODE )
			singleLineDrawClickHandler( x , y );


		else if( drawMode == POLYGON_MODE || drawMode == FILLED_POLYGON_MODE )
			polygonDrawClickHandler(x,y);


		else if( drawMode == RECT_MODE || drawMode == FILLED_RECT_MODE )
			rectDrawClickHandler(x,y);
		}

	else if( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ) {


		}

	}

void initMenus() {

	colorsMenu = glutCreateMenu( colorsMenuCallback );

	glutSetMenu(colorsMenu);

	glutAddMenuEntry( "Red" , MENU_COLOR_RED );

	glutAddMenuEntry( "Green" , MENU_COLOR_GREEN );

	glutAddMenuEntry( "Blue" , MENU_COLOR_BLUE );

	glutAddMenuEntry( "Cyan" , MENU_COLOR_CYAN );

	glutAddMenuEntry( "Magenta" , MENU_COLOR_MAGENTA );

	glutAddMenuEntry( "Yellow" , MENU_COLOR_YELLOW );

	drawMenu = glutCreateMenu( drawMenuCallback );

	glutSetMenu(drawMenu);

	glutAddMenuEntry( "Single Line" , MENU_DRAW_LINE );

	glutAddMenuEntry( "Polygon" , MENU_DRAW_POLYGON );

	glutAddMenuEntry( "Filled Polygon" , MENU_DRAW_FILLED_POLYGON );

	glutAddMenuEntry( "Rectangle" , MENU_DRAW_RECT );

	glutAddMenuEntry( "Filled Rectangle" , MENU_DRAW_FILLED_RECT );

	glutAddSubMenu( "Colors", colorsMenu );

	glutAddMenuEntry( "Select Mode" , MENU_SWITCH_SELECT );

	glutAttachMenu( GLUT_RIGHT_BUTTON );

	/*selectMenu = glutCreateMenu( selectMenuCallback );

	glutSetMenu(drawMenu);

	glutAddMenuEntry( "Single Line" , MENU_DRAW_LINE );

	glutAddMenuEntry( "Polygon" , MENU_DRAW_POLYGON );

	glutAddMenuEntry( "Filled Polygon" , MENU_DRAW_FILLED_POLYGON );

	glutAddMenuEntry( "Rectangle" , MENU_DRAW_RECT );

	glutAddMenuEntry( "Filled Rectangle" , MENU_DRAW_FILLED_RECT );*/

	}
void colorsMenuCallback( int val ) {

	if(val == MENU_COLOR_RED)
		currentColor = COLOR_RED;
	if(val == MENU_COLOR_GREEN)
		currentColor = COLOR_GREEN;
	if(val == MENU_COLOR_BLUE)
		currentColor = COLOR_BLUE;
	if(val == MENU_COLOR_CYAN)
		currentColor = COLOR_CYAN;
	if(val == MENU_COLOR_MAGENTA)
		currentColor = COLOR_MAGENTA;
	if(val == MENU_COLOR_YELLOW)
		currentColor = COLOR_YELLOW;

	}
void drawMenuCallback( int val ) {

	if( val == MENU_DRAW_LINE ) {
		drawMode = LINE_MODE;
		}

	else if( val == MENU_DRAW_POLYGON ) {
		drawMode = POLYGON_MODE;
		}

	else if( val == MENU_DRAW_FILLED_POLYGON ) {
		drawMode = FILLED_POLYGON_MODE;
		}

	else if( val == MENU_DRAW_RECT ) {
		drawMode = RECT_MODE;
		}

	else if( val == MENU_DRAW_FILLED_RECT ) {
		drawMode = FILLED_RECT_MODE;
		}
	else if( val == MENU_SWITCH_SELECT ) {

		glutSetMenu(selectMenu);

		glutAttachMenu(GLUT_RIGHT_BUTTON);

		sketchpadMode = SELECT_MODE;
		}
	}

void save() {

	}

void load() {

	}

