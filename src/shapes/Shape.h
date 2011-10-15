/*
 * GL_Shape.h
 *
 *  Created on: Sep 21, 2011
 *      Author: njordan
 */

#ifndef GL_SHAPE_H_
#define GL_SHAPE_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <limits>
#include <iostream>
#include "../util/Utils.h"
#include "../math/vec4.h"
#include "../math/mat4.h"


class Shape {
public:
	static const int dimension = 2;

	Shape();
	virtual ~Shape();
	void draw();
	void setVertices( int , TVec4<GLfloat>* );
	void setColors( int , TVec4<GLfloat>* );
	void rotate( float );

	Mat4 translationMatrix;
	Mat4 rotationMatrix;
	Mat4 scaleMatrix;
	Mat4 projectionMatrix;
	GLfloat friction;
	TVec4<GLfloat>* vertices;
	TVec4<GLfloat>* colors;
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
