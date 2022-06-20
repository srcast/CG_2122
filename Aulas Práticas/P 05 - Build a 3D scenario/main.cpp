#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>


float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;

float angTeapotInterior, angTeapotExterior;
int rc = 15;
int ri = 35;
int r = 50;


void spherical2Cartesian() {

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

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	// End of frame

	//desenhar o torus
	glPushMatrix();
	glColor3f(0.456, 0.40, 0.245);
	glTranslatef(0, 4, 0);
	glutSolidTorus(2, 4, 8, 15);
	glPopMatrix();

	//desenhar teapots dentro
	angTeapotInterior = 360 / 8;
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		glRotatef(i * angTeapotInterior, 0, 1, 0);
		glTranslatef(rc, 1.5, 0); // fazer translate em y para 1.5 pois ele vai ter 3 de tamanho e senao o fizesse ficava cortado ao meio
		glutSolidTeapot(3);
		glPopMatrix();
	}


	//desenhar teapots fora
	angTeapotExterior = 360 / 16;
	glColor3f(1.0, 0.0, 0.0);
	for (int j = 0; j < 16; j++) {
		glPushMatrix();
		glRotatef(j * angTeapotExterior, 0, 1, 0);
		glTranslatef(ri, 1.5, 0); // fazer translate em y para 1.5 pois ele vai ter 3 de tamanho
		glRotatef(90, 0, 1, 0);
		glutSolidTeapot(3);
		glPopMatrix();
	}


	//desenhar árvores
	glColor3f(0.0, 0.8, 0.0);

	for (int k = 0; k < 200; k++) { //desenhar 200 arvores
		float posx = 0.0, posz = 0.0;

		while ((pow(posx, 2) + pow(posz, 2)) < pow(r, 2)) {
			posx = -100 + ((rand() % 100) *0.01 * 200);
			posz = -100 + ((rand() % 100) *0.01 * 200);
		}
		
		glPushMatrix();
		glColor3f(0.3f, 0.0f, 0.0f);
		glTranslatef(posx, 0, posz);
		glRotated(-90, 1, 0, 0); // rodar o tronco da árvore para cima ----> é desenhada deitada
		glutSolidCone(1.5, 6, 10, 10); //tronco
		glTranslatef(0, 0 , 2); //transalate em relação ao z 
		float verde = 0.0, azul = 1.0;
		while (verde < 0.2) {
			verde = rand() % 100 * 0.01;
		}
		
		while (azul > 0.2) {
			azul = rand() % 100 * 0.01;
		}
		glColor3f(0.0, verde, azul);
		glutSolidCone(3, 8, 15, 15); // folhas
		glPopMatrix();


	}
	
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

	srand(10); // nao esta a funcionar -------------------------------------------------------------------------------------

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Ficha 5");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
