#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#define TIMER_INTERVAL 10
#define TIMER_ID 0
#define TIMER_ID_CAMERA_OUT 1
#define TIMER_ID_CAMERA_IN 2
#define RANDOM_ARRAY_LENGTH 120
#define TRIANGLE_OPTIMIZATION_ARRAY_LENGTH 100

#define PI 3.1415926535897

#define MAX_NUM_MOVES 200

// TODO add animation and failure when the figure falls off the tiles


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
int last_special_tile_activated = 0;

/* used to keep track of the light source so a small cube acting as a 
   lightbulb can be drawn to more easily see where the light is coming from */
static int light_x = 3;
static int light_y = 4;
static int light_z = -5;

float random_array[RANDOM_ARRAY_LENGTH];
float animation_parameter = 0;
char current_pressed_key = '\0';
char previous_pressed_key = 'w'; // because at the beginning the figure is facing forward
int animation_ongoing = 0;
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

float camera_parameter_in_out = 0;
float camera_parameter_in_out_max = 0; // TODO change for each level depending on far you need to zoom out
float camera_parameter_z = 0;
float camera_parameter_x = 0;

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
	array_of_moves[0] = '\0';
	set_arena_for_level(current_level);
	
	/* initializing carpet */
	int i;
	for (i = 0; i < RANDOM_ARRAY_LENGTH; i++)
	{
		random_array[i] = (float) rand() / RAND_MAX;
	}
	
    glClearColor(0,0, 0, 0);
    glutMainLoop();

    return 0;
}

void set_arena_for_level(int level)
{

	animation_parameter = 0;
	current_pressed_key = '\0';
	previous_pressed_key = 'w'; // because at the beginning the figure is facing forward
	animation_ongoing = 0;
	z = 0;
	x = 0;
	y = 0;
	previous_tile_z = 0;
	previous_tile_x = 0;

	current_move_index = 0;

	pressed_enter = 0;

	is_current_special_activated = 0;

	camera_parameter_in_out = 0;
	camera_parameter_z = 0;
	camera_parameter_x = 0;

	array_of_moves[0] = '\0';

	glutTimerFunc(250, on_timer, TIMER_ID_CAMERA_OUT);

	switch(level)
	{
		case 1:
			camera_parameter_in_out_max = 0.7;
		
			array_special_tiles[0].x = 0;
			array_special_tiles[0].z = -3;
			array_special_tiles[0].activated = 0;
			array_special_tiles[1].x = INT_MAX;
			
			array_simple_tiles[0].x = 0; 
			array_simple_tiles[0].z = 0;
			array_simple_tiles[1].x = 0; 
			array_simple_tiles[1].z = -1;
			array_simple_tiles[2].x = 0; 
			array_simple_tiles[2].z = -2;
			array_simple_tiles[3].x = INT_MAX;
			break;
		case 2:
			camera_parameter_in_out_max = 1;
		
			array_special_tiles[0].x = 0;
			array_special_tiles[0].z = -2;
			array_special_tiles[0].activated = 0;
			array_special_tiles[1].x = 0;
			array_special_tiles[1].z = -5;
			array_special_tiles[1].activated = 0;
			array_special_tiles[2].x = -4;
			array_special_tiles[2].z = -5;
			array_special_tiles[2].activated = 0;
			array_special_tiles[3].x = INT_MAX;
			
			array_simple_tiles[0].x = 0; 
			array_simple_tiles[0].z = 0;
			array_simple_tiles[1].x = 0; 
			array_simple_tiles[1].z = -1;
			array_simple_tiles[2].x = 0; 
			array_simple_tiles[2].z = -3;
			array_simple_tiles[3].x = 0; 
			array_simple_tiles[3].z = -4;
			array_simple_tiles[4].x = -1; 
			array_simple_tiles[4].z = -5;
			array_simple_tiles[5].x = -2; 
			array_simple_tiles[5].z = -5;
			array_simple_tiles[6].x = -3; 
			array_simple_tiles[6].z = -5;
			array_simple_tiles[7].x = INT_MAX;
			break;
	}
	
	glutPostRedisplay();
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

    gluPerspective(30, (float) width/height, 1, 150);
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
	gluLookAt(18 - camera_parameter_in_out*3 + camera_parameter_x, 16 + camera_parameter_in_out*24, 22 + camera_parameter_in_out*8 - camera_parameter_z,
			  0 + camera_parameter_x, 0, 0 - camera_parameter_in_out*10 - camera_parameter_z,
			  0, 1, 0);
              
    if (!pressed_enter)
    {
		glPushAttrib(GL_LIGHTING_BIT);
			glPushMatrix();
				// glRasterPos3i(-15,-20,1);
				glRasterPos3i(-15 * camera_parameter_in_out_max,-20 * camera_parameter_in_out_max,1);
				GLfloat mat_ambient[] ={ 1, 1, 1, 1 };
				glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
				glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, array_of_moves);
			glPopMatrix();
		glPopAttrib();
	}
	
	
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
	
	float extra_static_rotation = 0;
	float extra_dynamic_rotation = 0;
	
	switch (current_pressed_key)
	{
		case 'w':
			z += animation_parameter*(-3.75);
			break;
		case 'a':
			x += animation_parameter*(-3.75);
			break;
		case 's':
			z += animation_parameter*(3.75);
			break;
		case 'd':
			x += animation_parameter*(3.75);
			break;
	}
	
	switch (previous_pressed_key)
	{
		case 'w':
			switch (current_pressed_key)
			{
				case 'a':
					extra_dynamic_rotation = animation_parameter*90; 
					break;
				case 's':
					extra_dynamic_rotation = animation_parameter*180; 
					break;
				case 'd':
					extra_dynamic_rotation = -animation_parameter*90;
					break;
			}
			break;
		case 'a':
			extra_static_rotation = 90;
			switch (current_pressed_key)
			{
				case 'w':
					extra_dynamic_rotation = -animation_parameter*90; 
					break;
				case 's':
					extra_dynamic_rotation = animation_parameter*90; 
					break;
				case 'd':
					extra_dynamic_rotation = animation_parameter*180;
					break;
			}
			break;
		case 's':
			extra_static_rotation = 180;
			switch (current_pressed_key)
			{
				case 'w':
					extra_dynamic_rotation = animation_parameter*180; 
					break;
				case 'a':
					extra_dynamic_rotation = -animation_parameter*90; 
					break;
				case 'd':
					extra_dynamic_rotation = animation_parameter*90;
					break;
			}
			break;
		case 'd':
			extra_static_rotation = 270;
			switch (current_pressed_key)
			{
				case 'w':
					extra_dynamic_rotation = animation_parameter*90; 
					break;
				case 'a':
					extra_dynamic_rotation = animation_parameter*180; 
					break;
				case 's':
					extra_dynamic_rotation = -animation_parameter*90;
					break;
			}
			break;
		default:
			break;
	}	
	
	GLfloat mat_ambient[] ={ 0.7, 0.0, 0.2, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	if (is_current_special_activated)
	{
		if (!last_special_tile_activated)
		{
			y = 0;
		}
		
		/* body */
		glPushMatrix();
			glTranslatef(x, y + 0.6, z);
			glRotatef(25,0,1,0);
			glRotatef(90,0,0,1);
			glScalef(1.3,1.3,1.3);
			glRotatef(animation_parameter*130,1,0,0);
			glutSolidTetrahedron();
		glPopMatrix();
		
	}
	else 
	{
		/* body */
		glPushMatrix();
			glTranslatef(x, y + 0.6, z);
			glRotatef(25,0,1,0);
			glRotatef(90,0,0,1);
			glScalef(1.3,1.3,1.3);
			glRotatef(extra_dynamic_rotation,1,0,0); 
			glRotatef(extra_static_rotation, 1,0,0);
			glutSolidTetrahedron();
		glPopMatrix();
	}
	
	/* head */
	glPushMatrix();
		glTranslatef(x,y + 2.1, z);
		glRotatef(animation_parameter*90,0,1,0);
		glScalef(0.65,0.65,0.65);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	
	/* hair */
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(extra_dynamic_rotation,0,1,0); 
		glRotatef(extra_static_rotation, 0,1,0);
		glTranslatef(0.6,2.5,0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(extra_dynamic_rotation,0,1,0); 
		glRotatef(extra_static_rotation, 0,1,0);
		glTranslatef(-0.6,2.5,0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	
	glPopAttrib();
}

void draw_boy()
{
	glPushAttrib(GL_LIGHTING_BIT);
	/*
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
	
	float triangle_optimization_array[TRIANGLE_OPTIMIZATION_ARRAY_LENGTH];
	
	int triangle_optimization_array_position = 0;
	for(x = -70; x < 70; x = x + 4)
	{
		for(z = -70; z < 70; z = z + 4)
		{
			if (index_of_random_array == RANDOM_ARRAY_LENGTH - 4)
				index_of_random_array = 0;
			float random_position = random_array[index_of_random_array++];
			float position_z = z - random_position;
			float position_x = x + random_position;
			
			if (triangle_optimization_array_position == TRIANGLE_OPTIMIZATION_ARRAY_LENGTH)
			{
				// draw_triangle_carpet_optimized(triangle_optimization_array, random_array, index_of_random_array);
				
				int group_index;
				int number_of_groups = 4;
				int i;
				GLfloat mat_ambient_1[] ={ 0.1, 1, 0.1, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient_1);
				
				GLfloat mat_ambient_2[] ={ 1, 1, 0.3, 1.0f };
				
				for(group_index = 0; group_index < number_of_groups*2; group_index = group_index + 2)
				{
					// when we're done with drawing the first "half" of the triangles we want to change the color
					if (group_index == number_of_groups)
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient_2);
					}
				
					glPushMatrix();
						glRotatef(90*random_array[index_of_random_array++], 0, 1, 0);
						glBegin(GL_TRIANGLES);
						for (i = group_index; i <= TRIANGLE_OPTIMIZATION_ARRAY_LENGTH; i = i + 8)
						{	
							glVertex3f(triangle_optimization_array[i],-0.4,triangle_optimization_array[i+1]);
							glVertex3f(triangle_optimization_array[i],-0.4, triangle_optimization_array[i+1] - 0.7);
							glVertex3f(triangle_optimization_array[i] + 0.7,-0.4,triangle_optimization_array[i+1]);
						}
						glEnd();
					glPopMatrix();
				}
				
				triangle_optimization_array_position = 0;
			}
			
			triangle_optimization_array[triangle_optimization_array_position++] = position_x;
			triangle_optimization_array[triangle_optimization_array_position++] = position_z;
		}
	}
	
	glPopMatrix();
	
	glPopAttrib();
}

void add_to_move_array(char move) 
{
	array_of_moves[current_move_index] = move;
	current_move_index++;
	array_of_moves[current_move_index] = '\0';
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
        	glutPostRedisplay();
        	break;  
        case 'C':
        case 'c':
        	// resetting input
        	if (pressed_enter)
        		break;
        	current_move_index = 0;
        	array_of_moves[0] = '\0';
        	glutPostRedisplay();
        	break;
        case 13: // code for ENTER
        	if (pressed_enter) 
        		break;
        	pressed_enter = 1;
        	current_move_index = 0;
        	current_pressed_key = array_of_moves[current_move_index];
			// change_key_pressed(array_of_moves[current_move_index]);
        	// animation_ongoing = 1;
        	// glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        	glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID_CAMERA_IN);
        	break;
        case 27:
          exit(0);
          break;
    }
}
// TODO spreci da se desava on oza kamerom zoom in out

void on_timer(int id) {
    
    if (id == TIMER_ID_CAMERA_OUT)
    {
    	if (camera_parameter_in_out >= camera_parameter_in_out_max)
    		return;
    		
    	camera_parameter_in_out += 0.005;
    	glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID_CAMERA_OUT);
    	
    }
    else if (id == TIMER_ID_CAMERA_IN)
    {
    	if (camera_parameter_in_out > 0)
    	{
			camera_parameter_in_out -= 0.008;
			glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID_CAMERA_IN);
    	}
    	else 
    	{
    		animation_ongoing = 1;
    		animation_parameter = 0;
    		glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    	}
    	
    }
    else if (animation_ongoing)
	{
		if (animation_parameter >= 1)
		{
			animation_parameter = 0;
			animation_ongoing = 0;
			if (current_pressed_key != 'e')
				previous_pressed_key = current_pressed_key;
			switch(current_pressed_key)
			{
				case 'w':
					previous_tile_z++;
					break;
				case 'a':
					previous_tile_x++;
					break;
				case 's':
					previous_tile_z--;
					break;
				case 'd':
					previous_tile_x--;
					break;
			}
			
			// TODO change
			is_current_special_activated = 0;
		}
		else {
			switch(current_pressed_key)
			{
				case 'w':
					camera_parameter_z += 0.08;
					break;
				case 'a':
					camera_parameter_x -= 0.08;
					break;
				case 's':
					camera_parameter_z -= 0.08;
					break;
				case 'd':
					camera_parameter_x += 0.08;
					break;
			}
			
			animation_parameter += 0.03;	
		}
		
		glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
	}
	else
	{
		if(check_all_specials_activated())
		{
			printf("Level Complete!\n");
			// previous_pressed_key = current_pressed_key;
			// TODO move all initializations to level set up method so that everything's reset properly
			set_arena_for_level(++current_level);
		}
		else 
		{
			current_move_index++;
			char current = array_of_moves[current_move_index];
			if (current == '\0') 
			{
				// previous_pressed_key = current_pressed_key;
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
							check_all_specials_activated();
						}
						current_special_index++;
					} 
				}
				
				// I don't want to change previous to e, it's supposed to keep track of the last movement thingie
				if (current_pressed_key != 'e')
					previous_pressed_key = current_pressed_key;
				
				current_pressed_key = array_of_moves[current_move_index];
				animation_ongoing = 1;
				animation_parameter = 0;
				glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
			}
		}
	}
	
	glutPostRedisplay();
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
	
	last_special_tile_activated = 1;
	return 1;
}

