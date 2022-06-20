#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float tx = 0.1;
float ty = 0.1;
float tz = 0.1;
float angle = 2.0;
// GL_FILL, GL_LINE, GL_POINT
GLenum modo;

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
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, modo);

	//como não queremos o referencial a mexer temos de colocar o seu desenho antes das transformações
	//desenhar o referencial
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

// put the geometric transformations here
	glTranslatef(tx, 0.0, tz); //apenas faz translações nos eixos do x e z
	angle += 0.05; // faz a pirâmide rodar sozinha
	glRotatef(angle, 0.0, 1.0, 0.0); //apenas roda em torno do y. O resto é 0 por ser um vetor
	glScalef(1.0, 1.5, 1.0); //apenas queremos escalar em altura mas os outros tem que ser 1 devido a multiplicar tudo


// put drawing instructions here

	glBegin(GL_TRIANGLES);

		// TER ATENÇAO À REGRA DA MAO DIREITA -> POLEGAR APONTA PARA A FACE ESTÁ VOLTADA
		//desenhar a base da piramide em dois triangulos
		glColor3f(0.411, 0.886, 0.807);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);


		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		

		// lado esquerdo
		glColor3f(0.686, 0.839, 0.462);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);

		//lado direito
		glColor3f(0.952, 0.125, 0.752);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);

		//lado frente
		glColor3f(0.058, 0.054, 0.725);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);

		//lado trás
		glColor3f(0.725, 0.709, 0.054);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		

	glEnd();



	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void reactKey(unsigned char key, int x, int y) {

	switch (key) {
	case 'a':
		tx -= 0.1;
		break;
	case 'd':
		tx += 0.1;
		break;
	case 'w':
		tz += 0.1;
		break;
	case 's':
		tz -= 0.1;
		break;
	case 'h':
		angle += 1.0;
		break;
	case 'j':
		angle -= 1.0;
		break;

	}
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
	glutCreateWindow("Prática 2");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(reactKey);
	glutSpecialFunc(escolherModo);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
