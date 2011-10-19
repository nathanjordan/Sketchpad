/*
 * SingleLineFunctions.cpp
 *
 *  Created on: Oct 15, 2011
 *      Author: njordan
 *
 */

void singleLineDrawModeHandler( int x , int y) {
	TVec4<GLfloat> vertices[2];
	TVec4<GLfloat> second( ( x - originX ) , ( y - originY ) , 0.0 , 1.0 );

	vertices[0] = currentShape->vertices[0];
	vertices[1] = second;

	currentShape->setVertices( 2 , vertices );
	}

void singleLineDrawClickHandler( int x , int y) {

	if( currentShape ) {
		currentShape = 0;
		return;
		}

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

	SingleLine* tempLine = new SingleLine();

	TVec4<GLfloat> begin( 0.0 , 0.0 , 0.0 , 1.0 );
	TVec4<GLfloat> end( 0.0 , 0.0 , 0.0 , 1.0 );

	vertices[0] = begin;
	vertices[1] = end;

	tempLine->setVertices( 2 , vertices );

	colors[0] = currentColor;
	colors[1] = currentColor;

	tempLine->setColors( 2 , colors );

	TVec2<GLfloat> translationVec;

	translationVec[0] = worldX;
	translationVec[1] = worldY;

	tempLine->translationVec = translationVec;

	currentShape = tempLine;

	shapeList.insert( shapeList.begin() , tempLine );

	originX = x;
	originY = y;
	}
