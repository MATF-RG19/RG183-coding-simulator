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
#include "figure.h"

extern float x, y, z, animation_parameter;
extern char current_pressed_key, previous_pressed_key, animation_ongoing;
extern int is_current_special_activated, last_special_tile_activated, previous_tile_x, previous_tile_z, level_failed, game_complete, game_complete_animation_stage;

float level_failed_head_translation_x = 0;
float level_failed_head_translation_y = 0;
float level_failed_head_translation_z = 0;

// variables related to the game complete animation
float head_translation_x = 0;
float head_translation_y = 0;
float head_translation_z = 0;

float hair_translation_x = 0;
float hair_translation_y = 0;
float hair_translation_z = 0;

/*
	The draw girl process is a bit complicated. We have to keep track of what tile she starts the animation at, 
	which direction she's facing, what part of the animation she's in, whether the "sad" animation should play, etc etc. 
*/
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
			level_failed_head_translation_x = 0;
			level_failed_head_translation_y = -0.3;
			level_failed_head_translation_z = -0.3;
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
			level_failed_head_translation_x = -0.3;
			level_failed_head_translation_y = -0.3;
			level_failed_head_translation_z = 0;
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
			level_failed_head_translation_x = 0;
			level_failed_head_translation_y = -0.3;
			level_failed_head_translation_z = 0.3;
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
			level_failed_head_translation_x = 0.3;
			level_failed_head_translation_y = -0.3;
			level_failed_head_translation_z = 0;
			break;
		default:
			break;
	}	
	
	GLfloat mat_ambient[] ={ 0.7, 0.0, 0.2, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	
	if (game_complete)
	{
		draw_girl_game_complete();
	}
	else 
	{
		if (level_failed)
		{
			y = 0;
		}
		
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
			if (level_failed)
			{
				glTranslatef(level_failed_head_translation_x*animation_parameter, 
							 level_failed_head_translation_y*animation_parameter,
							 level_failed_head_translation_z*animation_parameter);
				if (animation_parameter == 0 && animation_ongoing == 0)
				{
					glTranslatef(level_failed_head_translation_x, level_failed_head_translation_y, level_failed_head_translation_z);
				}
				// glRotatef(animation_parameter*90,0,1,0);
			}
			glScalef(0.65,0.65,0.65);
			glutSolidSphere(1, 16, 16);
		glPopMatrix();
		
		/* hair */
		glPushMatrix();
			glTranslatef(x,y,z);
			glRotatef(extra_dynamic_rotation,0,1,0); 
			glRotatef(extra_static_rotation, 0,1,0);
			if (level_failed)
			{
				float parameter = animation_parameter == 0 ? 1 : animation_parameter;
				glRotatef(-20*parameter, 1,0,0);
				glTranslatef(level_failed_head_translation_x*parameter, 
							 level_failed_head_translation_y*parameter,
							 level_failed_head_translation_z*parameter);
			}
			glTranslatef(0.6,2.5,0.5);
			glScalef(0.2,0.2,0.2);
			glutSolidSphere(1, 16, 16);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(x,y,z);
			glRotatef(extra_dynamic_rotation,0,1,0); 
			glRotatef(extra_static_rotation, 0,1,0);
			if (level_failed)
			{
				float parameter = animation_parameter == 0 ? 1 : animation_parameter;
				glRotatef(-20*parameter, 1,0,0);
				glTranslatef(level_failed_head_translation_x*parameter, 
							 level_failed_head_translation_y*parameter,
							 level_failed_head_translation_z*parameter);
			}
			glTranslatef(-0.6,2.5,0.5);
			glScalef(0.2,0.2,0.2);
			glutSolidSphere(1, 16, 16);
		glPopMatrix();
	}
	glPopAttrib();
}

void draw_girl_game_complete()
{
	float extra_static_rotation = 0;
	float extra_dynamic_rotation = 0;

	y = 0;
	head_translation_x = 0;
	head_translation_y = 0;
	head_translation_z = 0;
	
	hair_translation_x = 0;
	hair_translation_y = 0;
	hair_translation_z = 0;
	
	switch(game_complete_animation_stage)
	{
		case 0:
		case 6:
			extra_static_rotation = 190;
			extra_dynamic_rotation = animation_parameter*30;
			break;
		case 1:
		case 7:
			extra_static_rotation = 220;
			head_translation_x = 0.3;
			head_translation_y = -0.3;
			head_translation_z = 0.3;
			
			hair_translation_x = -2*head_translation_x;
			hair_translation_y = 1.5*head_translation_y;
			hair_translation_z = 2*head_translation_z;
			break;
		case 2:
		case 8:
			extra_static_rotation = 220;
			extra_dynamic_rotation = animation_parameter*(-50);
			break;
		case 3:
		case 9:
			extra_static_rotation = 170;
			head_translation_x = -0.3;
			head_translation_y = -0.3;
			head_translation_z = 0.3;
			
			hair_translation_x = -2*head_translation_x;
			hair_translation_y = 1.5*head_translation_y;
			hair_translation_z = 2*head_translation_z;
			break;
		case 4:
		case 10:
			extra_static_rotation = 170;
			extra_dynamic_rotation = animation_parameter*(20);
			break;
		case 5:
			extra_static_rotation = 190;
			head_translation_x = -0.05;
			head_translation_y = -0.3;
			head_translation_z = 0.3;
			
			hair_translation_x = -3*head_translation_x;
			hair_translation_y = head_translation_y;
			hair_translation_z = head_translation_z;
			
			break;
	}
	
	
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
	
	
	/* head */
	glPushMatrix();
		glTranslatef(x,y + 2.1, z);
		glTranslatef(head_translation_x * animation_parameter, 
					 head_translation_y * animation_parameter,
					 head_translation_z * animation_parameter);
		glScalef(0.65,0.65,0.65);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	
	/* hair */
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(extra_dynamic_rotation,0,1,0); 
		glRotatef(extra_static_rotation, 0,1,0);
		glTranslatef(hair_translation_x*animation_parameter, 
					 hair_translation_y*animation_parameter,
					 hair_translation_z*animation_parameter);
		glTranslatef(0.6,2.5,0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(extra_dynamic_rotation,0,1,0); 
		glRotatef(extra_static_rotation, 0,1,0);
		glTranslatef(hair_translation_x*animation_parameter, 
					 hair_translation_y*animation_parameter,
					 hair_translation_z*animation_parameter);
		glTranslatef(-0.6,2.5,0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
}
