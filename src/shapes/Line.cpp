/*
 * GL_Shape.cpp
 *
 *  Created on: Sep 21, 2011
 *      Author: njordan
 */

#include "Line.h"

Line::Line( ) {
	
	translationMatrix = translationMatrix.I();

	rotationMatrix = rotationMatrix.I();

	scaleMatrix = scaleMatrix.I();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

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

	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays( 1, &vao );

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray( vao );

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	glGenBuffers( 2, vbo );

	numVertices = 0;

	}

Line::~Line() {
	// TODO Auto-generated destructor stub
}


void Line::draw() {


	GLfloat tempVertices[ numVertices ][ Line::dimension ];

	for( int i = 0 ; i < numVertices ; i++ ) {

		for( int j = 0 ; j < Line::dimension ; j++ ) {

			tempVertices[i][j] = vertices[i][j];

			}

		}

	GLfloat tempColors[ numVertices ][ 4 ];

		for( int i = 0 ; i < numVertices ; i++ ) {

			for( int j = 0 ; j < 4 ; j++ ) {

				tempColors[i][j] = colors[i][j];

				}

			}

	GLfloat* rotation= new GLfloat[16];
	GLfloat* scale= new GLfloat[16];

	buildMatrixUniform(rotation,rotationMatrix);
	buildMatrixUniform(scale,rotationMatrix);

	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

	// Copy the vertex data from diamond to our buffer
	// 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values
	glBufferData(GL_ARRAY_BUFFER, numVertices * Line::dimension * sizeof(GLfloat), tempVertices , GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable attribute index 0 as being used
	glEnableVertexAttribArray(0);

	//Bind our second VBO as being the active buffer and storing vertex attributes (colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// Copy the color data from colors to our buffer
	// 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values
	glBufferData(GL_ARRAY_BUFFER, numVertices * 4 * sizeof(GLfloat), tempColors , GL_STATIC_DRAW);

	// Specify that our color data is going into attribute index 1, and contains three floats per vertex
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable attribute index 1 as being used
	glEnableVertexAttribArray(1);

	glUniform4fv(0,1, translationVec );

	glUniformMatrix4fv(1,1,GL_FALSE, rotation );

	glUniformMatrix4fv(2,1,GL_FALSE, scale );

	glDrawArrays( GL_LINE_STRIP , 0, numVertices );

}
void Line::setVertices( int numVerts , TVec4<GLfloat> verts[2] ) {

	numVertices = numVerts;

	for( int i = 0 ; i < numVerts ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			vertices[i][j] = verts[i][j];

			}
		}

	}

void Line::setColors( int numColors , TVec4<GLfloat> newColors[2] ) {

	if( numColors != numVertices )
		throw 30;

	for( int i = 0 ; i < numColors ; i++ ) {

		for( int j = 0 ; j < 4 ; j++ ) {

			colors[i][j] = newColors[i][j];

			}
		}

	}


void Line::rotate( float radians ) {

	}

void Line::buildMatrixUniform( GLfloat* arr , Mat4& matrix ) {
	double* mat = matrix;
	for( int i = 0 ; i < 4 ; i++ )
		for( int j = 0 ; j < 4 ; j++ )
			arr[ i * 4 + j ] = mat[ i * 4 + j ];
	}