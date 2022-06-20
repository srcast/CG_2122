#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include <vector>

using namespace std;

#define _PI_ 3.14159

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint vertexCount, vertices;
GLuint buffers[2];

vector<float> vertexB;
vector<float> normalB;

int timebase = 0, frame = 0;

float polarX(float alpha, float raio) {
	return raio * sin(alpha);
}

float polarZ(float alpha, float raio) {
	return raio * cos(alpha);
}

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void prepareCilinder(float height, float radius, int sides) {

	vertexB.clear();
	normalB.clear();

	float *v;

	int vertex = 0;
	float delta = 2.0f * _PI_ / sides;  //é como se fosse o alpha
	int i;

	for (i = 0; i < sides; ++i) {
		// top
		// central point
		vertexB.push_back(0.0f);
		vertexB.push_back(height / 2.0f);
		vertexB.push_back(0.0f);

			normalB.push_back(0);
			normalB.push_back(1); // só y é que tem componete por ser normais verticais
			normalB.push_back(0);

		vertexB.push_back(radius * sin(i * delta));
		vertexB.push_back(height / 2.0f);
		vertexB.push_back(radius * cos(i * delta));

			normalB.push_back(0);
			normalB.push_back(1);
			normalB.push_back(0);

		vertexB.push_back(radius * sin((i + 1) * delta));
		vertexB.push_back(height / 2.0f);
		vertexB.push_back(radius * cos((i + 1) * delta));

			normalB.push_back(0);
			normalB.push_back(1);
			normalB.push_back(0);

	}


	for (i = 0; i < sides; ++i) {
		// body
		// triângulo 1
		vertexB.push_back(radius * sin((i + 1) * delta));
		vertexB.push_back(height / 2.0f);
		vertexB.push_back(radius * cos((i + 1) * delta));

			normalB.push_back(sin((i + 1) * delta));
			normalB.push_back(0);
			normalB.push_back(cos((i + 1) * delta));

		vertexB.push_back(radius * sin(i * delta));
		vertexB.push_back(height / 2.0f);
		vertexB.push_back(radius * cos(i * delta));

			normalB.push_back(sin(i * delta));
			normalB.push_back(0);
			normalB.push_back(cos(i * delta));

		vertexB.push_back(radius * sin(i * delta));
		vertexB.push_back (-height / 2.0f);
		vertexB.push_back(radius * cos(i * delta));

			normalB.push_back(sin(i * delta));
			normalB.push_back(0);
			normalB.push_back(cos(i * delta));

		// triangle 2
		vertexB.push_back(radius * sin((i + 1) * delta));
		vertexB.push_back(-height / 2.0f);
		vertexB.push_back(radius * cos((i + 1) * delta));

			normalB.push_back(sin((i + 1) * delta));
			normalB.push_back(0);
			normalB.push_back(cos((i + 1) * delta));

		vertexB.push_back(radius * sin((i + 1) * delta));
		vertexB.push_back(height / 2.0f);
		vertexB.push_back(radius * cos((i + 1) * delta));

			normalB.push_back(sin((i + 1) * delta));
			normalB.push_back(0);
			normalB.push_back(cos((i + 1) * delta));

		vertexB.push_back(radius * sin(i * delta));
		vertexB.push_back (-height / 2.0f);
		vertexB.push_back(radius * cos(i * delta));

			normalB.push_back(sin(i * delta));
			normalB.push_back(0);
			normalB.push_back(cos(i * delta));

	}

	for (i = 0; i < sides; ++i) {
		// base
		// central point
		vertexB.push_back(0.0f);
		vertexB.push_back (-height /2.0f);
		vertexB.push_back(0.0f);

			normalB.push_back(0);
			normalB.push_back(-1);  // neste caso o valor de y é negativo por a normal vertical apontar para baixo
			normalB.push_back(0);

		vertexB.push_back(radius * sin( (i+1) * delta));
		vertexB.push_back (-height /2.0f);
		vertexB.push_back(radius * cos( (i+1) * delta));

			normalB.push_back(0);
			normalB.push_back(-1);
			normalB.push_back(0);

		vertexB.push_back(radius * sin( i * delta));
		vertexB.push_back (-height /2.0f);
		vertexB.push_back(radius * cos( i * delta));

			normalB.push_back(0);
			normalB.push_back(-1);
			normalB.push_back(0);

	}

	vertexCount = vertexB.size() / 3;

	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexB.size(), vertexB.data(),     GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalB.size(), normalB.data(), GL_STATIC_DRAW);

}


void drawCilinder() {
		
	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void renderScene(void) {

	float pos[4] = {1.0, 1.0, 1.0, 0.0};
	float fps;
	int time;
	char s[64];

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// glLightfv(GL_LIGHT0, GL_POSITION, pos); //se estiver aqui a luz segue a câmara

	gluLookAt(camX,camY,camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	
	////////////////////////////////////////////////////
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	float dark[] = { 0.2, 0.2, 0.2, 1.0 };
	float white[] = { 0.8, 0.8, 0.8, 1.0 };
	float red[] = { 0.8, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);

	drawCilinder();

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	} 

// End of frame
	glutSwapBuffers();
}



void processKeys(int key, int xx, int yy) 
{
	switch(key) {
	
		case GLUT_KEY_RIGHT: 
						alfa -=0.1; break;

		case GLUT_KEY_LEFT: 
						alfa += 0.1; break;

		case GLUT_KEY_UP : 
						beta += 0.1f;
						if (beta > 1.5f)
							beta = 1.5f;
						break;

		case GLUT_KEY_DOWN: 
						beta -= 0.1f; 
						if (beta < -1.5f)
							beta = -1.5f;
						break;

		case GLUT_KEY_PAGE_DOWN : radius -= 0.1f; 
			if (radius < 0.1f)
				radius = 0.1f;
			break;

		case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

	}
	sphericalToCartesian();

}



void initGL() {

// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING); /////////////////////////////////////////////////////7
	glEnable(GL_LIGHT0); ///////////////////////////////////////////////////////

	////////////////////////////////////////7 
	GLfloat dark[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat white[4] = { 0.1, 1.0, 1.0, 1.0 };

	// light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);



	////////////////////////////////////

// init
	sphericalToCartesian();
	glEnableClientState(GL_VERTEX_ARRAY);

	prepareCilinder(2,1,16);
}


int main(int argc, char **argv) {

// init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Fich 9");
		
// callback registration
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// keyboard callback registration
	glutSpecialFunc(processKeys);

#ifndef __APPLE__	
// init GLEW
	glewInit();
#endif	

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	initGL();

	glutMainLoop();
	
	return 1;
}

