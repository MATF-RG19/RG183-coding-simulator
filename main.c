#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define TIMER_INTERVAL 1
#define TIMER_ID 0

#define PI 3.1415926535897

static void on_display();
static void on_reshape(int width, int height);
void draw_girl(void);
void draw_boy(void);
void draw_floor(int num_tiles);
void draw_triangle_carpet(void);
void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);
void change_key_pressed(char);

/* used to keep track of the light source so a small cube acting as a 
   lightbulb can be drawn to more easily see where the light is coming from */
static int light_x = 3;
static int light_y = 4;
static int light_z = -5;

float random_array[500];
float animation_parameter = 0;
int animation_parameter_x = 0;
int animation_parameter_z = 0;
int pressed_a = 0;
int pressed_w = 0;
int pressed_s = 0;
int pressed_d = 0;
int animation_ongoing = 0;
int privremeni_brojac = 0;
float z = 0;
float x = 0;
float y = 0;
int previous_tile_z = 0;
int previous_tile_x = 0;

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

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
	
	/* initializing carpet */
	int i;
	for (i = 0; i < 500; i++)
	{
		random_array[i] = (float) rand() / RAND_MAX;
	}
	
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
    
    gluLookAt(18, 16, 22,
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
        glTranslatef(-3.75, 0, 0);
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
	
	x = previous_tile_x*(-3.75);
	z = previous_tile_z*(-3.75);
	y = sin(animation_parameter * PI);
	
	if (pressed_a)
		x += animation_parameter*(-3.75);
	if (pressed_w)
		z += animation_parameter*(-3.75);
	if (pressed_d)
		x += animation_parameter*(3.75);
	if(pressed_s)
		z += animation_parameter*(3.75);
	
	GLfloat mat_ambient[] ={ 0.7, 0.0, 0.2, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	/* body */
	glPushMatrix();
		glTranslatef(x, y + 0.6, z);
		glRotatef(25,0,1,0);
		glRotatef(90,0,0,1);
		glScalef(1.3,1.3,1.3);
		glutSolidTetrahedron();
	glPopMatrix();
	
	/* head */
	glPushMatrix();
		glTranslatef(x,y + 2.1, z);
		glScalef(0.65,0.65,0.65);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	
	/* hair */
	glPushMatrix();
		glTranslatef(x + 0.6,y + 2.5, z + 0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(x - 0.6,y +2.5, z + 0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	
	glPopAttrib();
}

void draw_boy()
{
	glPushAttrib(GL_LIGHTING_BIT);
	
	x = previous_tile_x*(-3.75);
	z = previous_tile_z*(-3.75);
	y = sin(animation_parameter * PI);
	
	if (pressed_a)
		x += animation_parameter*(-3.75);
	if (pressed_w)
		z += animation_parameter*(-3.75);
	if (pressed_d)
		x += animation_parameter*(3.75);
	if(pressed_s)
		z += animation_parameter*(3.75);

	GLfloat mat_ambient[] ={ 0.05, 0.0, 1, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	/* body */
	glPushMatrix();
    	glTranslatef(x,y + 1.3,z);
    	glScalef(1.3,1.2,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    /* legs */
    glPushMatrix();
    	glTranslatef(x - 0.34,y + 0.5,z);
    	glScalef(0.6,0.6,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(x + 0.34,y + 0.5,z);
    	glScalef(0.6,0.6,1);
    	glutSolidCube(1);
    glPopMatrix();
    
    /* head */
    glPushMatrix();
    	glTranslatef(x,y +2.4,z);
    	glScalef(0.65,0.65,0.65);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    
    /* hair */
    glPushMatrix();
    	glTranslatef(x,y +3.2, z);
    	glScalef(0.2,0.2,0.2);
    	glutSolidSphere(1, 16, 16);
    glPopMatrix();
    
    glPopAttrib();
}

void draw_floor(int num_tiles)
{
	glPushMatrix();
	
	float translate_by_x = 3.75;
	float translate_by_z = 3.75;

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
    
    glPushMatrix();
    	glTranslatef(0,0, -translate_by_z*2);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(-translate_by_x,0, -translate_by_z*2);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(-translate_by_x,0, -translate_by_z);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    	glTranslatef(-translate_by_x,0, 0);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
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
	int index_of_random_array = 0;
	
	for(x = -40; x < 40; x = x + 4)
	{
		for(z = -40; z < 40; z = z + 4)
		{
			float random_position = random_array[index_of_random_array++];
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

void on_keyboard(unsigned char key, int x, int y) {

	change_key_pressed(key);
    switch(key) {
        case 'w':
        case 'W': 
        case 's': 
        case 'S':
        case 'a':
        case 'A':
        case 'd':
        case 'D':
        	animation_ongoing = 1;
        	glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        	break;  
        case 27:
          exit(0);
          break;
    }
}

void on_timer(int id) {
    if (id == TIMER_ID) {

		
    }
    
    glutPostRedisplay();
    
	if (animation_ongoing)
	{
		if (animation_parameter >= 1)
		{
			animation_parameter = 0;
			animation_ongoing = 0;
			if (pressed_w)
				previous_tile_z++;
			if (pressed_a)
				previous_tile_x++;
			if (pressed_s)
				previous_tile_z--;
			if (pressed_d)
				previous_tile_x--;
		}
		else {
			animation_parameter += 0.02;
			glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);	
		}
	}
}

void change_key_pressed(char key)
{
	pressed_a = 0;
	pressed_w = 0;
	pressed_s = 0;
	pressed_d = 0;
        	
	switch(key) {
        case 'w':
        case 'W': 
        	pressed_w = 1;
            break;
        case 's': 
        case 'S':
        	pressed_s = 1;
            break;
        case 'a':
        case 'A':
        	pressed_a = 1;
            break;
        case 'd':
        case 'D':
        	pressed_d = 1;
        	break;  
        case 27:
          exit(0);
          break;
    }
}
    

