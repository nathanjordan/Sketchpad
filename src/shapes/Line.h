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
#include "../util/Utils.h"
#include "../math/vec4.h"
#include "../math/mat4.h"


class Line {
public:
	static const int dimension = 2;

	Line( );
	virtual ~Line();
	void draw();
	void setVertices( int , TVec4<GLfloat>[2] );
	void setColors( int , TVec4<GLfloat>[2] );
	void rotate( float );
	void buildMatrixUniform( GLfloat* arr , Mat4& matrix );

	Mat4 translationMatrix;
	TVec4<GLfloat> translationVec;
	Mat4 rotationMatrix;
	Mat4 scaleMatrix;
	Mat4 projectionMatrix;
	GLfloat friction;
	TVec4<GLfloat> vertices[2];
	TVec4<GLfloat> colors[2];
	GLfloat height,width;
	GLfloat boundingBox[4][4];

protected:
	
	GLuint program;
	GLuint vertexShader;
	GLuint fragShader;
	GLuint vao, vbo[2];

	int numVertices;
	};

#endif /* GL_SHAPE_H_ */
