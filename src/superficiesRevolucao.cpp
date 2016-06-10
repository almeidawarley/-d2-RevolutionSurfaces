/*
    Name:        superficieRevolucao.cpp
    Copyright:   Version 0.1
    Author:      Lucas Carvalho Ribeiro
                 Warley Almeida Silva
    Date:        10/06/2016
    Description: Draw revolution surfaces based on a b-spline curve
*/


#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glui.h>
#include <GL/glut.h>
#include <iostream>
#include "Ponto.h"
#include "ListaEncad.h"
#include "glcText.h"
#include <math.h>
#include <string.h>
#include <fstream>
#define PI 3.14

using namespace std;
float prec = 100;
float t;
char str;
ListaEncad *l = new ListaEncad();
glcText *text = NULL;
int i;
float x=0,y=0, z=0;
float *xadj, *yadj;
int pontomovimento = -1;
int ultimox, ultimoy;
int largura = 400, altura = 400;
int passo = 10;
float **c1, **c2;
float nx = 0, ny = 0, nz = 0;
bool superficie = false;
int tamanho = 0;
int xrot = 0, yrot = 0;
float xrotation = 0, yrotation = 0;

/*
    Fun��o de cada uma das variaveis declaradas:
    - float prec = ir� definir a precisao, ou seja, quantidade de pontos dentro da curva;
    - float t, int i = vari�veis auxiliares para estruturas de for;
    - char str = v�riavel auxiliar para convers�o de int para string;
    - ListaEncad*l = lista encadeada com os pontos guardados, alocada dinamicamente;
    - glc Text *text = vetor de objetos do tipo glcText que renderizam a numera��o dos pontos;
    - float x,float y = vari�veis que guardam as coordenadas dos pontos das curvas baseados nos pontos de controle ao redor;
    - float *xadj, float *yadj = vetores alocados dinamicamente que recebem todos os valores X e Y dos pontos da lista;
    - int pontomovimento = vari�vel que guarda o in�dice do ponto que est� sendo movimentado;
    - int ultimox, int ultimoy = guarda os valores do in�cio do movimento para calcular qual � a taxa de varia��o no eixo x e no eixo y;
    - int largura, int altura = vari�veis que guardam o tamanho da tela para que possam ser feitas as adapta��es necess�rias caso o usu�rio aumente a tela;
    - int passo = indica o passo de refinamento da revolu��o;
    - float **c1, **c2 = matrizes alocadas dinamicamente na hora da costura;
    - float nx, ny, nz = componentes x, y e z da normal;
    - bool superficie = vari�vel que controla em qual momento de exibi��o estamos;
    - int tamanho = guardar� sempre o n�mero de pontos que precisaremos rotacionar, dependendo da precisao da curva;
    - int xrot, yrot = vari�veis utilizadas na movimenta��o no eixo x e y da nossa superf�cie;
    - float xrotation, yrotation = vari�veis uilizadas na rota��o da c�mera;


*/

int mainWindow;
int wireframe = true;

GLUI_Checkbox   *checkbox;
GLUI *glui;

/*
    As fun��es carregaDeArquivo() e atualizaArquivo() tem por objetivo atualizar um arquivo ponto.txt com os pontos salvos
*/

void carregaDeArquivo(){
    ifstream arquivo;
    float xarq, yarq;
    arquivo.open("pontos.txt");
    while(arquivo>>xarq>>yarq){
        l->insere(xarq, yarq);
        cout << "Lendo - X : " << xarq << " , Y : " << yarq << endl;
    }
    glutPostRedisplay();
}

void atualizaArquivo(){
    int t, i;
    float *xj, *yj;
    t = l->consultaTamanho();
    xj = new float [t];
    yj = new float [t];
    ofstream arquivo;
    remove("pontos.txt");
    arquivo.open("pontos.txt");
    cout << "Reescrevendo pontos.txt ..." << endl;
    l->retornaPontos(0, xj, yj);
    for(i=0; i<t; i++){
        arquivo<<xj[i] << " " <<yj[i] << "\n";
    }
    delete xj;
    delete yj;
    glutPostRedisplay();

}

void configuraIluminacao(){

    /*
        Nesta fun��o, s�o definidas as luzes utilizadas na exibi��o 3D
    */

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    GLfloat posicao_luz[] = { 0, 10.0,  10.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
}

void configuraMaterial(){
    /*
        Nesta fun��o, s�o definidas as caracter�sticas do material que dar� origem � superf�cie
    */

   GLfloat objeto_ambient[]   = { .1745, .01175, .01175, 1.0 };
   GLfloat objeto_difusa[]    = { .61424, .04136, .04136, 1.0 };
   GLfloat objeto_especular[] = { .727811, .626959, .626959, 1.0 };
   GLfloat objeto_brilho[]    = { 90.0f };

   glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
   glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
   glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void calculaNormal(int indice, int tipotriangulo){
    float u[3], v[3]; //vetores U e V que usarei no c�lculo da normal

    /*
    Calculamos a normal a partir dos pontos armazenados em c1 e c2, seguindo a seguinte l�gica:

    U = p2 - p1
    V = p3 - p1

    p1, p2 e p3 variam nos dois poss�veis tipos de triangulo, por isso, criamos a vari�vel tipotriangulo para lidar
    com essa diferen�a

    Nx = UyVz - UzVy
    Ny = UzVx - UxVz
    Nz = UxVy - UyVx

    */
    if(tipotriangulo==1){
        u[0] = c2[indice][0] - c1[indice][0];
        u[1] = c2[indice][1] - c1[indice][1];
        u[2] = c2[indice][2] - c1[indice][2];
        v[0] = c1[indice+1][0] - c1[indice][0];
        v[1] = c1[indice+1][1] - c1[indice][1];
        v[2] = c1[indice+1][2] - c1[indice][2];

        nx = (u[1]*v[2] - u[2]*v[1])*-1;
        ny = (u[2]*v[0] - u[0]*v[2])*-1;
        nz = (u[0]*v[1] - u[1]*v[0])*-1;
    }
    if(tipotriangulo==2){
        u[0] = c2[indice][0] - c1[indice+1][0];
        u[1] = c2[indice][1] - c1[indice+1][1];
        u[2] = c2[indice][2] - c1[indice+1][2];
        v[0] = c2[indice+1][0] - c1[indice+1][0];
        v[1] = c2[indice+1][1] - c1[indice+1][1];
        v[2] = c2[indice+1][2] - c1[indice+1][2];
        nx = (u[1]*v[2] - u[2]*v[1])*-1;
        ny = (u[2]*v[0] - u[0]*v[2])*-1;
        nz = (u[0]*v[1] - u[1]*v[0])*-1;
    }
}

void costura(){

    /*
        Nesta fun��o, realizamos a costura, ou seja, a uni�o dos pontos revolucionados com tri�ngulos.
        Utilizamos aqui as vari�veis
        - tamanho, que ser� sempre atualizada com a quantidade de pontos em c1 e c2;
        - c1 e c2, que s�o matrizes tamanho x 3, com as coordenadas dos pontos da curva;
        - nx, ny, nz, que s�o as coordenadas da normal calculadas na fun��o acima.

    */

    for(i=0; i<tamanho-1; i++){
    /*
        Teste para impress�o dos pontos de c1 e c2
        glColor3f(1,0,0);
        glBegin(GL_POINTS);
            glVertex3f(c1[i][0], c1[i][1], c1[i][2]);
        glEnd();
        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex3f(c2[i][0], c2[i][1], c2[i][2]);
        glEnd();
    */
        configuraMaterial();
        glBegin(GL_TRIANGLES);
                glVertex3f(c1[i][0], c1[i][1], c1[i][2]);
                glVertex3f(c2[i][0], c2[i][1], c2[i][2]);
                glVertex3f(c1[i+1][0], c1[i+1][1], c1[i+1][2]);
                //cout << "oi";
               calculaNormal(i, 1);
               glNormal3f(nx, ny, nz);
            glEnd();

            glBegin(GL_TRIANGLES);
                glVertex3f(c1[i+1][0], c1[i+1][1], c1[i+1][2]);
                glVertex3f(c2[i][0], c2[i][1], c2[i][2]);
                glVertex3f(c2[i+1][0], c2[i+1][1], c2[i+1][2]);

                calculaNormal(i, 2);
                glNormal3f(nx, ny, nz);
            glEnd();

    }
}

void projecao2d(){

    /*
        Fun��o que controla a exibi��o 2D, grande parte do c�digo foi importado do D1
    */

    glClearColor (0.8, 0.8, 0.8, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glui->show();
    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
       glBegin(GL_LINES);
            glVertex2f(0, 10);
            glVertex2f(0, -10);
       glEnd();

       glBegin(GL_LINES);
            glVertex2f(-10, 0);
            glVertex2f(10,0);
       glEnd();

       glPointSize(5);
       int aux = l->consultaTamanho();      //Consultamos o tamanho da lista e declaramos vetores que buscam os valores de X e Y
       xadj = new float [aux];
       yadj = new float [aux];
       text = new glcText[aux];
       l->retornaPontos(0,xadj, yadj);      //Utilizamos uma fun��o da estrutura para que esses vetores sejam carregados de forma adequada
       glBegin(GL_POINTS);
            for(i=0; i<aux; i++){           //Imprimos todos os pontos de controle recuperados da estrutura
               glVertex3f(xadj[i],yadj[i], 0);
            }
       glEnd();
       for(i=0; i<aux; i++){                //Imprimimos, logo acima do ponto, seu respectivo �ndice, usando a classe glcText
            itoa(i, &str, 10);
            text[i].setAll(&str, 3, xadj[i], yadj[i]+1, 1, 0, 0);
            text[i].render();
        }
       if(wireframe){
       glBegin(GL_LINE_STRIP);
            for(i=0; i<aux; i++){           //Caso a exibi��o de pol�gono esteja ativada, exibimos uma conex�o entre os v�rtices atrav�s do GL_LINE_STRIP
               glVertex3f(xadj[i],yadj[i], 0);
            }
       glEnd();
       }

       glColor3f(0,0,1);
       glLineWidth(2.0);

           for(i=0; i<aux-3; i++){
               glBegin(GL_LINE_STRIP);
                for(t=0;t<1;t = t + 1/prec){
                    x=(((-1*pow(t,3) +3*pow(t,2)  -3*t +1)*xadj[i] +( 3*pow(t,3)  -6*pow(t,2) +4)*xadj[i+1] +(-3*pow(t,3) +3*pow(t,2) +3*t +1)*xadj[i+2] +( 1*pow(t,3))*xadj[i+3])/6);
                    y=(((-1*pow(t,3) +3*pow(t,2)  -3*t +1)*yadj[i] +( 3*pow(t,3)  -6*pow(t,2) +4)*yadj[i+1] +(-3*pow(t,3) +3*pow(t,2) +3*t +1)*yadj[i+2] +( 1*pow(t,3))*yadj[i+3])/6);
                    glVertex3f(x,y,0.0);
                    //cout << "x : " << x << " y: " << y << "z: " << z << endl;

                }
                glEnd();
            }
}

void projecao3d(){

        /*
            Fun��o que controla exibi��o 3D.
        */

        glEnable(GL_NORMALIZE);

        glClearColor (0.8, 0.8, 0.8, 0.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (GLfloat)largura/(GLfloat)altura, 10, -10);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        glRotatef(yrotation, 1.0, 0.0, 0.0 ); //xrotation e y rotation controlam a rota��o da c�mera
        glRotatef(xrotation, 0.0, 1.0, 0.0 );

        int contador = 0;
        int angle = 0;
        float auxvalor = 0;
        bool primeira = true;
        tamanho = 0;
        int aux = l->consultaTamanho();
        delete xadj;
        delete yadj;
        xadj = new float [aux];
        yadj = new float [aux];
        l->retornaPontos(0,xadj, yadj);
        //glui->hide();
        for(i=0;i<aux-3;i++)
        { //Nesta fun��o calculamos, com base nos atuais pontos da nossa lista, quantos pontos ter� cada curva
            for(t=0; t<1; t = t+1/prec){
                tamanho++;
            }
        }

        int cont;
        cont = tamanho/(aux-3);
        if(c1!=NULL)
            delete c1;
        if(c2!=NULL)
            delete c2;
        c1 = new float* [tamanho]; // Declaramos com base no tamanho as matrizes c1 e c2 que guardar�o os pontos
        c2 = new float* [tamanho];
        for(contador = 0; contador<tamanho; contador++){
                    c1[contador] = new float[3];
                    c2[contador] = new float[3];
                }
        primeira = true;
        //Iremos agora passar por todos os �ngulos, de 360 a 0
        for(angle = 360; angle>=-passo; angle = angle-passo){
           for(i=0; i<aux-3; i++){
                contador = 0;
                for(t=0;t<1;t = t + 1/prec){
                    x=(((-1*pow(t,3) +3*pow(t,2)  -3*t +1)*xadj[i] +( 3*pow(t,3)  -6*pow(t,2) +4)*xadj[i+1] +(-3*pow(t,3) +3*pow(t,2) +3*t +1)*xadj[i+2] +( 1*pow(t,3))*xadj[i+3])/6);
                    y=(((-1*pow(t,3) +3*pow(t,2)  -3*t +1)*yadj[i] +( 3*pow(t,3)  -6*pow(t,2) +4)*yadj[i+1] +(-3*pow(t,3) +3*pow(t,2) +3*t +1)*yadj[i+2] +( 1*pow(t,3))*yadj[i+3])/6);
                    auxvalor = x;
                    x = auxvalor*cosf((float) (angle*PI)/180);
                    z = auxvalor*sinf((float) (angle*PI)/180);

                    /*
                        Calculamos os valores de x, y e z transladados na parte acima. Preenchemos c2 com as coordenadas encontradas
                    */

                    c2[i*cont + contador][0] = x;
                    c2[i*cont + contador][1] = y;
                    c2[i*cont + contador][2] = z;
                    contador++;
                }

            }
         if(!primeira)
            {
                /*
                    Se n�o for a primeira intera��o, chamaremos a costura e faremos sempre c1 receber os valores de c2;
                */
                 costura();
                 for(int i=0;i<tamanho;i++)
                 {
                    c1[i][0] = c2[i][0];
                    c1[i][1] = c2[i][1];
                    c1[i][2] = c2[i][2];
                 }

            }
        else
            {
                /*
                    Se for a primeira intera��o, n�o chamaremos a costura, pois somente c2 possui valores.
                    Aproveitamos para passar os valores de c2 para c1 para que, na pr�xima intera��o, c2
                    seja atualizada e at� o fim c1 e c2 recebam os pontos corretamnte, sempre chamando a costura.
                */
                  for(int i=0;i<tamanho;i++)
                 {
                    c1[i][0] = c2[i][0];
                    c1[i][1] = c2[i][1];
                    c1[i][2] = c2[i][2];
                 }
                 primeira = false;
            }
        }
    //glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void idle(void)
{
    if(superficie)
    glutPostRedisplay();
}

void display (void)
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!superficie){
        projecao2d();
    }else{
        configuraIluminacao();
        configuraMaterial();
        projecao3d();
    }
   glutSwapBuffers ( );
}


void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
      exit(0);
      break;
   case 's':
        superficie = !superficie;
        glutPostRedisplay();
        break;
   case 'c':
        atualizaArquivo();
        break;
   }

}

void motion(int x, int y){
    if(!superficie){
        if(pontomovimento!=-1){
            l->atualizaPonto(pontomovimento, (float)(x-ultimox)/(largura/20), (float)(ultimoy-y)/(altura/20));
            /*

                Caso exista um ponto em movimento, seu �ndice est� salvo na vari�vel ponto em movimento.
                Quando est� � diferente de -1, iremos atualizando o ponto de acordo com a diferen�a das
                coordenadas atuais com as coordenadas iniciais, que foram informadas na fun��o mouse.

            */
            glutPostRedisplay();
            ultimox = x;
            ultimoy = y;
        }
    }else{
        xrotation += ((float)x - xrot);
        yrotation += ((float)y - yrot);
        xrot = (float)x;
        yrot = (float)y;
        glutPostRedisplay();

    }
}

void mouse (int button, int buttonState, int x, int y)
{
    /*
        Nesse momento, as vari�veis de largura e altura da tela s�o atualizadas, utilizando fun��es do OpenGL.
    */
    largura = glutGet(GLUT_WINDOW_WIDTH);
    altura = glutGet(GLUT_WINDOW_HEIGHT);

    if(!superficie){
        if(button == GLUT_LEFT_BUTTON && buttonState == GLUT_UP){
            /*
                Se o bot�o esquerdo for solto, n�o existe nenhum ponto em movimento.
                Por isso, atualizamos pontomovimento para seu valor padr�o, isto �, -1.
            */
            pontomovimento = -1;
        }
        if(button == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN){
            /*
                Se o bot�o esquerdo for pressionado, iremos analisar, inicialmente, se existe algum ponto naquela proximidade.
                Para isso, visitados todos os pontos da nossa lista encadeada e procuramos se h� algum ponto cadastrado com
                aquelas coordenadas. Se for encontrado um ponto, a vari�vel pontomovimento assume o valor do seu �ndice.
            */
            for (int i = 0; i<l->consultaTamanho() ;i++){
                  if (xadj[i]*(largura/20) + (largura/2-5)<x && xadj[i]*(largura/20)+(largura/2+5)>x && -(altura/20)*yadj[i]+(altura/2-5) < y && -(altura/20)*yadj[i]+(altura/2+5) > y){
                        pontomovimento = i;
                        ultimox = x;
                        ultimoy = y;
                    }
            }
            /*
                Se n�o houver nenhum ponto naquela proximidade, pontomovimento continuar� com seu valor padr�o, -1.
                Logo, podemos assumir que a inten��o do usu�rio � adicionar um novo ponto atrav�s de um clique, o que fazemos abaixo.
            */

            if(pontomovimento==-1){
                l->insere((float)(x-((float)largura/2))/(largura/20), (float)((altura/2)-y)/(altura/20));
                /*cout << "________________";
                cout << "Inserido : " << endl;
                l->imprime();
                cout << endl;*/
            }
            glutPostRedisplay();
        }
        if(button == GLUT_RIGHT_BUTTON && buttonState == GLUT_DOWN){
            /*
                Se o bot�o direito do mouse for pressionado, a �nica op��o de escolha � a exclus�o de um ponto.
                Para isso, verificamos a proximidade do local onde o mouse foi clicado e procuramos por um ponto que esteja ali.
                Caso ele exista, o ponto com esse determinado �ndice ser� eliminado.
            */
            for (int i = 0; i<l->consultaTamanho() ;i++){
                if (xadj[i]*(float)(largura/20) + (float)(largura/2-5)<x && xadj[i]*(float)(largura/20)+(float)(largura/2+5)>x && -(float)(altura/20)*yadj[i]+(float)(altura/2-5) < y && -(float)(altura/20)*yadj[i]+(float)(altura/2+5) > y){

                    l->elimina(i);
                    /*cout << "________________";
                    cout << "Removido : p" << i << endl;
                    l->imprime();
                    cout << endl;*/

                }
            }
            glutPostRedisplay();
        }
    }
    else{
        xrot = x;
        yrot = y;
    }
}
void control_cb(int control)
{
    /*
        Local de controle da vari�vel wireframe, que indica se o pol�gono ser� ou n�o exibido.
    */
    if(control == 0){
        wireframe = !wireframe;
        cout << wireframe;
    }
}

void menu(){
    cout << "**********************************************************************" << endl << endl;
    cout << "Superficies de Revolucao"<<endl<<endl;
    cout << "- Na interface da curva:"<<endl;
    cout << "Utilize o botao direito do mouse para adicionar novos pontos" << endl;
    cout << "Utilize o botao esquerdo do mouse para remover um determinado ponto" << endl;
    cout << "Clique em um ponto e segure para arrasta-lo na tela"<<endl;
    cout << "Ative e desative a exibicao de poligono atraves da caixa de selecao" << endl<<endl;
    cout << "- Na interface de superficie"<<endl;
    cout << "Para salvar a curva, utilize a tecla c"<<endl;
    cout << "Controle a exibicao do poligono atraves da tecla s"<<endl<<endl;
    cout << "***********************************************************************" << endl;

}

int main(int argc, char** argv)
{

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (largura, altura);
   glutInitWindowPosition (100, 100);
   mainWindow = glutCreateWindow ("Superficies de Revolucao");
   menu();
   glui = GLUI_Master.create_glui_subwindow(mainWindow);
   glui->set_main_gfx_window(mainWindow);
   checkbox = new GLUI_Checkbox(glui, "Poligono on/off", &wireframe, 1, control_cb );
    carregaDeArquivo();
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutIdleFunc(idle);

   glutMainLoop();
   return 0;
}
