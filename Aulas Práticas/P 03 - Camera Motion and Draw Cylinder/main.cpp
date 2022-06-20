#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>


struct Polar {
	float raio;
	float angulo;
};

GLenum modo;

Polar camara = { sqrt(50), M_PI_4 };

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


float xPolar(Polar p) {

	return p.raio * sin(p.angulo);
}

float zPolar(Polar p) {

	return p.raio * cos(p.angulo);
}

void drawCylinder(float radius, float height, int slices) {

// put code to draw cylinder in here

	float angulo = (M_PI * 2) / slices;
	float x, x2, z, z2;
	int i;

	//TOP 
	float alturaTop = height / 2;
	float alturaBase = -(height / 2);
	glPolygonMode(GL_FRONT_AND_BACK, modo);

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0, 0.0, 0.5);
		glVertex3f(0.0f, alturaTop, 0.0f);
	for (i = 0; i <= slices; i++) {

		Polar p1 = { radius, angulo * i };

		x = xPolar(p1);
		z = zPolar(p1);
		//x2 = xPolar(radius, angulo * (i+1));
		//z2 = zPolar(radius, angulo * (i+1));

		//glVertex3f(0.0f, alturaTop, 0.0f);
		glVertex3f(x, alturaTop, z);
		//glVertex3f(x2, alturaTop, z2);
		}

	glEnd();


	//BASE
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.411, 0.886, 0.807);
		glVertex3f(0.0f, alturaBase, 0.0f);
		for (i = 0; i <= slices; i++) {

			Polar p1 = { radius, angulo * i };

			x = xPolar(p1);
			z = zPolar(p1);
			//x2 = xPolar(radius, angulo * (i+1));
			//z2 = zPolar(radius, angulo * (i+1));

			//glVertex3f(0.0f, alturaTop, 0.0f);
			glVertex3f(x, alturaBase, z);
			//glVertex3f(x2, alturaTop, z2);
		}

	glEnd();


	//SIDE

	float cor = 0.86;
	float dec = 0.9;
	glBegin(GL_TRIANGLES);
		
		for (i = 0; i <= slices; i++) {
			cor = cor * dec;
			glColor3f(0.0, cor, 0.0);

			Polar p1 = { radius, angulo * i };
			Polar p2 = { radius, angulo * (i + 1) };

			x = xPolar(p1);
			z = zPolar(p1);
			x2 = xPolar(p2);
			z2 = zPolar(p2);

			glVertex3f(x, alturaTop, z);
			glVertex3f(x, alturaBase, z);
			glVertex3f(x2, alturaTop, z2);

			glVertex3f(x2, alturaTop, z2);
			glVertex3f(x, alturaBase, z);
			glVertex3f(x2, alturaBase, z2);


		}
	glEnd();

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(xPolar(camara),5.0,zPolar(camara), 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	
	drawCylinder(1.0,2.0,10);

	// End of frame
	glutSwapBuffers();
}


// write function to process keyboard events
void reactKey(unsigned char key, int x, int y) {

	switch (key) {
	case 'a':
		camara.angulo -= 0.1;
		break;
	case 'd':
		camara.angulo += 0.1;
		break;
	}

	if (camara.angulo < 0) camara.angulo += M_PI * 2;

	else if (camara.angulo > M_PI * 2) camara.angulo -= M_PI * 2;

	glutPostRedisplay();
}

void escolherModo(int key_code, int x, int y) {
	switch (key_code) {
	case 0x0065: // seta para cima
		modo = GL_FILL;
		break;
	case 0x0064: // seta esquerda
		modo = GL_LINE;
		break;
	case 0x0066: // seta direita
		modo = GL_POINT;
		break;
	}
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Aula 3");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(reactKey);
	glutSpecialFunc(escolherModo);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
