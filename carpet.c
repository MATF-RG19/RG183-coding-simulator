#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "carpet.h"

extern int index_of_random_array;
extern float random_array[RANDOM_ARRAY_LENGTH];

void draw_triangle_carpet()
{
	glPushAttrib(GL_LIGHTING_BIT);

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
