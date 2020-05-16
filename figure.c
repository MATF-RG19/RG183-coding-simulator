#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define PI 3.1415926535897

extern float x, y, z, animation_parameter;
extern char current_pressed_key, previous_pressed_key;
extern int is_current_special_activated, last_special_tile_activated, previous_tile_x, previous_tile_z, level_failed;

void draw_girl()
{ 
	glPushAttrib(GL_LIGHTING_BIT);
	
	x = previous_tile_x*(-3.75);
	z = previous_tile_z*(-3.75);
	y = sin(animation_parameter * PI);
	
	float level_failed_head_translation_x = 0;
	float level_failed_head_translation_y = 0;
	float level_failed_head_translation_z = 0;
	
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
			level_failed_head_translation_y = -0.3*animation_parameter;
			level_failed_head_translation_z = -0.3*animation_parameter;
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
			level_failed_head_translation_x = -0.3*animation_parameter;
			level_failed_head_translation_y = -0.3*animation_parameter;
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
			level_failed_head_translation_y = -0.3*animation_parameter;
			level_failed_head_translation_z = 0.3*animation_parameter;
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
			level_failed_head_translation_x = 0.3*animation_parameter;
			level_failed_head_translation_y = -0.3*animation_parameter;
			level_failed_head_translation_z = 0;
			break;
		default:
			break;
	}	
	
	GLfloat mat_ambient[] ={ 0.7, 0.0, 0.2, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

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
			glTranslatef(level_failed_head_translation_x, level_failed_head_translation_y, level_failed_head_translation_z);
			glRotatef(animation_parameter*90,0,1,0);
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
			glRotatef(-20*animation_parameter, 1,0,0);
			glTranslatef(level_failed_head_translation_x, level_failed_head_translation_y, level_failed_head_translation_z);	
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
			glRotatef(-20*animation_parameter*level_failed, 1,0,0);
			glTranslatef(level_failed_head_translation_x, level_failed_head_translation_y, level_failed_head_translation_z);
		}
		glTranslatef(-0.6,2.5,0.5);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(1, 16, 16);
	glPopMatrix();
	
	glPopAttrib();
}
