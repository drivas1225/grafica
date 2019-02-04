#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <GL/glext.h>

#include "TextureManager.h"


using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


//variables para el gizmo
float delta_x = 0.0;
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

float tamHouse = 2;
float hTriangle = tamHouse;
static int posX = 0;    /// CUBE pos X
static int posY = 0;    /// CUBE pos Y

///posisicon de la iluminacion
GLfloat position[] = { 0.0, 0.0 , 0.0 , 1.0 };

///Normal Roof
    vector<float> NormalFrontRoof;
    vector<float> NormalBackRoof;


///Textura
GLint TextureGrass;
GLint TextureWall;
GLint TextureRoof;
GLint TextureTree;
GLint TextureWindow;
GLint TextureDoor;


vector<float> getNormal(vector<float> Vec1, vector<float> Vec2, vector<float> Vec3)
{
   vector<float> Normal;
   float Qx, Qy, Qz, Px, Py, Pz, vecUnit;

   Qx = Vec1[0]-Vec2[0];
   Qy = Vec1[1]-Vec2[1];
   Qz = Vec1[2]-Vec2[2];

   Px = Vec3[0]-Vec2[0];
   Py = Vec3[1]-Vec2[1];
   Pz = Vec3[2]-Vec2[2];

   Normal.push_back((Py*Qz) - (Pz*Qy));
   Normal.push_back((Pz*Qx) - (Px*Qz));
   Normal.push_back((Px*Qy) - (Py*Qx));

   vecUnit = sqrt(pow(Normal[0],2) +  pow(Normal[1],2)+ pow(Normal[2],2));
   Normal[0] = Normal[0] / vecUnit;
   Normal[1] = Normal[1] / vecUnit;
   Normal[2] = Normal[2] / vecUnit;

   return Normal;
}


///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
		break;




	}
}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{

	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


//function called on each frame
GLvoid window_idle();




GLvoid initGL()
{

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glClearColor(RED, GREEN, BLUE, ALPHA);

	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la corrección de perspectiva

    GLfloat mat_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat mat_defused[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat lightIntensity[] = {1.7f, 1.7f, 1.7f, 1.0f};
    //GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

}


void Gizmo3D(){


	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();


	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();



	return;

}

void grass()
{
    float tam = 10;

    glPushMatrix();
        //glColor3f(0.0f,1.0f,0.0f);
        glNormal3f( 0.0f , 1.0f ,0.0f );
        glBindTexture(GL_TEXTURE_2D, TextureGrass);
        glBegin(GL_QUADS);
            // Back Face
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-tam , 0 , -tam );
            glTexCoord2f(1.0f, 0.0f);glVertex3f(-tam , 0 , tam );
            glTexCoord2f(1.0f, 1.0f);glVertex3f( tam , 0 , tam );
            glTexCoord2f(0.0f, 1.0f);glVertex3f( tam , 0 , -tam );
        glEnd();

    glPopMatrix();

}

void building()
{

    glTranslated(0,tamHouse,0);
    glPushMatrix();
            ///Walls
            //glColor3f(0.51f,0.258f,0.13f);
            glBindTexture(GL_TEXTURE_2D, TextureWall);
            glNormal3f( 0.0f , 0.0f , 1.0f );
            glBegin(GL_QUADS);
                // Front Face
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-2*tamHouse, -tamHouse,  tamHouse);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 2*tamHouse, -tamHouse,  tamHouse);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 2*tamHouse, tamHouse,  tamHouse);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-2*tamHouse,  tamHouse,  tamHouse);
            glEnd();

            glNormal3f( 0.0f , 0.0f , -1.0f );
            glBegin(GL_QUADS);
                // Back Face
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-2*tamHouse, -tamHouse, -tamHouse);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-2*tamHouse,  tamHouse, -tamHouse);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( 2*tamHouse,  tamHouse, -tamHouse);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( 2*tamHouse, -tamHouse, -tamHouse);
            glEnd();

            ///Lateral walls

            glNormal3f( 1.0f , 0.0f , 0.0f );
            glBegin(GL_QUADS);
            // Right face
                glTexCoord2f(1.0f, 0.0f); glVertex3f( 2*tamHouse, -tamHouse, -tamHouse);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 2*tamHouse,  tamHouse, -tamHouse);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( 2*tamHouse,  tamHouse,  tamHouse);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( 2*tamHouse, -tamHouse,  tamHouse);
            glEnd();

            glNormal3f( -1.0f , 0.0f , 0.0f );
            glBegin(GL_QUADS);
                // Left Face
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-2*tamHouse, -tamHouse, -tamHouse);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-2*tamHouse, -tamHouse,  tamHouse);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-2*tamHouse,  tamHouse,  tamHouse);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-2*tamHouse,  tamHouse, -tamHouse);
            glEnd();

            ///Lateral triangles walls

            glNormal3f( 1.0f , 0.0f , 0.0f );
            glBegin(GL_TRIANGLES);
                // Right triangle wall
                glTexCoord2f(1.0f, 1.0f); glVertex3f( 2*tamHouse,  tamHouse, -tamHouse);
                glTexCoord2f(0.5f, 0.5f); glVertex3f( 2*tamHouse, tamHouse+hTriangle, 0);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( 2*tamHouse,  tamHouse,  tamHouse);
            glEnd();

            glNormal3f( -1.0f , 0.0f , 0.0f );
            glBegin(GL_TRIANGLES);
                // Left triangle wall
                glTexCoord2f(1.0f, 1.0f); glVertex3f( -2*tamHouse, tamHouse,  tamHouse);
                glTexCoord2f(0.5f, 0.5f); glVertex3f( -2*tamHouse, tamHouse+hTriangle ,0);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( -2*tamHouse,  tamHouse,  -tamHouse);

            glEnd();

            ///Roof
            //glColor3f(0.815f,0.815f,0.815f);
            glBindTexture(GL_TEXTURE_2D, TextureRoof);
            glNormal3f(NormalFrontRoof[0],NormalFrontRoof[1],NormalFrontRoof[2]);
            glBegin(GL_QUADS);
                //Front roof
                glTexCoord2f(1.0f, 1.0f);  glVertex3f( -2*tamHouse, tamHouse,  tamHouse);
                glTexCoord2f(0.0f, 1.0f);  glVertex3f(  2*tamHouse, tamHouse,  tamHouse);
                glTexCoord2f(0.0f, 0.26f); glVertex3f(  2*tamHouse, tamHouse+hTriangle, 0.0f);
                glTexCoord2f(1.0f, 0.26f); glVertex3f( -2*tamHouse, tamHouse+hTriangle, 0.0f);
            glEnd();

            glNormal3f(NormalBackRoof[0],NormalBackRoof[1],NormalBackRoof[2]);
            glBegin(GL_QUADS);
                //Back roof
                glTexCoord2f(1.0f, 1.0f);  glVertex3f(  2*tamHouse,  tamHouse, -tamHouse);
                glTexCoord2f(0.0f, 1.0f);  glVertex3f( -2*tamHouse,  tamHouse, -tamHouse);
                glTexCoord2f(0.0f, 0.26f); glVertex3f( -2*tamHouse, tamHouse+hTriangle ,0.0f);
                glTexCoord2f(1.0f, 0.26f); glVertex3f(  2*tamHouse, tamHouse+hTriangle, 0.0f);
            glEnd();

            ///door and windows
            //glColor3f(0.815f,0.815f,0.815f);
            ///right windows
            glBindTexture(GL_TEXTURE_2D, TextureWindow);
            glNormal3f( 0.0f , 0.0f , 1.0f );
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-tamHouse     , -tamHouse/2 ,  tamHouse+0.1 );
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-tamHouse*1/2 , -tamHouse/2 ,  tamHouse+0.1 );
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-tamHouse*1/2 , tamHouse/2  ,  tamHouse+0.1 );
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-tamHouse     ,  tamHouse/2 ,  tamHouse+0.1 );
            glEnd();

            ///left windows
            glNormal3f( 0.0f , 0.0f , 1.0f );
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( tamHouse*1/2 , -tamHouse/2 ,  tamHouse+0.1 );
                glTexCoord2f(1.0f, 0.0f); glVertex3f( tamHouse     , -tamHouse/2 ,  tamHouse+0.1 );
                glTexCoord2f(1.0f, 1.0f); glVertex3f( tamHouse     ,  tamHouse/2 ,  tamHouse+0.1 );
                glTexCoord2f(0.0f, 1.0f); glVertex3f( tamHouse*1/2 ,  tamHouse/2 ,  tamHouse+0.1 );
            glEnd();

            ///door
            glBindTexture(GL_TEXTURE_2D, TextureDoor);
            glNormal3f( 0.0f , 0.0f , 1.0f );
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-tamHouse/4, -tamHouse   ,  tamHouse+0.1 );
                glTexCoord2f(1.0f, 0.0f); glVertex3f( tamHouse/4, -tamHouse   ,  tamHouse+0.1 );
                glTexCoord2f(1.0f, 1.0f); glVertex3f( tamHouse/4,  tamHouse/3 ,  tamHouse+0.1 );
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-tamHouse/4,  tamHouse/3 ,  tamHouse+0.1 );
            glEnd();

    glPopMatrix();

}

void displayArbol()
{
    glPushMatrix();
        glNormal3f( 0.0f , 0.0f , 1.0f );
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-tamHouse, -tamHouse,  0);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( tamHouse, -tamHouse,  0);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( tamHouse,  3*tamHouse,  0);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-tamHouse,  3*tamHouse,  0);
        glEnd();
    glPopMatrix();
}

void Billboard()
{

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, TextureTree);
        for(int i=0;i<360;i+=360/4)
		{
			glRotatef(i, 0.0, 1.0, 0.0);
			displayArbol();
		}
    glDisable(GL_BLEND);
}


GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f, 1.0f, 0.01f, 100.0f);


	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);


        ///Light
        glLightfv (GL_LIGHT0, GL_POSITION, position);
        glPushMatrix();
            glTranslated(position[0],position[1],position[2]);
            glutSolidCube(0.1);
        glPopMatrix();

        grass();
        building();

        glPushMatrix();
        glTranslated(7,0,7);
        Billboard();
        glPopMatrix();
	//Gizmo3D();




	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;
    case 'w':
        position[2]-=1;
        break;
    case 's':
        position[2]+=1;
        break;
    case 'a':
        position[0]-=1;
        break;
    case 'd':
        position[0]+=1;
        break;
	case 'q':
        position[1]-=1;
        break;
	case 'e':
        position[1]+=1;
        break;
	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}


void calcularNormas(){

    NormalFrontRoof = getNormal({-2*tamHouse,tamHouse,tamHouse},{2*tamHouse,tamHouse,tamHouse},{2*tamHouse,tamHouse+hTriangle,0});
    NormalBackRoof = getNormal({2*tamHouse,  tamHouse, -tamHouse},{-2*tamHouse,  tamHouse, -tamHouse},{-2*tamHouse, tamHouse+hTriangle , 0});
}

void LoadTexture()
{
    TextureGrass = TextureManager::Inst()->LoadTexture("C:/Users/Lenovo/Desktop/practicas glut/Practica4/GrassSamp1.png", GL_BGR_EXT, GL_RGB);
	TextureWall  = TextureManager::Inst()->LoadTexture("C:/Users/Lenovo/Desktop/practicas glut/Practica4/bricks.png", GL_BGR_EXT, GL_RGB);
	TextureRoof  = TextureManager::Inst()->LoadTexture("C:/Users/Lenovo/Desktop/practicas glut/Practica4/roof.png", GL_BGR_EXT, GL_RGB);
	TextureTree  = TextureManager::Inst()->LoadTexture("C:/Users/Lenovo/Desktop/practicas glut/Practica4/tree.png", GL_BGRA_EXT, GL_RGBA);
	TextureWindow= TextureManager::Inst()->LoadTexture("C:/Users/Lenovo/Desktop/practicas glut/Practica4/window.png", GL_BGR_EXT, GL_RGB);
	TextureDoor  = TextureManager::Inst()->LoadTexture("C:/Users/Lenovo/Desktop/practicas glut/Practica4/door.png", GL_BGR_EXT, GL_RGB);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 3 : Transformations 3D Part 2");

    calcularNormas();
    LoadTexture();

	initGL();
	init_scene();


	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);


	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}



