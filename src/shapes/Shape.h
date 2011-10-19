/*
 * GL_Shape.h
 *
 *  Created on: Sep 21, 2011
 *      Author: njordan
 */

#ifndef GL_LINE_H_
#define GL_LINE_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <limits>
#include <iostream>
//#include "../util/Utils.h"
#include "../math/vec4.h"
#include "../math/mat4.h"
#include <math.h>


class Shape {

public:

	Shape( );
	virtual ~Shape();
	void draw();
	void setVertices( int , TVec4<GLfloat>* );
	void setColors( int , TVec4<GLfloat>* );
	void rotate( float );
	void calcBoundingBox();
	void drawBoundingBox();
	void checkSelection( int , int );

	TVec2<GLfloat> translationVec;
	TVec4<GLfloat>* vertices;
	TVec4<GLfloat>* colors;
	GLfloat height, width;
	TVec4<GLfloat>* boundingBox;
	int numVertices;
	float rotation;

protected:

	GLuint glDrawMode;
	GLuint program;
	GLuint vertexShader;
	GLuint fragShader;
	GLuint vao, vbo[2];
	GLint translationLocation;
	GLint rotationLocation;
	GLint scaleLocation;
	GLint vertexLocation;
	GLint colorLocation;

	};

#endif /* GL_SHAPE_H_ */
