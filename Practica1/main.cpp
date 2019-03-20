#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <GL/glut.h>

#define KEY_ESC 27

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(0, 0);
	glVertex2d(1, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, 1);
	glEnd();
}
///dibujar cuadrado
void displayCuad(int tam, int x , int y){

    int tem = tam/2;
    glBegin(GL_LINE_LOOP);
	glColor3d(250,250,250);
	glVertex2d(x-tem, y+tem);
	glVertex2d(x+tem, y+tem);
	glVertex2d(x+tem, y-tem);
	glVertex2d(x-tem, y-tem);
	glEnd();

}
///dibujar circulo
void displayCircle(double Xpos, double Ypos, double radio){

    int cantpoints = 100;
    double angulo = 360/cantpoints;
    glBegin(GL_LINE_LOOP);
    glColor3d(255,250,250);

    for(double alpha = 0 ; alpha < 360; alpha =alpha + angulo){
        double x2 = radio*sin(alpha*3.1416/180)+Xpos;
        double y2 = radio*cos(alpha*3.1416/180)+Ypos;
        glVertex2f(x2,y2);
    }
    glEnd();
}

///dibuja circulos con porcentaje de reduccion
void displayCircles(int Xpos, int Ypos,int cantCircles, double reduction){
    double r = 15;
    double newR;
    for(int i = 0 ; i < cantCircles ; i++){
        displayCircle(Xpos, Ypos, r);
        newR = r-ceil(r*reduction/100);
        Xpos = Xpos-(r-newR);
        r = newR;
    }
}

///citculos juntos usando porcentaje de reduccion
void displayCirclesJuntos(int Xpos, int Ypos,int cantCircles, double reduction){

    double r = 10;
    double newR;
    for(int i = 0 ; i < cantCircles ; i++){
        displayCircle(Xpos, Ypos, r);
        newR = r-ceil(r*reduction/100);
        Xpos = Xpos+(r+newR);
        r = newR;
    }
}

///citculos juntos con angulo usnado porcetaje de reduccion
void displayCirclesAngulo(float Xpos, float Ypos,int cantCircles, double reduction, double angulo){
    double r = 5;
    double newR;
    double angluloRad = angulo*3.1416/180;

    for(int i = 0 ; i < cantCircles ; i++){
        displayCircle(Xpos, Ypos, r);
        newR = r-(r*reduction/100);
        Xpos = Xpos+((r+newR)*sin(angluloRad));
        Ypos = Ypos+((r+newR)*cos(angluloRad));
        r = newR;

    }

}



//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();


	//dibuja el gizmo
	//displayGizmo();
    displayCuad(15,0,0);
    displayCircle(-20,20,5);
    displayCircles(20,20,10,4);
    displayCirclesJuntos(-20,-20,6,28);
	displayCirclesAngulo(20,-39,5,28,180);
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f);
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}



GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutMainLoop(); //bucle de rendering

	return 0;
}
