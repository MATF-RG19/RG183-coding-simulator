#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

static void on_display();
static void on_reshape(int width, int height);
void draw_girl(void);
void draw_boy(void);

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_position[] = {-1, 3, 2, 1};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glClearColor(0.7, 0.7, 0.7, 0);
    glutMainLoop();

    return 0;
}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) width/height, 1, 20);
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(7, 6, 12,
              0, 0, 0,
              0, 1, 0);

    glPushMatrix();
        /* Testing boy/girl */
        draw_girl();
        glTranslatef(3,0,0);
        draw_boy();
    glPopMatrix();

    glutSwapBuffers();
}

void draw_girl()
{
	/* body */
    glPushMatrix();
    	glTranslatef(0, 0.4, 0.0);
    	glRotatef(90,0,0,1);
    	glScalef(1.3,1.3,1.3);
    	glutSolidTetrahedron();
    glPopMatrix();
    
    /* head */
    glPushMatrix();
    	glTranslatef(0,2.1,0);
    	glScalef(0.65,0.65,0.65);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    
    /* hair */
    glPushMatrix();
    	glTranslatef(0.6,2.5, 0.5);
    	glScalef(0.2,0.2,0.2);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    glPushMatrix();
    	glTranslatef(-0.6,2.5, 0.5);
    	glScalef(0.2,0.2,0.2);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
}

void draw_boy()
{
	/* body */
	glPushMatrix();
    	glTranslatef(0,1,0);
    	glScalef(1.3,1.2,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    /* legs */
    glPushMatrix();
    	glTranslatef(0.34,0.08,0);
    	glScalef(0.6,0.6,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(-0.34,0.08,0);
    	glScalef(0.6,0.6,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    /* head */
    glPushMatrix();
    	glTranslatef(0,2.1,0);
    	glScalef(0.65,0.65,0.65);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    
    /* hair */
    glPushMatrix();
    	glTranslatef(0,2.9, 0);
    	glScalef(0.2,0.2,0.2);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
}

