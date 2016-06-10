/**************************************************/
/*                                                */
/* RODRIGO LUIS DE SOUZA DA SILVA    28/10/2004   */
/*                                                */
/*  			        circulo.cpp                   */
/*                                                */
/* LAST UPDATE: 28/10/2004                        */
/**************************************************/
/*

Animação de criação pra criar um círculo

*/

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <iostream>

#define NUM 60
#define PI 3.1415927
#define TAM 3
float ANGLE = 360/ NUM;

float current = 0;
float angle = 0;
float raio = 1.5;
float dir = 1;
bool idleOn = true;
bool aux = false;
void display(void);
void init (void);
void idle();
void mouse(int button, int state, int x, int y);

/*void costura(float c1[][], float c2[][]){
    int i;
    for(i=0; i<TAM; i++){
        glColor3f(1,0,0);
        glBegin(GL_POINTS);
            glVertex2f(c1[i][0], c1[i][1]);
        glEnd();
        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex2f(c2[i][0], c2[i][1]);
        glEnd();
    }
    glColor3f(0,0,1);
    for(i=0; i<TAM; i++){
        if(i==0){
            glBegin(GL_TRIANGLES);
                glVertex2f(c1[i][0], c1[i][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i+1][0], c1[i+1][1]);
            glEnd();
        }else if(i=TAM-1){
            glBegin(GL_TRIANGLES);
                glVertex2f(c1[i][0], c1[i][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i+1][0], c1[i+1][1]);
            glEnd();
        }else{
            glBegin(GL_TRIANGLES);
                glVertex2f(c2[i-1][0], c2[i-1][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i][0], c1[i][1]);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(c1[i][0], c1[i][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i+1][0], c1[i+1][1]);
            glEnd();
        }
    }
    glEnd();

}*/

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("hello");
	glutMouseFunc( mouse );
   glutIdleFunc( idle);
   init ();
   glutDisplayFunc(display);
   glutMainLoop();

   return 0;
}

void idle()
{
   if(current <= 360)
   {
      angle += ANGLE;
      current++;
   }
   glutPostRedisplay();
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      (idleOn) ? glutIdleFunc( NULL ) : glutIdleFunc( idle );
      idleOn = !idleOn;
   }
}

void display(void)
{
   // Limpar todos os pixels
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glColor3f (1.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
   float c1[TAM][2], c2[TAM][2];

   c1[0][0] = 7;
   c1[0][1] = 7;
   c1[1][0] = 5;
   c1[1][1] = 0;
   c1[2][0] = 7;
   c1[2][1] = -7;

   c2[0][0] = -7;
   c2[0][1] = 7;
   c2[1][0] = -5;
   c2[1][1] = 0;
   c2[2][0] = -7;
   c2[2][1] = -7;

   if(!aux){
    std::cout << c1[0][0] << c1[0][1];
    aux = true;
   }

   //algoritmo de costura

   int i;
    for(i=0; i<TAM; i++){
        glColor3f(1,0,0);
        glBegin(GL_POINTS);
            glVertex2f(c1[i][0], c1[i][1]);
        glEnd();
        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex2f(c2[i][0], c2[i][1]);
        glEnd();
    }
    glColor3f(0,0,1);
    for(i=0; i<TAM; i++){
        if(i==0){
            glBegin(GL_TRIANGLES);
                glVertex2f(c1[i][0], c1[i][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i+1][0], c1[i+1][1]);
            glEnd();
        }else if(i==TAM-1){
            glBegin(GL_TRIANGLES);
                glVertex2f(c2[i-1][0], c2[i-1][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i][0], c1[i][1]);
            glEnd();
        }else{
            glBegin(GL_TRIANGLES);
                glVertex2f(c2[i-1][0], c2[i-1][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i][0], c1[i][1]);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(c1[i][0], c1[i][1]);
                glVertex2f(c2[i][0], c2[i][1]);
                glVertex2f(c1[i+1][0], c1[i+1][1]);
            glEnd();
        }
    }
    //acaba aqui

   /*float g;
   int i;
   for(i = 0, g = 0.0f; i < current, g <= angle; i++, g+=ANGLE)
      glVertex2f (raio * cos(g * PI / 180), raio * sin(g * PI / 180));

   glEnd();*/
	glutSwapBuffers ();
}

void init (void)
{
  // selecionar cor de fundo (preto)
  glClearColor (0.0, 0.0, 0.0, 0.0);

  // inicializar sistema de viz.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10.0, 10.0, -10.0, 10.0, -2.0, 2.0);
  glLineWidth(3.0);
}

