#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

static void on_display();
static void on_reshape(int width, int height);
void draw_girl(void);
void draw_boy(void);
void draw_floor(int num_tiles);
void draw_triangle_carpet(void);

/* used to keep track of the light source so a small cube acting as a 
   lightbulb can be drawn to more easily see where the light is coming from */
static int light_x = 3;
static int light_y = 4;
static int light_z = -5;

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

    float light_position[] = {light_x,light_y,light_z,1};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	
	srand(time(NULL));
	
    glClearColor(0,0, 0, 0);
    glutMainLoop();

    return 0;
}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) width/height, 1, 60);
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(15, 13, 18,
              0, 0, 0,
              0, 1, 0);
    
    /*
    gluLookAt(0, 0, 15,
              0, 0, 0,
              0, 1, 0);
	*/
	/*
	gluLookAt(-15, 0, 0,
              0, 0, 0,
              0, 1, 0);
    */
    
    glPushMatrix();
    	glTranslatef(light_x, light_y, light_z);
    	glutSolidCube(0.2);
    glPopMatrix();
        draw_girl();
        glPushMatrix();
        glTranslatef(3,0,0);
        draw_boy();
        glPopMatrix();
        draw_floor(1);
        draw_triangle_carpet();
    glPopMatrix();

    glutSwapBuffers();
}

void draw_girl()
{
	glPushAttrib(GL_LIGHTING_BIT);
	
	GLfloat mat_ambient[] ={ 0.7, 0.0, 0.2, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	/* body */
    glPushMatrix();
    	glTranslatef(0, 0.6, 0.0);
    	glRotatef(25,0,1,0);
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
    
    glPopAttrib();
}

void draw_boy()
{
	glPushAttrib(GL_LIGHTING_BIT);

	GLfloat mat_ambient[] ={ 0.05, 0.0, 1, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	/* body */
	glPushMatrix();
    	glTranslatef(0,1.3,0);
    	glScalef(1.3,1.2,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    /* legs */
    glPushMatrix();
    	/* glTranslatef(0.34,0.08,0); */
    	glTranslatef(-0.34,0.5,0);
    	glScalef(0.6,0.6,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(0.34,0.5,0);
    	glScalef(0.6,0.6,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    /* head */
    glPushMatrix();
    	glTranslatef(0,2.4,0);
    	glScalef(0.65,0.65,0.65);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    
    /* hair */
    glPushMatrix();
    	glTranslatef(0,3.2, 0);
    	glScalef(0.2,0.2,0.2);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    
    glPopAttrib();
}

void draw_floor(int num_tiles)
{
	float translate_by_z = 4;

	glTranslatef(-0.3,0,0);

	glPushAttrib(GL_LIGHTING_BIT);

	GLfloat mat_ambient[] ={ 1,1,1,1};
	GLfloat mat_diffuse[] ={1,1,1,1 };
	GLfloat mat_specular[] ={ 1,1,1, 0.922f};
	GLfloat shine[] = { 11.264f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    
	glPushMatrix();
    	glTranslatef(0,0,0);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(0,0, -translate_by_z);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPopAttrib();
}

void draw_triangle_carpet()
{
	glPushAttrib(GL_LIGHTING_BIT);

	/* GLfloat mat_ambient[] ={ 1, 0.0, 1, 1.0f }; */
	GLfloat mat_ambient[] ={ 0.1, 1, 0.1, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	
	int x;
	int z;
	
	for(x = -40; x < 40; x = x + 4)
	{
		for(z = -40; z < 40; z = z + 4)
		{
			float random_position = (float) rand() / RAND_MAX;
			float position_z = z - random_position;
			float position_x = x + random_position;
			
			if (random_position < 0.5)
			{
				GLfloat mat_ambient[] ={ 0.1, 1, 0.1, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
			}
			else 
			{
				GLfloat mat_ambient[] ={ 1, 1, 0.3, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
			}
			
			glPushMatrix();
				glRotatef(90*random_position, 0, 1, 0);
			
				glBegin(GL_POLYGON);
					glVertex3f(position_x,-0.4,position_z);
					glVertex3f(position_x,-0.4, position_z - 0.7);
					glVertex3f(position_x + 0.7,-0.4,position_z);
				glEnd();
			glPopMatrix();
		}
	}
	
	glPopMatrix();
	
	glPopAttrib();
}
    

