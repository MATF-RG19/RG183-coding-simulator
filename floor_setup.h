#ifndef LEVEL
#define LEVEL
#define MAX_NUM_MOVES 200
#define MAX_NUM_SIMPLE_TILES 150
#define MAX_NUM_SPECIAL_TILES 40
#define RANDOM_ARRAY_LENGTH 120
#define TRIANGLE_OPTIMIZATION_ARRAY_LENGTH 100

typedef struct simple_tile_struct {
	int x;
	int z;
} simple_tile;

typedef struct special_tile_struct {
	int x;
	int z;
	int activated;
} special_tile;

void draw_triangle_carpet(GLfloat ambient1[4], GLfloat ambient2[4]);
void draw_floor();
void draw_special();
void draw_final_level_carpet();
void godhelpme();
#endif
