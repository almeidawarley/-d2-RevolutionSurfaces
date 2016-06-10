#include <stdlib.h>
#include <GL/glut.h>

// Include this file
#include "glcText.h"
int width, height;

void draw(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   int size = 20;
   glOrtho(-size, size, -size, size, -size, size);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glDisable(GL_LIGHTING);
   // Criando o texto setando todos os parâmetros com um único comando
   glcText t1;
   t1.setAll("Bico", 0, 15, 6, 1.0,1.0,0.0 );

   // Criando um segundo texto utilizando todos os parâmetros da classe glcText
   glcText *text2 = new glcText();
      text2->setString("Tampa");
      text2->setColor(1.0,1.0,1.0);
      text2->setPos(-7,8);
      text2->setType(4);
      text2->render();
   glEnable(GL_LIGHTING);

   gluLookAt (0.0, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glutSolidTeapot(10.0);

   glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
   {
		case 27:	exit(0);
      break;
   }
}

void resizeWindow(int w, int h)
{
   width  = w;
   height = h;
   glViewport(0, 0, w, h);
}

void idle()
{
   glutPostRedisplay();
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   GLfloat cor_luz[]         = {1.0, 0.0, 0.0, 1.0};
	GLfloat posicao_luz[]     = { 50.0, 50.0, 50.0, 1.0};
   glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
   glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
   glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("glcTextTest");
   init();
   glutDisplayFunc(draw);
   glutReshapeFunc(resizeWindow);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
