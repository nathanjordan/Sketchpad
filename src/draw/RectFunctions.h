/*
 * RectFunctions.h
 *
 *  Created on: Oct 16, 2011
 *      Author: njordan
 */

#ifndef RECTFUNCTIONS_H_
#define RECTFUNCTIONS_H_


void rectDrawMoveHandler( int x , int y) {

	currentShape->vertices[1][0] = x - originX;

	currentShape->vertices[2][0] = x - originX;

	currentShape->vertices[2][1] = y - originY;

	currentShape->vertices[3][1] = y - originY;

	}

void rectDrawClickHandler( int x , int y) {

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

	if(currentShape) {
		currentShape = 0;
		return;
		}
	else {

		if( drawMode == RECT_MODE )
			currentShape = new Rect();
		else if( drawMode == FILLED_RECT_MODE )
			currentShape = new FilledRect();

		originX = x;
		originY = y;

		TVec4<GLfloat>* vertices = new TVec4<GLfloat>[5];
		TVec4<GLfloat>* colors = new TVec4<GLfloat>[5];

		vertices[0] = TVec4<GLfloat>( 0.0 , 0.0 , 0.0 , 1.0 );
		vertices[1] = TVec4<GLfloat>( 0.0 , 0.0 , 0.0 , 1.0 );
		vertices[2] = TVec4<GLfloat>( 0.0 , 0.0 , 0.0 , 1.0 );
		vertices[3] = TVec4<GLfloat>( 0.0 , 0.0 , 0.0 , 1.0 );
		vertices[4] = TVec4<GLfloat>( 0.0 , 0.0 , 0.0 , 1.0 );

		colors[0] = currentColor;
		colors[1] = currentColor;
		colors[2] = currentColor;
		colors[3] = currentColor;
		colors[4] = currentColor;

		currentShape->setVertices( 5 , vertices );

		currentShape->setColors( 5 , colors );

		TVec2<GLfloat> translationVec;

		translationVec[0] = worldX;
		translationVec[1] = worldY;

		currentShape->translationVec = translationVec;

		shapeList.insert( shapeList.begin() , currentShape );

		}

	}

#endif /* RECTFUNCTIONS_H_ */
