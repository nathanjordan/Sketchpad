/*
 * SingleLineFunctions.cpp
 *
 *  Created on: Oct 15, 2011
 *      Author: njordan
 *
 */

void polygonDrawMoveHandler( int x , int y) {

	currentShape->vertices[ currentShape->numVertices - 1 ] = TVec4<GLfloat>( x - originX , y - originY , 0.0 , 1.0 );

	}

void polygonDrawEnterHandler(unsigned char key , int x , int y ) {

	TVec4<GLfloat>* tempVerts = new TVec4<GLfloat>[ currentShape->numVertices + 1 ];

	for( int i = 0 ; i < currentShape->numVertices ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			tempVerts[i][j] = currentShape->vertices[i][j];

			}

		}

	TVec4<GLfloat>* tempColors = new TVec4<GLfloat>[ currentShape->numVertices + 1 ];

	for( int i = 0 ; i < currentShape->numVertices ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			tempColors[i][j] = currentShape->colors[i][j];

			}

		}

	tempVerts[currentShape->numVertices][0] = tempVerts[0][0];
	tempVerts[currentShape->numVertices][1] = tempVerts[0][1];
	tempVerts[currentShape->numVertices][2] = 0.0;
	tempVerts[currentShape->numVertices][3] = 1.0;

	tempColors[currentShape->numVertices] = currentColor;

	currentShape->setVertices( currentShape->numVertices + 1 , tempVerts );

	currentShape->setColors( currentShape->numVertices , tempColors );

	currentShape = 0;

	}

void polygonDrawClickHandler( int x , int y) {

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

	if(!currentShape) {

		if( drawMode == POLYGON_MODE )
			currentShape = new Polygon();
		else if(drawMode == FILLED_POLYGON_MODE)
			currentShape = new FilledPolygon();

		originX = x;
		originY = y;

		TVec4<GLfloat>* vertices = new TVec4<GLfloat>[1];
		TVec4<GLfloat>* colors = new TVec4<GLfloat>[1];

		vertices[0] = TVec4<GLfloat>( 0.0 , 0.0 , 0.0 , 1.0 );

		colors[0] = currentColor;

		currentShape->setVertices( 1 , vertices );

		currentShape->setColors( 1 , colors );

		TVec2<GLfloat> translationVec;

		translationVec[0] = worldX;
		translationVec[1] = worldY;

		currentShape->translationVec = translationVec;

		shapeList.insert( shapeList.begin() , currentShape );

		}

	TVec4<GLfloat>* tempVerts = new TVec4<GLfloat>[ currentShape->numVertices + 1 ];

	for( int i = 0 ; i < currentShape->numVertices ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			tempVerts[i][j] = currentShape->vertices[i][j];

			}

		}

	TVec4<GLfloat>* tempColors = new TVec4<GLfloat>[ currentShape->numVertices + 1 ];

	for( int i = 0 ; i < currentShape->numVertices ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			tempColors[i][j] = currentShape->colors[i][j];

			}

		}

	tempVerts[currentShape->numVertices][0] = x - originX;
	tempVerts[currentShape->numVertices][1] = y - originY;
	tempVerts[currentShape->numVertices][2] = 0.0;
	tempVerts[currentShape->numVertices][3] = 1.0;

	tempColors[currentShape->numVertices] = currentColor;

	currentShape->setVertices( currentShape->numVertices + 1 , tempVerts );

	currentShape->setColors( currentShape->numVertices , tempColors );
	}
