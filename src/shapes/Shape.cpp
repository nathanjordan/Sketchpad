/*
 * GL_Shape.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: njordan
 */

#include "Shape.h"

Shape::Shape( ) {
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	vertices = 0;
	colors = 0;
	rotation = 0;

	boundingBox = new TVec4<GLfloat>[4];

	GLchar *vertexsource, *fragmentsource;

	vertexsource = Utils::parseGLSL((char*)"/home/njordan/Dropbox/UNR/CS480/Sketchpad/src/shaders/shapes.vert");
	fragmentsource = Utils::parseGLSL((char*)"/home/njordan/Dropbox/UNR/CS480/Sketchpad/src/shaders/shapes.frag");

	glShaderSource( vertexShader , 1 , (const GLchar**)&vertexsource , NULL );
	glShaderSource( fragShader , 1 , (const GLchar**)&fragmentsource , NULL );

	glCompileShader( vertexShader );
	glCompileShader( fragShader );

	int isCompiled_vertexShader;

	//get the shader status from the vertex shader
	glGetShaderiv( vertexShader , GL_COMPILE_STATUS, &isCompiled_vertexShader );

	//if the vertex shader did not compile, get some info about why it didnt
	if( ! (bool) isCompiled_vertexShader ) {

		char *vertexInfoLog;
		int maxLength,logLen;

		//Get the shader logs length
		glGetShaderiv( vertexShader , GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		vertexInfoLog = (char *)malloc(maxLength);

		//get the log info
		glGetShaderInfoLog( vertexShader , maxLength , &logLen , vertexInfoLog );

		std::string log( vertexInfoLog , logLen );

		std::cout << "ERROR COMPILING VERTEX SHADER: \n\n" << log;

		//exit indicating error
		exit(1);

		}

	int isCompiled_fragmentShader;

	//get the shader status from the fragment shader
	glGetShaderiv( fragShader , GL_COMPILE_STATUS, &isCompiled_fragmentShader );

	//if the fragment shader did not compile, get some info about why it didnt
	if( ! (bool) isCompiled_fragmentShader ) {

		char *fragmentInfoLog;
		int maxLength,logLen;

		//Get the shader logs length
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		fragmentInfoLog = (char *)malloc(maxLength);

		//get the log info
		glGetShaderInfoLog( fragShader , maxLength , &logLen , fragmentInfoLog );

		std::string log( fragmentInfoLog , logLen );

		std::cout << "ERROR COMPILING FRAG SHADER: \n\n" << log;

		//exit indicating error
		exit(1);

		}

	glAttachShader( program , vertexShader );
	glAttachShader( program , fragShader );

	glLinkProgram( program );

	int isLinked;

	glGetProgramiv( program , GL_LINK_STATUS , (int *)&isLinked );

	if( ! (bool) isLinked) {

		char *shaderProgramInfoLog;
		int maxLength,logLen;

		/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		shaderProgramInfoLog = (char *)malloc(maxLength);

		/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
		glGetProgramInfoLog( program , maxLength, &logLen, shaderProgramInfoLog );

		std::string log( shaderProgramInfoLog , logLen );

		std::cout << "ERROR LINKING: \n\n" << log;

		exit(1);
		}

	glUseProgram( program );

	translationLocation = glGetUniformLocation( program , "translation" );

	vertexLocation = glGetAttribLocation( program , "vPosition" );

	colorLocation = glGetAttribLocation( program , "in_Color" );

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays( 1, &vao );

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray( vao );

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	glGenBuffers( 2, vbo );

	numVertices = 0;

	}

Shape::~Shape() {
	// TODO Auto-generated destructor stub
}

void Shape::draw() {

	GLfloat* tempVertices = new GLfloat[ numVertices * 4];

	for( int i = 0 ; i < numVertices ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			tempVertices[i * 4 + j] = vertices[i][j];

			}

		}

	GLfloat* tempColors = new GLfloat[ numVertices * 4];

		for( int i = 0 ; i < numVertices ; i++ ) {

			for( int j = 0 ; j < 4 ; j++ ) {

				tempColors[i * 4 + j] = colors[i][j];

				}

			}

	glUseProgram( program );

	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

	// Copy the vertex data from diamond to our buffer
	// 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values
	glBufferData( GL_ARRAY_BUFFER , numVertices * 4 * sizeof(GLfloat) , tempVertices , GL_STATIC_DRAW );

	// Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex
	glVertexAttribPointer( vertexLocation , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

	// Enable attribute index 0 as being used
	glEnableVertexAttribArray( vertexLocation );

	//Bind our second VBO as being the active buffer and storing vertex attributes (colors)
	glBindBuffer( GL_ARRAY_BUFFER , vbo[1] );

	// Copy the color data from colors to our buffer
	// 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values
	glBufferData( GL_ARRAY_BUFFER , numVertices * 4 * sizeof(GLfloat) , tempColors , GL_STATIC_DRAW );

	// Specify that our color data is going into attribute index 1, and contains three floats per vertex
	glVertexAttribPointer( colorLocation , 4 , GL_FLOAT, GL_FALSE, 0, 0);

	// Enable attribute index 1 as being used
	glEnableVertexAttribArray( colorLocation );

	glUniform2fv( translationLocation , 1 , translationVec );

	glDrawArrays( glDrawMode , 0 , numVertices );

}
void Shape::setVertices( int numVerts , TVec4<GLfloat>* verts ) {

	numVertices = numVerts;

	if(vertices)
		delete [] vertices;

	vertices = new TVec4<GLfloat>[numVertices];

	for( int i = 0 ; i < numVerts ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			vertices[i][j] = verts[i][j];

			}
		}

	calcBoundingBox();

	}

void Shape::setColors( int numColors , TVec4<GLfloat>* newColors ) {

	if( numColors != numVertices )
		throw 30;

	if(colors)
		delete [] colors;

	colors = new TVec4<GLfloat>[numVertices];

	for( int i = 0 ; i < numColors ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			colors[i][j] = newColors[i][j];

			}
		}

	}

void Shape::drawBoundingBox() {

	GLfloat* tempVertices = new GLfloat[ 16 ];

		for( int i = 0 ; i < 4 ; i++ ) {

			for( int j = 0 ; j < 4 ; j++ ) {

				tempVertices[i * 4 + j] = boundingBox[i][j];

				}

			}

		GLfloat* tempColors = new GLfloat[ 16 ];

			for( int i = 0 ; i < 4 ; i++ ) {

				for( int j = 0 ; j < 4 ; j++ ) {

					tempColors[i * 4 + j] = TVec4<GLfloat>( 1.0 , 1.0 , 1.0 , 1.0 );

					}

				}

		glUseProgram( program );

		// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
		glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

		// Copy the vertex data from diamond to our buffer
		// 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values
		glBufferData( GL_ARRAY_BUFFER , 4 * 4 * sizeof(GLfloat) , tempVertices , GL_STATIC_DRAW );

		// Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex
		glVertexAttribPointer( vertexLocation , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

		// Enable attribute index 0 as being used
		glEnableVertexAttribArray( vertexLocation );

		//Bind our second VBO as being the active buffer and storing vertex attributes (colors)
		glBindBuffer( GL_ARRAY_BUFFER , vbo[1] );

		// Copy the color data from colors to our buffer
		// 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values
		glBufferData( GL_ARRAY_BUFFER , 4 * 4 * sizeof(GLfloat) , tempColors , GL_STATIC_DRAW );

		// Specify that our color data is going into attribute index 1, and contains three floats per vertex
		glVertexAttribPointer( colorLocation , 4 , GL_FLOAT, GL_FALSE, 0, 0);

		// Enable attribute index 1 as being used
		glEnableVertexAttribArray( colorLocation );

		glUniform2fv( translationLocation , 1 , translationVec );

		glDrawArrays( GL_LINE_STRIP , 0 , 4 );

	}
void Shape::rotate( float radians ) {

	rotation = radians;

	}

void Shape::calcBoundingBox() {

	float maxX,maxY,minX,minY;

	for( int i = 0 ; i < numVertices ; i++ ) {

		if( vertices[i][0] < minX )
			minX = vertices[i][0];
		else if ( vertices[i][0] > maxX )
			maxX = vertices[i][0];

		if( vertices[i][1] < minY )
			minY = vertices[i][1];
		else if( vertices[i][1] > maxY )
			maxY = vertices[i][1];

		}

	boundingBox[0] = TVec4<GLfloat>( minX , minY , 0.0 , 1.0 );
	boundingBox[1] = TVec4<GLfloat>( maxX , minY , 0.0 , 1.0 );
	boundingBox[2] = TVec4<GLfloat>( maxX , maxY , 0.0 , 1.0 );
	boundingBox[3] = TVec4<GLfloat>( minX , maxY , 0.0 , 1.0 );

	}

bool Shape::checkSelection( int x , int y ) {

	minX = translationVec[0] * cos( rotation ) - translationVec[1] * sin( rotation );
	minY = translationVec[0] * sin( rotation ) + translationVec[1] * cos( rotation );
	maxX = (translationVec[0] + width) * cos( rotation ) - (translationVec[1] + height) * sin( rotation );
	maxX = (translationVec[0] + width) * sin( rotation ) + (translationVec[1] + height) * cos( rotation );

	clickX =  ( x * cos( rotation ) - y * sin( rotation ) ) - ( translationVec[0] * cos( rotation ) - translationVec[1] * sin( rotation ) );
	clickY =  ( x * sin( rotation ) + y * cos( rotation ) ) - ( translationVec[0] * sin( rotation ) + translationVec[1] * cos( rotation ) );

	return ( clickX < maxX && clickX > minX && clickY < maxY && clickY > minY ) ? true : false;

	}
