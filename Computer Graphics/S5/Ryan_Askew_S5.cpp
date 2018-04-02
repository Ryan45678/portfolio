// Ryan Askew (raa97)
// CG Spring 2017
// S5

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h> // used for circle generation
#include <fstream> // for ppm_read
#include <cassert>

float zCoord = -4; // default z coordinate

const int width = 640; // window width
const int height = 480; // window height

// used to create texture objects
struct Texture
{
	std::string name; // read from ppm file
	GLuint max; // read from ppm file
	
	// values used in glTexImage2D
	GLenum target;
	GLint level;
	GLint components;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLenum format;
	GLenum type;
	GLubyte* texData;
};

// function to draw several curves
void drawCurves()
{
	glEnable(GL_MAP1_VERTEX_3); // enable 3D vertex mapping
	glPushMatrix(); // push a new matrix
	
	/* Uniform Location of Evaluation Points using glEvalCoord 
	 * This curve appears as a straight line on the floor of the room*/
	
	// point matrix for a uniform Bezier curve
	GLfloat pointsUBEZ[4][3] = { {-0.5, -0.4, -1.05}, {-0.25, -0.4, -1.05}, 
								 {0.25, -0.4, -1.05}, {0.5, -0.4, -1.05} };
	// create an evaluator
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &pointsUBEZ[0][0]);
	// evaluate the curve at each point on the map
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 50; i++)
		glEvalCoord1f((float) i / 50.0);
	glEnd();
	
	/* Uniform Location of Evaluation Points using glMapGrid 
	 * This curve appears as a straight line on the top corner of the room*/
	
	// point matrix for uniform curve (straight line) using glMapGrid
	GLfloat pointsUGRID[2][3] = { {-0.5, 0.3235, -1}, {0.5, 0.3235, -1} };
	// create an evaluator
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 2, &pointsUGRID[0][0]);
	// create a mesh
	glMapGrid1f(100, 0.0, 1.0);
	// evaluate the curve along the mesh
	glEvalMesh1(GL_LINE, 0, 99);
	
	/* interpolating Curve 
	 * This curve appears as a swirl at the top of the screen*/
	
	// push a new matrix
	glPushMatrix();
	// matrix to transform the curve from Hermite to Bezier
	GLfloat hermite[16] = {1, 0, 0, 0, 
						   -5/6.0, 3, -3/2.0, 1/3.0, 
						   1/3, -3/2.0, 3, -5/6.0,
						   0, 0, 0, 1};
	// load the new matrix into the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(hermite);
	// point matrix for an interpolating (hermite) curve
	GLfloat pointsNUINT[4][3] = { {0.1, -0.5, -2.5},
								{3 * (0.2 - 0.1), 3 * (-0.4 + 0.5), 0}, 
								{3 * (0.4 - 0.3), 3 * (-0.5 + 0.4), 0},
								{0.4, -0.5, -2.5}};
	// create an evaluator
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &pointsNUINT[0][0]);
	// render the curve
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 50; i++)
		glEvalCoord1f((float) i / 50.0);
	glEnd();
	// pop the matrix
	glPopMatrix();
	
	/* B-Spline Curve 
	 * This curve appears as an upside down 'U' shape at the bottom of the screen*/
	glPushMatrix(); // push a new matrix
	// matrix to transoform the curve from BSpline to Bezier
	GLfloat BSpline[16] = {1, 4, 1, 0, 
						   0, 4, 2, 0, 
						   0, 2, 4, 0,
						   0, 1, 4, 2};
	// load the BSpline matrix into the modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(BSpline);
	// point matrix for the BSpline curve
	GLfloat pointsNUBSpline[4][3] = { {0.5, -0.1, -4.5},
								{0.05, 0.8, -2}, 
								{0.35, 0.8, -2},
								{1.2, -0.1, -4.5}};
	// create an evaluator
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &pointsNUBSpline[0][0]);
	// render the curve
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 50; i++)
		glEvalCoord1f((float) i / 50.0);
	glEnd();
	// pop the matrix
	glPopMatrix();
	
	/* Bezier Curve 
	 * Non-Uniform Location of Evaluation Points using glEvalCoord 
	 * Tis curve is drawn between the circle and the triangle*/
	
	// point map for Bezier curve
	GLfloat pointsNUBEZ[4][3] = { {-0.3, 0.1, -1.1}, {-0.2, 0.05, -1.1}, 
								{-0.2, -0.05, -1.1}, {-0.3, -0.1, -1.1} };
	// create an evaluator
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &pointsNUBEZ[0][0]);
	// render the curve
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 50; i++)
		glEvalCoord1f((float) i / 50.0);
	glEnd();
	
	// pop the matrix
	glPopMatrix();
}

GLuint ppm_read(char* file)
{
	GLuint texture = 0; // texture data
	
	Texture texels; // use the struct to store texture info
	// fields default to these values
	texels.target = GL_TEXTURE_2D;
	texels.level = 0;
	texels.components = 3;
	texels.border = 0;
	texels.format = GL_RGB;
	texels.type = GL_UNSIGNED_BYTE;
	
	// vars used when reading in ppm pixel data
	int red, green, blue;
	float scale;
	
	// open ppm file for reading
	std::ifstream image;
	image.open(file);
	assert((image, "ERROR: specified .ppm file not found"));
	
	// read in name, width, height, max; normalize scale
	image >> texels.name >> texels.width >> texels.height >> texels.max;
	scale = 255.0 / texels.max;
	
	// create image data array
	texels.texData = new GLubyte[texels.components * texels.width * texels.height];
	
	// read in pixels from ppm file
	for (int i = 0; i < texels.width * texels.height; i++)
	{
		image >> red >> green >> blue;
		texels.texData[texels.components * texels.width * texels.height - 3 * i - 3]=red;
		texels.texData[texels.components * texels.width * texels.height- 3 * i - 2]=green;
		texels.texData[texels.components * texels.width * texels.height- 3 * i - 1]=blue;
	}
	//scale and change swap bytes property
	glPixelTransferf(GL_RED_SCALE, scale);
	glPixelTransferf(GL_GREEN_SCALE, scale);
	glPixelTransferf(GL_BLUE_SCALE, scale);
	glPixelStorei(GL_UNPACK_SWAP_BYTES,GL_TRUE);
	
	// crreate texxture object, then bind it
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// change texture object params
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// specify the texture image
	glTexImage2D(texels.target, texels.level, texels.components, texels.width, texels.height,
			     texels.border, texels.format, texels.type, texels.texData);
	// return the newly created texture object
	return texture;
}

// initialize properties
void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.0);
	glLineWidth(4.0);
	
	// allows objects to occlude one another based on distance to camera
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glMatrixMode(GL_PROJECTION); // use projection matrix
	glViewport(70, -10, 500, 500); // 500x500 viewport
	gluPerspective(45, 1.3333333, 1, 5); // set the viewing volume
	
	// move the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	
	// enable lightings, autonormalization, and each light source
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	
	// disable auto texture coordinate generation
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

//Function to draw a N-sided object that is upright (defined to have the bottom edge horizontal)
//at the center of the current model identity, with diameter = 1
void ngon(int n)
{

	float degree, vertx, verty, rotate, degToRad;
	rotate = 360.0/n;       //Vertex rotation increment
	degree = rotate/2 + 180;//Current degree of vertex (starts rotated to make object upright)
	degToRad = 180/3.14159; //Conversion factor from degrees to radians
	glBegin(GL_POLYGON);

	for(int i = 0; i < n; i++, degree += rotate)
	{
		vertx = 0.5 * sin(degree/degToRad);        //next vertex's x coordinate
		verty = 0.5 * sin((90 - degree)/degToRad); //next vertex's y coordinate
		glTexCoord2f(vertx + 0.5, verty + 0.5);
		glVertex3f(vertx, verty, 0);	
	}

	glEnd(); 

}

// fuction to draw a cube
void cube()
{
	glBegin(GL_QUADS);
	
	// front face
	glTexCoord2f(0,0);
	glVertex3f(0, 0, -1);
	glTexCoord2f(1,0);
	glVertex3f(0.25, 0, -1);
	glTexCoord2f(1,0);
	glVertex3f(0.25, 0.25, -1);
	glTexCoord2f(0,1);
	glVertex3f(0, 0.25, -1);
	
	// back face
	glTexCoord2f(0,0);
	glVertex3f(0, 0, -1.25);
	glTexCoord2f(1,0);
	glVertex3f(0.25, 0, -1.25);
	glTexCoord2f(1,1);
	glVertex3f(0.25, 0.25, -1.25);
	glTexCoord2f(0,1);
	glVertex3f(0, 0.25, -1.25);
	
	// bottom face
	glTexCoord2f(0,0);
	glVertex3f(0, 0, -1);
	glTexCoord2f(1,0);
	glVertex3f(0.25, 0, -1);
	glTexCoord2f(1,1);
	glVertex3f(0.25, 0, -1.25);
	glTexCoord2f(0,1);
	glVertex3f(0, 0, -1.25);
		
	// top face
	glTexCoord2f(0,0);
	glVertex3f(0, 0.25, -1);
	glTexCoord2f(1,0);
	glVertex3f(0.25, 0.25, -1);
	glTexCoord2f(1,1);
	glVertex3f(0.25, 0.25, -1.25);
	glTexCoord2f(0,1);
	glVertex3f(0, 0.25, -1.25);
	
	// right face
	glTexCoord2f(0,0);
	glVertex3f(0.25, 0, -1);
	glTexCoord2f(1,0);
	glVertex3f(0.25, 0, -1.25);
	glTexCoord2f(1,1);
	glVertex3f(0.25, 0.25, -1.25);
	glTexCoord2f(0,1);
	glVertex3f(0.25, 0.25, -1);
		
	// left face
	glTexCoord2f(0,0);
	glVertex3f(0, 0, -1);
	glTexCoord2f(1,0);
	glVertex3f(0, 0, -1.25);
	glTexCoord2f(1,1);
	glVertex3f(0, 0.25, -1.25);
	glTexCoord2f(0,1);
	glVertex3f(0, 0.25, -1);
		
	glEnd();
}

void sphere()
{
	float PI = atan(1.0)*4.0;
	float a, b, x, y, z;
	float da = 0.7;
	float db = 0.7;
	float radius = 1.0;
	
	for (a = -90.0; a + da <= 90.0; a += da)
	{
		glBegin(GL_QUAD_STRIP);
		
		for(b = 0.0; b <= 360; b += db)
		{
			
			x = radius * cos(b*PI/180) * cos(a*PI/180);
			y = radius*sin(b*PI/180)*cos(a*PI/180);
			z = radius*sin(a*PI/180);
			glTexCoord2f(x*2.5, y*2.5);
			glVertex3f(x,y,z);
			x = radius*cos(b*PI/180)*cos((a+da)*PI/180);
			y = radius*sin(b*PI/180)*cos((a+da)*PI/180);
			z = radius*sin((a+da)*PI/180);
			glTexCoord2f(x*2.5, y*2.5);
			glVertex3f(x,y,z);
		}
	glEnd();
	}
}

GLuint CGI()
{
	GLuint *grid; // image data pointer
	GLuint texture; // texture object pointer
	
	
	// create and bind texture object
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
			
	// set texture params
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			
	// populate the texture data (was intended to be b/w grid, but the actual result looks
	// better, so I kept it)
	grid = new GLuint[2*2*3];
	for (int i = 0; i < 12; i += 3)
	{
		if (i % 2 == 0)
		{
			grid[i] = 0;
			grid[i + 1] = 0;
			grid [i + 2] = 0;
		}
		if (i % 4 == 1)
		{
			grid[i] = 256;
			grid[i + 1] = 256;
			grid[i + 2] = 256;
		}
	}
			
	// specify the texture image
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 4, 4,
			     0, GL_RGB, GL_UNSIGNED_INT, grid);
	
	return texture;
}

void lightScene()
{
	
	GLfloat ZERO[] = {0, 0, 0, 0};
	/**********************************
	* Light 0
	**********************************/
	glPushMatrix();
	
	glShadeModel(GL_FLAT);
	
	GLfloat position0[] = {-0.7, -0.5, -3.0, 1.0};
	GLfloat PointFiniteD[] = {0.6, 0.7, 0.9, 0.0};
	GLfloat PointFiniteS[] = {0.1, 0.2, 0.15, 1.0};
	
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, PointFiniteD);
	glLightfv(GL_LIGHT0, GL_SPECULAR, PointFiniteS);
	
		const GLfloat PAttQ = .1;
		const GLfloat PAttL = 0.3;
		const GLfloat PAttC = 0.2;
		
		glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &PAttQ);
		glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &PAttL);
		glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, &PAttC);
	
	glPopMatrix();
	/**********************************
	* Light 1
	**********************************/
	
	glPushMatrix();
	
	glShadeModel(GL_SMOOTH);
	
	GLfloat position1[] = {0.5, -0.5, 2.0, 0.0};
	GLfloat PointInfiniteD[] = {0.25, 0.27, 0.26, 0.0};
	GLfloat PointInfiniteS[] = {0.1, 0.12, 0.08, 0.0};
	
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, PointInfiniteD);
	glLightfv(GL_LIGHT1, GL_SPECULAR, PointInfiniteS);
	
	glPopMatrix();
	/**********************************
	* Light 2
	**********************************/
	
	glPushMatrix();
	GLfloat em2[] = {.1, .07, .07, 1};
	GLfloat Light2Color[] = {1, 1, .8, 1};
	glMaterialfv(GL_FRONT, GL_EMISSION, em2);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Light2Color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Light2Color);
	
	glTranslatef(.35, 0.05, -2.5);
	glScalef(0.03, 0.03, 0.03);
	glTranslatef(-0.01, -0.01, -0.007);
	sphere();
	glPopMatrix();
	
	glPushMatrix();
		GLfloat position2[] = {0.35, 0.0, -0.9, 1.0};
		GLfloat SpotD[] = {0.7, 0.2, 0.8, 1.0};
		GLfloat SpotS[] = {0.7, 0.75, 0.39, 1.0};
		GLfloat SpotDir[] = {-0.25, 0.75, -0.3};
		
		glLightfv(GL_LIGHT2, GL_POSITION, position2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, SpotD);
		glLightfv(GL_LIGHT2, GL_SPECULAR, SpotS);
		
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 70.0);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 2.0);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, SpotDir);
		
		glPopMatrix();
	
	/**********************************
	* Light 3
	**********************************/
		
		glPushMatrix();
			
		glShadeModel(GL_FLAT);
			
		GLfloat position3[] = {-0.5, -0.5, 2.0, 1.0};
		GLfloat PointAmbientA[] = {0.07, 0.08, 0.06, 0.0};
		
		glLightfv(GL_LIGHT3, GL_POSITION, position3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, PointAmbientA);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, ZERO);
		glLightfv(GL_LIGHT3, GL_SPECULAR, ZERO);
			
		glPopMatrix();
		
}

void myDisplay()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*************************
	draw room "walls"
	*************************/
	// wall texture 
	glEnable(GL_TEXTURE_2D); // enable texture mapping
	// create / bind texture
	GLuint texWall;
	texWall = ppm_read("tile.ppm");
	glBindTexture(GL_TEXTURE_2D, texWall);
	
	// draw floor
	glPushMatrix();
	
	float wallBottomColor[] = {0.3, 0.4, 0.1, 1.0};
	float wallBottomSpecular[] = {0.9, 0.56, 0.47, 0};
	float wallBottomShiny = 36;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wallBottomColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wallBottomSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, wallBottomShiny);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-2, -2, -1);
	glTexCoord2f(1, 0);
	glVertex3f(2, -2, -1);
	glTexCoord2f(1, 1);
	glVertex3f(2, -1.35, -5);
	glTexCoord2f(0, 1);
	glVertex3f(-2, -1.35, -5);
	glEnd();
	
	glPopMatrix();
	
	// draw back wall
	glPushMatrix();
	
	float wallBackColor[] = {0.1, 0.7, 0.1, 1.0};
	float wallBackSpecular[] = {0.6, 0.66, 0.37, 0.3};
	float wallBackShiny = 72;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wallBackColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wallBackSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, wallBackShiny);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-2, 2, -5);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2, -5);
	glTexCoord2f(1, 1);
	glVertex3f(2, -1.35, -5);
	glTexCoord2f(0, 1);
	glVertex3f(-2, -1.35, -5);
	glEnd();
	
	glPopMatrix();
	
	// draw top
	glPushMatrix();
	
	float wallTopColor[] = {0.6, 0.2, 0.9, 1.0};
	float wallTopSpecular[] = {0.6, 0.76, 0.37, 0};
	float wallTopShiny = 16;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wallTopColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wallTopSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, wallTopShiny);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-2, 2.25, -1);
	glTexCoord2f(1, 0);
	glVertex3f(2, 2.25, -1);
	glTexCoord2f(1, 1);
	glVertex3f(2, 1.6, -5);
	glTexCoord2f(0, 1);
	glVertex3f(-2, 1.6, -5);
	glEnd();
	
	glPopMatrix();
	
	// draw left
	glPushMatrix();
	
	float wallLeftColor[] = {0.37, 0.32, 0.38, 1.0};
	float wallLeftSpecular[] = {0.3, 0.46, 0.17, 0};
	float wallLeftShiny = 112;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wallLeftColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wallLeftSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, wallLeftShiny);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-2, 2, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-2, -2, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-2, -1.35, -5);
	glTexCoord2f(0, 1);
	glVertex3f(-2, 2, -5);
	glEnd();
	
	glPopMatrix();
	
	// draw right
	glPushMatrix();
	
	float wallRightColor[] = {0.67, 0.82, 0.18, 1.0};
	float wallRightSpecular[] = {0.8, 0.56, 0.77, 0};
	float wallRightShiny = 87;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wallRightColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, wallRightSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, wallRightShiny);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(2, 2, -1);
	glTexCoord2f(1,0);
	glVertex3f(2, -2, -1);
	glTexCoord2f(1,1);
	glVertex3f(2, -1.35, -5);
	glTexCoord2f(0,1);
	glVertex3f(2, 2, -5);
	glEnd();
	
	glPopMatrix();
	
	/*************************
	draw point
	*************************/
	glPushMatrix(); // push the current matrix to avoid unwnated changes
	
	// set color
	float PointColor[] = {0.2, 0.9, 0.3, 1.0};
	float PointSpecular[] = {0.3, 0.2, 0.5, 0};
	float PointShiny = 20;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, PointColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, PointSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, PointShiny);
	
	// scale, then translate
	glTranslatef(-0.75, 0.75, zCoord);
	glScalef(3, 3, 3);
	
	// add point
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();

	// pop the matrix to allow for a new one for the next shape
	glPopMatrix();
	
	
	/*************************
	draw line
	 *************************/
	glPushMatrix();
	
	// set color
	float LineColor[] = {0.7, 0.5, 0.4, 1.0};
	float LineSpecular[] = {0.1, 0.9, 0.3, 0};
	float LineShiny = 55;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LineColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LineSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, LineShiny);
	
	// rotate, scale, then translate
	glTranslatef(-0.9, -1.1, zCoord);
	glScalef(2, 2, 2);
	glRotatef(20, 0, 0, -1);
	
	// add line
	glBegin(GL_LINES);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);
	glEnd();
	
	glPopMatrix();

	
	/*************************
	draw triangle
	*************************/
	glPushMatrix();
	
	// set color
	float TriangleColor[] = {0.1, 0.9, 0.4, 1.0};
	float TriangleSpecular[] = {0.2, 0.5, 0.2, 0};
	float TriangleShiny = 35;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, TriangleColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, TriangleSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, TriangleShiny);
	
	// rotate, scale, then translate
	glTranslatef(-0.2, -0.3, zCoord);
	glScalef(1.5, 1.5, 1.5);
	glRotatef(25, 0, 0, -1);
	
	// create / bind texture
	GLuint texT = ppm_read("tree.ppm");
	glBindTexture(GL_TEXTURE_2D, texT);
	
	ngon(3);
	
	glPopMatrix();

	
	/*************************
	draw square
	*************************/
	glPushMatrix();
	
	// set color
	float SquareColor[] = {0.7, 0.3, 0.4, 1.0};
	float SquareSpecular[] = {0.5, 0.5, 0.1, 0};
	float SquareShiny = 78;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, SquareColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SquareSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, SquareShiny);
	
	// rotate, scale, then translate
	glTranslatef(-1.5, 0.7, zCoord);
	glScalef(0.75, 0.75, 0.75);
	glRotatef(46, 0, 0, -1); 
	
	// create / bind texture
	GLuint texSqu = ppm_read("building.ppm");
	glBindTexture(GL_TEXTURE_2D, texSqu);

	ngon(4);	

	glPopMatrix();

	
	/*************************
	draw hexagon
	*************************/
	glPushMatrix();
	
	// set color
	float HexColor[] = {0.1, 0.2, 0.9, 1.0};
	float HexSpecular[] = {0.1, 0.5, 0.7, 0};
	float HexShiny = 25;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, HexColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, HexSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, HexShiny);
	
	// rotate, scale, then translate
	glTranslatef(1.2, -0.8, zCoord);
	glScalef(1.7, 1.2, 0.85);
	glRotatef(80, 0, 0, 1);
	
	// create / bind texture
	GLuint texH = ppm_read("girl.ppm");
	glBindTexture(GL_TEXTURE_2D, texH);
	
	ngon(6);
	
	glPopMatrix();

	
	/*************************
	draw circle
	*************************/
	glPushMatrix();
	// set color
	float CircleColor[] = {0.7, 0.2, 0.8, 1.0};
	float CircleSpecular[] = {0.7, 0.1, 0.3, 0};
	float CircleShiny = 100;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, CircleColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, CircleSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, CircleShiny);
	
	// rotate, scale, then translate
	glTranslatef(-1.5, -0.1, zCoord);
	glScalef(0.75, 0.5, 1.3);
	glRotatef(155, 1, 0, 0);
	
	// create / bind texture
	GLuint texCir = ppm_read("statue.ppm");
	glBindTexture(GL_TEXTURE_2D, texCir);
	
	ngon(100);
	
	glPopMatrix();
	
	/*************************
	draw sphere
	*************************/
		
	glPushMatrix();
	
	// set color
	float SphereColor[] = {0.4, 0.6, 0.8, 1.0};
	float SphereSpecular[] = {0.7, 0.2, 0.4, 0};
	float SphereShiny = 128;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, SphereColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SphereSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, SphereShiny);
	
	glTranslatef(0.3, 0.1, 0.0035);
	glRotatef(75, 1, 1, 1);
	glTranslatef(-0.01, -0.01, -0.007);
	
	// create / bind texture
	GLuint texS = ppm_read("s128.ppm");
	glBindTexture(GL_TEXTURE_2D, texS);
	
	sphere();
	
	// disable texture mapping for shadows
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	glPushMatrix();
	// create sphere shadow
	float ShadowColor[] = {0.15, 0.15, 0.15, 1.0};
	float ShadowSpecular[] = {0.0, 0.0, 0.0, 1.0};
	float ShadowShiny = 0;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ShadowColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ShadowSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, ShadowShiny);
	
	glTranslatef(0.85, 0.04, -3);
	glRotatef(75, -1, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	
	ngon(100);
	
	glPopMatrix();
	
	/*************************
	draw cubes
	*************************/
	glEnable(GL_TEXTURE_2D); // enable texture for cubes
	
	// create / bind cube 1 texture
	GLuint texC1 = ppm_read("bw.ppm");
	glBindTexture(GL_TEXTURE_2D, texC1);
	glPushMatrix();
	
	// set color
	float C1Color[] = {0.6, 0.5, 0.3, 1.0};
	float C1Specular[] = {0.6, 0.3, 0.3, 0};
	float C1Shiny = 43;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, C1Color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, C1Specular);
	glMateriali(GL_FRONT, GL_SHININESS, C1Shiny);
	
	// translate first cube to origin, then translate to another point
	glTranslatef(1.3, 1.0, zCoord);
	glTranslatef(-0.125, -0.125, 1.125);

	cube();
	
	float m[16]; // temporary array
	glGetFloatv(GL_MODELVIEW_MATRIX, m); // store the current modelview matrix
	glPopMatrix();
	glPushMatrix(); // push the current matrix to allow for a new one
	glLoadMatrixf(m); // set the new matrix equal to the copy of the old one,
	// effectively copying the cube
	
	// create / bind cube 2 texture
	GLuint texC2 = CGI();
	glBindTexture(GL_TEXTURE_2D, texC2);
	
	// set color
	float C2Color[] = {0.9, 0.1, 0.3, 1.0};
	float C2Specular[] = {0.7, 0.5, 0.1, 0};
	float C2Shiny = 120;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, C2Color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, C2Specular);
	glMateriali(GL_FRONT, GL_SHININESS, C2Shiny);
	
	// translate second cube back to origin, rotate, then translate to another point
	glTranslatef(0.65, 1.1, 0);
	glRotatef(45, 1, 1, 1);
	glTranslatef(-1.6, -1.2, 0);
	cube();
	glPopMatrix();
	
	glPushMatrix();
	
	glDisable(GL_TEXTURE_2D); // disable texture mapping for shadows
	
	// draw shadows for both cubes
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ShadowColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ShadowSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, ShadowShiny);
	
	glTranslatef(1.61, 1.49, -4);
	
	glBegin(GL_QUADS);
			
	glVertex3f(-.1, -.03, -1);
	glVertex3f(0, -0.35, -1);
	glVertex3f(0.25, -0.35,-1);
	glVertex3f(0.35, 0, -1);			
	glEnd();
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2., 1.63, -4);
	
	glBegin(GL_QUADS);
					
		glVertex3f(-1.45, -.35, -1);
		glVertex3f(-1.65, -0.9, -1);
		glVertex3f(-1.2, -0.8,-1);
		glVertex3f(-1.1, -.45, -1);
			
	glEnd();
	
	glPopMatrix();
	
	drawCurves();

	lightScene();

	// flush the scene to the screen.
	glFlush();
	
	glDeleteTextures(1, &texT);
	glDeleteTextures(1, &texSqu);
	glDeleteTextures(1, &texH);
	glDeleteTextures(1, &texCir);
	glDeleteTextures(1, &texS);
	glDeleteTextures(1, &texC1);
	glDeleteTextures(1, &texC2);
	glDeleteTextures(1, &texWall);
}	

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Scene 5");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
	
}