#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "figure.h"
#include "floor_setup.h"

// The timer interval is the same for all animations
#define TIMER_INTERVAL 10

// The timer ID for regular figure animations
#define TIMER_ID 0

// Timer IDs related to camera movement at the beginning of a level)
#define TIMER_ID_CAMERA_OUT 1
#define TIMER_ID_CAMERA_IN 2

// The timer ID for the game complete animation
#define TIMER_ID_GAME_COMPLETE 3

static void on_display();
void set_arena_for_level(int level);
static void on_reshape(int width, int height);
void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);
void change_key_pressed(char);
int check_all_specials_activated(void);
void reset_specials(void);
int check_if_off_path(char move);
void draw_game_complete(void);

// A special animation plays when the last special tile has been activated on a level
int last_special_tile_activated = 0;

/* used to keep track of the light source so a small cube acting as a 
   lightbulb can be drawn to more easily see where the light is coming from */
static int light_x = -4;
static int light_y = 5; 
static int light_z = -3; 

// array used for randomizing the triangles in the "carpet"
float random_array[RANDOM_ARRAY_LENGTH];

float animation_parameter = 0;
int animation_ongoing = 0;

// where the figure should move now
char current_pressed_key = '\0';
// where the figure has moved last time (needed for proper turning animations)
char previous_pressed_key = 'w'; // because at the beginning the figure is facing forward

// coordinates that help track the figures position 
float z = 0;
float x = 0;
float y = 0;

// how many "x" tiles the figure has moved so war ( = num of "W" movements - num of "S" movements)
int previous_tile_z = 0;
// how many "z" tiles the figure has moved so war ( = num of "A" movements - num of "D" movements)
int previous_tile_x = 0;

// text related coordinates (the current input text information)
int raster_pos_x = 0;
int raster_pos_y = 0;

// keeping track of the movement input and how many moves were processed so far
unsigned char array_of_moves[MAX_NUM_MOVES];
int current_move_index = 0;

// whether the enter key was pressed or not (movement input not allowed after enter has been pressed)
int pressed_enter = 0;

// used for animation purposes, and to change the "activated" attribute of the appropriate special tile
int is_current_special_activated = 0;

// keeping track of the current level (incremented when all the special tiles of a level have been activated)
int current_level = 1;

// camera related parameters
float camera_parameter_in_out = 0;
float camera_parameter_in_out_max = 0; 
float camera_parameter_z = 0;
float camera_parameter_x = 0;
int camera_stops_at_x = 0;
int camera_stops_at_y = 0;
int camera_stops_at_z = 0;
int camera_look_at_x = 0;
int camera_look_at_y = 0;
int camera_look_at_z = 0;

// pressing the "R" button to reset is allowed only if a level has been failed
int level_failed = 0;

// the final level has a different carpet than the rest, also used to complete the game if 
// is_final_level is true and the level has been passed
int is_final_level = 0;

// the level has been failed if the figure moves to a position where there is no tile
int has_wandered_off_the_path = 0;

int game_complete = 0;

// the animations at the end of the game are separated into several different stages
int game_complete_animation_stage = 0;

// keeps track of all the special tiles for a level
special_tile array_special_tiles[MAX_NUM_SPECIAL_TILES];
// keeps track of all the special tiles for a level
simple_tile array_simple_tiles[MAX_NUM_SIMPLE_TILES];

// file containing level information
FILE* level_file;

/* in case the level has been reset, i.e. pressing 'r' after a level has been failed
we don't want to read the next level from the file */
int level_reset = 0;


int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(700, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Another Hour of Code");

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
	
	// opening the file containing level information
	level_file = fopen("levels.txt", "r");
	// seed for the random array used to draw the triangle carpet
	srand(time(NULL));
	// set the first move to blank just in case
	array_of_moves[0] = '\0';
	// set the first level
	set_arena_for_level(current_level);
	
	// initializing the array of random values used to draw the triangle carpet
	int i;
	for (i = 0; i < RANDOM_ARRAY_LENGTH; i++)
	{
		random_array[i] = (float) rand() / RAND_MAX;
	}
	
    glClearColor(0, 0, 0, 0);
    glutMainLoop();

	fclose(level_file);
    return 0;
}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) width/height, 1, 250);
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	/*
	If the game hasn't been completed, the camera position depends on several factors:
		- at the beginning of the level the camera zooms out from the figure up to a certain point
		- when enter has been pressed, the camera zooms back in on the figure
		- every time the figure moves the camera follows (both with the camera position and where the camera is looking)
	Which gives us the complicated formula below based on the settings for each level and where the figure is located while moving.
	
	If the game has been completed, no animations requiring camera movement are present so it can have a nice, fixed value.
	*/
	if (game_complete)
	{
		gluLookAt(6, 6, 20,
			  	  3, 1, 0,
			      0, 1, 0);
	}
	else 
	{
		float camera_location_x = 18 - camera_parameter_in_out*(camera_stops_at_x - 18) + camera_parameter_x;
		float camera_location_y = 16 + camera_parameter_in_out*(camera_stops_at_y - 16);
		float camera_location_z = 22 + camera_parameter_in_out*(camera_stops_at_x - 22) - camera_parameter_z;  
		
		
		gluLookAt(camera_location_x, camera_location_y, camera_location_z,
				  0 + camera_parameter_x + camera_parameter_in_out*camera_look_at_x, 0 + camera_parameter_in_out*camera_look_at_y, 0 - camera_parameter_in_out*camera_look_at_z - camera_parameter_z,
				  0, 1, 0);
	}

	/*
	To avoid another set of complicated equations for text position, the text only appears before the user has pressed enter,
	since the text isn't of much use after that anyway
	*/
    if (!pressed_enter)
    {
		glPushAttrib(GL_LIGHTING_BIT);
			glPushMatrix();
				glRasterPos3i(raster_pos_x, raster_pos_y, 1);
				GLfloat mat_ambient[] ={ 1, 1, 1, 1 };
				glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
				glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, array_of_moves);
			glPopMatrix();
		glPopAttrib();
	}
	
	/* draw light bulb if needed */
	/*
    glPushMatrix();
    	glTranslatef(light_x, light_y, light_z);
    	glutSolidCube(0.2);
    glPopMatrix();
    */
    
    /*
    The final level has a special carpet, so the usual triangle carpet isn't drawn
    */
    if (!is_final_level)
    {
		GLfloat ambient1[] ={ 0.1, 1, 0.1, 1.0f };
		//GLfloat ambient1[] ={ (float)140/255, (float)170/255, (float)243/255, 1.0f };
		GLfloat ambient2[] ={ 1, 1, 0.3, 1.0f };
		//GLfloat ambient2[] ={ (float)161/255,(float)25/255, (float)34/255, 1.0f };
		draw_triangle_carpet(ambient1, ambient2);
	}
	else 
	{
		draw_final_level_carpet();
	}
	
	/*
	The game complete state has a different style of animation and a different look,
	without the triangle carpet, floors, and with a special prize of finishing the game.
	*/
	if (game_complete)
	{
		GLfloat ambient1[] ={ 1, 1, 1, 1.0f };
		GLfloat ambient2[] ={ 1, 1, 0.3, 1.0f };
		draw_triangle_carpet(ambient1, ambient2);
		draw_game_complete();
		glPushMatrix();
			glTranslatef(1, 0, 0);   
			draw_girl();    
		glPopMatrix();
	}
	else 
	{
		/* if the game is still in progress, the tiles and the figure should be drawn */
		draw_floor();
		draw_girl(); 
		draw_special();
	}
		 
    glPopMatrix();

    glutSwapBuffers();
}

/* simple method to update the values in the move array, and make sure that the "next" move is 
	set to blank to avoid unusual behavior when reading that value */
void add_to_move_array(char move) 
{
	array_of_moves[current_move_index] = move;
	current_move_index++;
	array_of_moves[current_move_index] = '\0';
}

void on_keyboard(unsigned char key, int x, int y) {
    
    // no input is allowedduring the game complete scene except exiting
    if (game_complete && key != 27)
	{
		return;
	}
   
    switch(key) {
    	/*
    	the behavior is the same for all movement keys, including "e', simply add it to the move array
    	and print the array out
    	*/
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
        	// clear last move
        	if (pressed_enter)
        		break;
        	array_of_moves[--current_move_index] = '\0';
        	glutPostRedisplay();
        	break;
        case 'r':
        case 'R':
        	// reset level if the level was failed
        	if(level_failed)
        	{
        		level_reset = 1;
        		set_arena_for_level(current_level);
        	}
        	break;
        case 'g':
        	// uncomment for God Mode
        	set_arena_for_level(++current_level);
        	break;
        case 13: // code for ENTER
        	/*
        	When enter is pressed, the sequence of moves stored in the move array is performed
        	after pressing enter, no additional moves can be added, only the "R" key can be pressed if 
        	the level has been failed
        	*/
        	if (pressed_enter) 
        		break;
        	pressed_enter = 1;
        	
        	// start at the beginning of the recorded move sequence 
        	current_move_index = 0;
        	current_pressed_key = array_of_moves[current_move_index];
        	
        	// bring the camera back in on the figure
        	glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID_CAMERA_IN);
        	break;
        case 27:
          exit(0);
          break;
    }
}

/*
	special seen when the game has been finished
*/
void draw_game_complete(void)
{	
	glPushAttrib(GL_LIGHTING_BIT);
	
	GLfloat mat_ambient[] ={ 0.24725f, 0.1995f, 0.0745f, 1.0f };
	GLfloat mat_diffuse[] ={0.75164f, 0.60648f, 0.22648f, 1.0f };
	GLfloat mat_specular[] ={0.628281f, 0.555802f, 0.366065f, 1.0f };
	GLfloat shine[] = {51.2f} ;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	
	double plane[] = {0, -1, 0, 3.8};
	glClipPlane(GL_CLIP_DISTANCE0, plane);
	glEnable(GL_CLIP_DISTANCE0);

	glPushMatrix();
		glTranslatef(6, 3, 0);
		glScalef(0.8, 1, 0.8);
		glutSolidSphere(2, 32, 32);
	glPopMatrix();
	
	glDisable(GL_CLIP_DISTANCE0);
	
	glPushMatrix();
		glTranslatef(6, 0, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(1.3, 1.5, 16, 16);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(4.2, 2.7, 0);
		glScalef(0.8, 1, 1);
		glutSolidTorus(0.1,0.7,32,32);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(7.7, 2.7, 0);
		glScalef(0.8, 1, 1);
		glutSolidTorus(0.1,0.7,32,32);
	glPopMatrix();
	
	glPopAttrib();
}

/*
	used to check whether the last move didn't end on one of the tiles 
*/
int check_if_off_path(char move)
{
	int x_to_check = previous_tile_x;
	int z_to_check = previous_tile_z;
	
	// have to check both special and simple tiles
	int current_special_index = 0;
	while (array_special_tiles[current_special_index].x != INT_MAX)
	{
		special_tile current_special = array_special_tiles[current_special_index];
		if (current_special.x == -x_to_check && current_special.z == -z_to_check)
		{
			return 0;
		}
		current_special_index++;
	} 
	
	int current_simple_index = 0;
	while (array_simple_tiles[current_simple_index].x != INT_MAX)
	{
		simple_tile current_simple = array_simple_tiles[current_simple_index];
		if (current_simple.x == -x_to_check && current_simple.z == -z_to_check)
		{
			return 0;
		}
		current_simple_index++;
	} 
	
	// couldn't find a tile (either special or simple) that had those coordinates
	// i.e. the figure wandered off the path
	has_wandered_off_the_path = 1;
	
	return 1;
}

// TODO fix: spreci da se desava ono sa kamerom zoom in out
void on_timer(int id) {
    
    switch(id)
    {
    	case TIMER_ID_CAMERA_OUT: // camera movement at the beginning of the level (zooming out from the figure)
			if (camera_parameter_in_out >= camera_parameter_in_out_max)
				return;
				
			camera_parameter_in_out += 0.005;
			glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID_CAMERA_OUT);
    		break;
    		
    	case TIMER_ID_CAMERA_IN: // camera movement after pressing enter for the first time (zooming in on the figure)
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
    		break;
    		
    	case TIMER_ID_GAME_COMPLETE:   // special animation in case the game has been completed
       		if (animation_parameter >= 1)
			{	
				game_complete_animation_stage++;
				if (game_complete_animation_stage == 11)
				{
					game_complete_animation_stage = 0;
				}
				animation_parameter = 0;
			}
			else
			{
				animation_parameter += 0.02;
			}
			glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID_GAME_COMPLETE);
			break;
			
    	default: // regular animation
    	/* 
    	There are a lot of things that need to be checked when it comes to regular animations.
    	First thing - whether an animation is currently happening. There's a special case here (in case the tile the level
    	is started at is a special one, and activated), we need to "manually" activate it. 
    	
    	Then we need to check the animation parameter. If it has exceeded 1, we need to change some data and start the next animation,
    	and handle the case where the previous move has caused a level failed state.
    	
    	Then if the level has no been previously failed, we need to check:
    		- whether all of the specials have been actived (level complete)
    		- go to the next level if there is one
   			- animate the next move in the sequence if the level was neither failed not completed

    	*/
    	if (animation_ongoing)
		{
			// handling the "special" case when the first tile is a special tile and it's activated
			if (array_of_moves[0] == 'e' && animation_parameter == 0 && current_move_index == 0)
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
			
			// handling the "special" case when no moves have been entered at all
			if (array_of_moves[0] == '\0' && animation_parameter == 0 && current_move_index == 0)
			{
				level_failed = 1;
			}
		
			if (animation_parameter >= 1)
			{
				if (!level_failed)
				{
					// if the level wasn't failed, we register where the figure has moved now 
					// i.e. increase previous_tile_z and previous_tile_x depending on where the figure moved
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
				}
				
				animation_parameter = 0;
				animation_ongoing = 0;
				
				// setting this to false just in case 
				is_current_special_activated = 0;
			}
			else {
				if (!level_failed)
				{
					switch(current_pressed_key)
					{
						case 'w':
							camera_parameter_z += 0.1;
							break;
						case 'a':
							camera_parameter_x -= 0.1;
							break;
						case 's':
							camera_parameter_z -= 0.1;
							break;
						case 'd':
							camera_parameter_x += 0.1;
							break;
					}
				}
				
				// if the level has been failed, the special "sad" animation is playing and the figure isn't
				// moving on the x or z axis
				animation_parameter += 0.03;	
			}
			
			glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
		}
		else if (!level_failed)
		{
			// if there level hasn't been failed, we need to check whether it was completed, 
			// if not, we need to animate the next move
			if(check_all_specials_activated())
			{
				printf("Level Complete!\n");
				if (is_final_level)
				{
					game_complete = 1;
					animation_parameter = 1;
					animation_parameter = 0;
					set_arena_for_level(1);
					glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID_GAME_COMPLETE);
					is_final_level = 0;
				}
				else 
					set_arena_for_level(++current_level);
			}
			else
			{
				current_move_index++;
				char current = array_of_moves[current_move_index];
				if (current == '\0') 
				{
					// previous_pressed_key = current_pressed_key;
					// sad animation has to play
					level_failed = 1;
					animation_ongoing = 1;
					animation_parameter = 0;
					current_pressed_key = '\0';
					
					glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
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
					
					// we don't want to change previous pressed key to e since we use that information when rotating the figure, 
					// it's supposed to keep track of the last movement 
					if(check_if_off_path(current_pressed_key))
					{
						level_failed = 1;
						animation_ongoing = 1;
						animation_parameter = 0;
						current_pressed_key = '\0';
						
						glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
					}
					else {
						if (current_pressed_key != 'e')
						{
							previous_pressed_key = current_pressed_key;
						}
						
						current_pressed_key = current;
						animation_ongoing = 1;
						animation_parameter = 0;
						glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
					}
				}
			}
		}
    	break;
    }
	
	glutPostRedisplay();
}

/*
	check whether all of the special tiles have been activated, if so the level is passed 
*/
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

void reset_specials()
{
	int current_special_index = 0;
	while(array_special_tiles[current_special_index].x != INT_MAX)
	{
		array_special_tiles[current_special_index].activated = 0;
		current_special_index++;
	}
}

void set_arena_for_level(int level)
{
	has_wandered_off_the_path = 0;
	last_special_tile_activated = 0;
	is_current_special_activated = 0;
	level_failed = 0;

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

	camera_parameter_in_out = 0;
	camera_parameter_z = 0;
	camera_parameter_x = 0;

	array_of_moves[0] = '\0';

	glutTimerFunc(250, on_timer, TIMER_ID_CAMERA_OUT);
	
	int num_simple_tiles;
	int num_special_tiles;
	int end;
	int current_x;
	int current_z;
	
	if (!level_reset)
	{
		fscanf(level_file, "%d", &end);
		if (end == -1)
		{
			return;
		}
		fscanf(level_file, "%d%d", &num_simple_tiles, &num_special_tiles);
		
		int i;
		for (i = 0; i < num_simple_tiles; i++)
		{
			fscanf(level_file, "%d%d", &current_x, &current_z);
			array_simple_tiles[i].x = current_x; 
			array_simple_tiles[i].z = current_z;
		}
		
		array_simple_tiles[i].x = INT_MAX;
		
		for (i = 0; i < num_special_tiles; i++)
		{
			fscanf(level_file, "%d%d", &current_x, &current_z);
			array_special_tiles[i].x = current_x; 
			array_special_tiles[i].z = current_z;
			array_special_tiles[i].activated = 0;
		}
		
		array_special_tiles[i].x = INT_MAX;
	}
	else 
	{
		reset_specials();
	}
	
	level_reset = 0;

	switch(level)
	{
		case 1: // straight line
			camera_parameter_in_out_max = 0.7;
			camera_stops_at_x = 21;
			camera_stops_at_y = 28;
			camera_stops_at_z = 28;
			camera_look_at_x = 0;
			camera_look_at_y = 0;
			camera_look_at_z = 5;
			raster_pos_x = -11;
			raster_pos_y = -10;
			break;
		case 2: // turn right
			camera_parameter_in_out_max = 1;
			camera_stops_at_x = 21;
			camera_stops_at_y = 20;
			camera_stops_at_z = 35;
			camera_look_at_x = 3;
			camera_look_at_y = 0;
			camera_look_at_z = 3;
			raster_pos_x = -7;
			raster_pos_y = -8;
			break;
		case 3: // cross
			camera_parameter_in_out_max = 1;
			camera_stops_at_x = 21;
			camera_stops_at_y = 20;
			camera_stops_at_z = 35;
			camera_look_at_x = 0;
			camera_look_at_y = 0;
			camera_look_at_z = 3;
			raster_pos_x = -7;
			raster_pos_y = -8;
			break;
		case 4: // tuning-fork looking thing
			camera_parameter_in_out_max = 1;
			camera_stops_at_x = 21;
			camera_stops_at_y = 20;
			camera_stops_at_z = 35;
			camera_look_at_x = 3;
			camera_look_at_y = 1;
			camera_look_at_z = 3;
			raster_pos_x = -7;
			raster_pos_y = -8;
			break;
		case 5: // cactus
			camera_parameter_in_out_max = 1;
			camera_stops_at_x = 21;
			camera_stops_at_y = 20;
			camera_stops_at_z = 35;
			camera_look_at_x = 3;
			camera_look_at_y = 0;
			camera_look_at_z = 3;
			raster_pos_x = -7;
			raster_pos_y = -8;
			break;	
		case 6: // heart
			camera_parameter_in_out_max = 1.6;
			camera_stops_at_x = 26;
			camera_stops_at_y = 25;
			camera_stops_at_z = 40;
			camera_look_at_x = 0;
			camera_look_at_y = 0;
			camera_look_at_z = 4;
			raster_pos_x = -7;
			raster_pos_y = -8;
			break;	
		case 7: // mug
			camera_parameter_in_out_max = 2.4;
			camera_stops_at_x = 26;
			camera_stops_at_y = 25;
			camera_stops_at_z = 50;
			camera_look_at_x = 5;
			camera_look_at_y = 0;
			camera_look_at_z = 4;
			raster_pos_x = 6;
			raster_pos_y = 20;
			break;
		case 8: // gameboy
			camera_parameter_in_out_max = 2.8;
			camera_stops_at_x = 26;
			camera_stops_at_y = 25;
			camera_stops_at_z = 40;
			camera_look_at_x = 0;
			camera_look_at_y = 0;
			camera_look_at_z = 4;
			raster_pos_x = -16;
			raster_pos_y = -6;
			break;
		case 9: // ghost
			camera_parameter_in_out_max = 2.8;
			camera_stops_at_x = 26;
			camera_stops_at_y = 25;
			camera_stops_at_z = 50;
			camera_look_at_x = 1;
			camera_look_at_y = 0;
			camera_look_at_z = 4;
			raster_pos_x = -12;
			raster_pos_y = -6;
			break;
		case 10: // deer
			camera_parameter_in_out_max = 2.8;
			camera_stops_at_x = 26;
			camera_stops_at_y = 25;
			camera_stops_at_z = 40;
			camera_look_at_x = 0.5;
			camera_look_at_y = 0;
			camera_look_at_z = 4;
			raster_pos_x = -16;
			raster_pos_y = -6;
			break;
		case 11: // CHICKEN
			is_final_level = 1;
			camera_parameter_in_out_max = 3.2;
			camera_stops_at_x = 30;
			camera_stops_at_y = 40;
			camera_stops_at_z = 60;
			camera_look_at_x = 3;
			camera_look_at_y = 0;
			camera_look_at_z = 3;
			raster_pos_x = -25;
			raster_pos_y = -10;
			break;
	}
	
	glutPostRedisplay();
}

