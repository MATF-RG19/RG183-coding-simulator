/* ******************

I am aware that this amount of extern variables defeats the purpose of separating the code into separate files,
but it's far more readable this way than if I kept it all in one single file. 

******************* */

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "floor_setup.h"

extern int index_of_random_array;
extern float random_array[RANDOM_ARRAY_LENGTH];

extern int is_current_special_activated, previous_tile_z, previous_tile_x, is_final_level;
extern int array_of_moves[MAX_NUM_MOVES];
extern simple_tile array_simple_tiles[MAX_NUM_SIMPLE_TILES];
extern special_tile;
extern float animation_parameter, x, y, z, camera_parameter_in_out, camera_parameter_z, camera_parameter_x, camera_parameter_in_out_max;

extern char current_pressed_key, previous_pressed_key;
extern FILE* level_file;

extern special_tile array_special_tiles[MAX_NUM_SPECIAL_TILES];
extern simple_tile array_simple_tiles[MAX_NUM_SIMPLE_TILES];

void draw_triangle_carpet(GLfloat ambient1[4], GLfloat ambient2[4])
{
	glPushAttrib(GL_LIGHTING_BIT);

	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	
	int x;
	int z;
	int index_of_random_array = 0;
	
	float triangle_optimization_array[TRIANGLE_OPTIMIZATION_ARRAY_LENGTH];
	
	int triangle_optimization_array_position = 0;
	for(x = -60; x < 80; x = x + 4)
	{
		for(z = -110; z < 40; z = z + 4)
		{
			if (index_of_random_array == RANDOM_ARRAY_LENGTH - 4)
				index_of_random_array = 0;
			float random_position = random_array[index_of_random_array++];
			float position_z = z - random_position;
			float position_x = x + random_position;
			
			if (triangle_optimization_array_position == TRIANGLE_OPTIMIZATION_ARRAY_LENGTH)
			{
				int group_index;
				int number_of_groups = 4;
				int i;
				// GLfloat mat_ambient_1[] ={ 0.1, 1, 0.1, 1.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient1);
				
				// GLfloat mat_ambient_2[] ={ 1, 1, 0.3, 1.0f };
				
				for(group_index = 0; group_index < number_of_groups*2; group_index = group_index + 2)
				{
					// when we're done with drawing the first "half" of the triangles we want to change the color
					if (group_index == number_of_groups)
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient2);
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

void draw_floor()
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

void draw_final_level_carpet()
{
}

