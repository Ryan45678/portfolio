// Ryan Askew (raa97)
// CG Spring 2017
// S3b

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h> // used for circle generation

float zCoord = -4; // default z coordinate

const int width = 640; // window width
const int height = 480; // window height

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
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	
	
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
		glVertex3f(vertx, verty, 0);
	}

	glEnd(); 

}

// fuction to draw a cube
void cube()
{
	glBegin(GL_QUADS);
	
	// front face
	glVertex3f(0, 0, -1);
	glVertex3f(0.25, 0, -1);
	glVertex3f(0.25, 0.25, -1);
	glVertex3f(0, 0.25, -1);
	
	// back face
	glVertex3f(0, 0, -1.25);
	glVertex3f(0.25, 0, -1.25);
	glVertex3f(0.25, 0.25, -1.25);
	glVertex3f(0, 0.25, -1.25);
	
	// bottom face
	glVertex3f(0, 0, -1);
	glVertex3f(0.25, 0, -1);
	glVertex3f(0.25, 0, -1.25);
	glVertex3f(0, 0, -1.25);
		
	// top face
	glVertex3f(0, 0.25, -1);
	glVertex3f(0.25, 0.25, -1);
	glVertex3f(0.25, 0.25, -1.25);
	glVertex3f(0, 0.25, -1.25);
	
	// right face
	glVertex3f(0.25, 0, -1);
	glVertex3f(0.25, 0, -1.25);
	glVertex3f(0.25, 0.25, -1.25);
	glVertex3f(0.25, 0.25, -1);
		
	// left face
	glVertex3f(0, 0, -1);
	glVertex3f(0, 0, -1.25);
	glVertex3f(0, 0.25, -1.25);
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
			glVertex3f(x,y,z);
			x = radius*cos(b*PI/180)*cos((a+da)*PI/180);
			y = radius*sin(b*PI/180)*cos((a+da)*PI/180);
			z = radius*sin((a+da)*PI/180);
			glVertex3f(x,y,z);
		}
	glEnd();
	}
}

void lightScene()
{
	
	GLfloat ZERO[] = {0, 0, 0, 0};
	/**********************************
	* Light 0
	**********************************/
	glPushMatrix();
	
	glShadeModel(GL_FLAT);
	
	GLfloat position0[] = {0.7, 0.5, -3.0, 1.0};
	GLfloat PointFiniteD[] = {0.6, 0.7, 0.9, 0.0};
	GLfloat PointFiniteS[] = {0.1, 0.2, 0.15, 1.0};
	
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, PointFiniteD);
	glLightfv(GL_LIGHT0, GL_SPECULAR, PointFiniteS);
	
	// set light attenuation
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.3);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
	
	glPopMatrix();
	/**********************************
	* Light 1
	**********************************/
	
	glPushMatrix();
	
	glShadeModel(GL_SMOOTH);
	
	GLfloat position1[] = {-0.1, 1, 3.0, 0.0};
	GLfloat PointInfiniteD[] = {0.25, 0.27, 0.26, 0.0};
	GLfloat PointInfiniteS[] = {0.1, 0.12, 0.08, 0.0};
	
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, PointInfiniteD);
	glLightfv(GL_LIGHT1, GL_SPECULAR, PointInfiniteS);
	
	// set light attenuation
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.9);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.9);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.9);
	
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
	
	glTranslatef(.5, 0.05, -2.5);
	glScalef(0.03, 0.03, 0.03);
	glTranslatef(-0.01, -0.01, -0.007);
	sphere();
	glPopMatrix();
	
	glPushMatrix();
		GLfloat position2[] = {0.5, 0.0, -0.9, 1.0};
		GLfloat SpotD[] = {0.7, 0.2, 0.8, 1.0};
		GLfloat SpotS[] = {0.7, 0.75, 0.39, 1.0};
		GLfloat SpotDir[] = {-0.25, 0.75, -0.3};
		
		glLightfv(GL_LIGHT2, GL_POSITION, position2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, SpotD);
		glLightfv(GL_LIGHT2, GL_SPECULAR, SpotS);
		
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 70.0);
		
		// set light attenuation
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 2.0);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.9);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.9);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, SpotDir);
		
		glPopMatrix();
	
	/**********************************
	* Light 3
	**********************************/
		
		glPushMatrix();
			
		glShadeModel(GL_FLAT);
			
		GLfloat position3[] = {0.5, 0.5, 0, 1.0};
		GLfloat PointAmbientA[] = {0.07, 0.08, 0.06, 0.0};
		
		glLightfv(GL_LIGHT3, GL_POSITION, position3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, PointAmbientA);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, ZERO);
		glLightfv(GL_LIGHT3, GL_SPECULAR, ZERO);
			
		// set light attenuation
		glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 1.1);
		glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 1.1);
		glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.1);
		
		glPopMatrix();
		
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
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
	glTranslatef(-0.9, -1.2, zCoord);
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
	glTranslatef(1.2, -0.9, zCoord);
	glScalef(1.7, 1.2, 0.85);
	glRotatef(80, 0, 0, 1);
	
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
	
	sphere();
	
	glPopMatrix();
	
	glPushMatrix();
	
	// set shadow properties
	float SShadowColor[] = {0.15, 0.15, 0.15, 1.0};
	float SShadowSpecular[] = {0.1, 0.1, 0.1, 0};
	float SShadowShiny = 0;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, SShadowColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SShadowSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, SShadowShiny);
	
	// draw the shadow
	glTranslatef(.95, .15, -3);
	glScalef(0.6, 0.4, 0.3);
	ngon(100);
	
	glPopMatrix();
	
	/*************************
	draw cubes
	*************************/
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
	glTranslatef(1.6, 1.2, zCoord);
	glTranslatef(-0.125, -0.125, 1.125);
	
	cube();
	
	float m[16]; // temporary array
	glGetFloatv(GL_MODELVIEW_MATRIX, m); // store the current modelview matrix
	glPopMatrix();
	glPushMatrix(); // push the current matrix to allow for a new one
	glLoadMatrixf(m); // set the new matrix equal to the copy of the old one,
	// effectively copying the cube
	
	// set color
	float C2Color[] = {0.9, 0.1, 0.3, 1.0};
	float C2Specular[] = {0.7, 0.5, 0.1, 0};
	float C2Shiny = 120;
	
	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, C2Color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, C2Specular);
	glMateriali(GL_FRONT, GL_SHININESS, C2Shiny);
	
	// translate second cube back to origin, rotate, then translate to another point
	glTranslatef(0.35, 1, 0);
	glRotatef(45, 1, 1, 1);
	glTranslatef(-1.6, -1.2, 0);
	cube();
	glPopMatrix();
	
	glPushMatrix();
	
	float ShadowColor[] = {0.15, 0.15, 0.15, 1.0};
	float ShadowSpecular[] = {0.0, 0.0, 0.0, 1.0};
	float ShadowShiny = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ShadowColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ShadowSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, ShadowShiny);
	
	glTranslatef(2, 1.75, -4);
	
	glBegin(GL_QUADS);
			
		glVertex3f(-.1, -.03, -1);
		glVertex3f(0, -0.35, -1);
		glVertex3f(0.25, -0.35,-1);
		glVertex3f(0.35, 0, -1);
		
		glVertex3f(-1.45, -.35, -1);
		glVertex3f(-1.65, -0.9, -1);
		glVertex3f(-1.2, -0.8,-1);
		glVertex3f(-1.1, -.45, -1);
			
	glEnd();
	
	glPopMatrix();

	lightScene();
	
	// flush the scene to the screen.
	glFlush();
}	

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Scene 3b");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
	
}