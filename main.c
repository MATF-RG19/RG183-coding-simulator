#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#define TIMER_INTERVAL 1
#define TIMER_ID 0

#define PI 3.1415926535897

#define MAX_NUM_MOVES 200

static void on_display();
static void on_reshape(int width, int height);
void draw_girl(void);
void draw_boy(void);
void draw_floor(int num_tiles);
void draw_triangle_carpet(void);
void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);
void change_key_pressed(char);
void draw_special();
void set_arena_for_level(int);
int check_all_specials_activated();

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

char array_of_moves[MAX_NUM_MOVES];
int current_move_index = 0;

int pressed_enter = 0;

int is_current_special_activated = 0;
int current_level = 1;

typedef struct simple_tile_struct {
	int x;
	int z;
} simple_tile;

typedef struct special_tile_struct {
	int x;
	int z;
	int activated;
} special_tile;
special_tile array_special_tiles[15];
simple_tile array_simple_tiles[40];

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
	array_of_moves[0] = 'q';
	set_arena_for_level(current_level);
	
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

void set_arena_for_level(int level)
{
	switch(level)
	{
		case 1:
			array_special_tiles[0].x = 0;
			array_special_tiles[0].z = -2;
			array_special_tiles[0].activated = 0; 
			array_special_tiles[1].x = -1;
			array_special_tiles[1].z = -2;
			array_special_tiles[1].activated = 0;
			array_special_tiles[2].x = INT_MAX;
			
			array_simple_tiles[0].x = 0; 
			array_simple_tiles[0].z = 0;
			array_simple_tiles[1].x = 0; 
			array_simple_tiles[1].z = -1;
			array_simple_tiles[2].x = -1; 
			array_simple_tiles[2].z = 0;
			array_simple_tiles[3].x = -1; 
			array_simple_tiles[3].z = -1;
			array_simple_tiles[4].x = INT_MAX;
			break;
	}
}

void draw_special() // only if e was pressed on a special tile
{
	if (!is_current_special_activated)
		return;
		
	glPushAttrib(GL_LIGHTING_BIT);
	
	x = previous_tile_x*(-3.75) - 0.3;
	z = previous_tile_z*(-3.75);
	float special_y = - animation_parameter;
	
	glPushAttrib(GL_LIGHTING_BIT);
	GLfloat mat_ambient_special[] ={ 0.5, 1, 1, 1 };
	GLfloat mat_diffuse_special[] ={ 0.5, 1, 1, 1 };
	GLfloat mat_specular_special[] ={ 0.5, 1, 1, 0.922f };
	GLfloat shine_special[] = { 11.264f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient_special);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_special);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_special);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_special);
    
    glPushMatrix();
    	glTranslatef(x, special_y, z);
    	glScalef(3.5,0.3,3.5);
    	glutSolidCube(1);
    glPopMatrix();
    
    glPopAttrib();
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
    	draw_special();
        draw_girl();
        glPushMatrix();
        //glTranslatef(-3.75, 0, 0);
        //draw_boy();
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
	
	int current_simple_index = 0;
	simple_tile current_simple;
	
	int current_special_index = 0;
	special_tile current_special;
	
	glPushAttrib(GL_LIGHTING_BIT);
	
	GLfloat mat_ambient[] ={ 1, 1, 1, 1 };
	GLfloat mat_diffuse[] ={ 1, 1, 1, 1 };
	GLfloat mat_specular[] ={ 1, 1, 1, 0.922f };
	GLfloat shine[] = { 11.264f };
	
	GLfloat mat_ambient_special[] ={ 0.5, 1, 1, 1 };
	GLfloat mat_diffuse_special[] ={ 0.5, 1, 1, 1 };
	GLfloat mat_specular_special[] ={ 0.5, 1, 1, 0.922f };
	GLfloat shine_special[] = { 11.264f };
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	
	while (array_simple_tiles[current_simple_index].x != INT_MAX)
	{
		current_simple = array_simple_tiles[current_simple_index];
	
		glPushMatrix();
			glTranslatef(translate_by_x * current_simple.x, 0 , translate_by_z * current_simple.z);
			glScalef(3.5,0.3,3.5);
			glutSolidCube(1);
    	glPopMatrix();
    	
    	current_simple_index++;
	}
	
	while (array_special_tiles[current_special_index].x != INT_MAX)
	{
		current_special = array_special_tiles[current_special_index];
	
		if (current_special.activated)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		}
		else 
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient_special);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_special);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_special);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_special);
		}
	
		glPushMatrix();
			glTranslatef(translate_by_x * current_special.x, 0 , translate_by_z * current_special.z);
			glScalef(3.5,0.3,3.5);
			glutSolidCube(1);
    	glPopMatrix();
    	
    	current_special_index++;
	}
	
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

void add_to_move_array(char move) 
{
	array_of_moves[current_move_index] = move;
	current_move_index++;
	array_of_moves[current_move_index] = 'q';
}

void on_keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'w':
        case 'W': 
        case 's': 
        case 'S':
        case 'a':
        case 'A':
        case 'd':
        case 'D':
        case 'E':
        case 'e':
        	if (pressed_enter)
        		break;
        	add_to_move_array(tolower(key));
        	break;  
        case 13: // code for ENTER
        	if (pressed_enter) 
        		break;
        	pressed_enter = 1;
        	current_move_index = 0;
			change_key_pressed(array_of_moves[current_move_index]);
        	animation_ongoing = 1;
        	glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        	break;
        case 27:
          exit(0);
          break;
    }
}

void on_timer(int id) {
    
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
			
			// TODO change
			is_current_special_activated = 0;
		}
		else {
			animation_parameter += 0.02;	
		}
		
		glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
	}
	else
	{
		if(check_all_specials_activated())
		{
			printf("Level Complete!\n");
			// TODO move all initializations to level set up method so that everything's reset properly
		}
		else 
		{
			current_move_index++;
			char current = array_of_moves[current_move_index];
			if (current == 'q') 
			{
				// do nothing
			}
			else // w a s d e
			{
				if (current == 'e')
				{
					int current_special_index = 0;
					while (array_special_tiles[current_special_index].x != INT_MAX)
					{
						special_tile current_special = array_special_tiles[current_special_index];
						if (current_special.x == -previous_tile_x && current_special.z == -previous_tile_z)
						{
							array_special_tiles[current_special_index].activated = 1;
							is_current_special_activated = 1;
						}
						current_special_index++;
					} 
				}
				
				change_key_pressed(array_of_moves[current_move_index]);
				animation_ongoing = 1;
				animation_parameter = 0;
				glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
			}
		}
	}
}

int check_all_specials_activated()
{
	int current_special_index = 0;
	
	while (array_special_tiles[current_special_index].x != INT_MAX)
	{
		if (array_special_tiles[current_special_index].activated == 0)
		{
			return 0;
		}
		current_special_index++;
	} 
	
	return 1;
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
    

