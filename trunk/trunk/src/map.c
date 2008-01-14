#include <GL/glut.h>

#include "map.h"


void map_wall_draw ( int posx, int posz )
{
	glPushMatrix ();
	glColor3f ( 0.0, 0.0, 1.0 );
	glTanslated ( posx*MAP_CELL_SIZE, 0.5*MAP_CELL_SIZE, posz*MAP_CELL_SIZE );
	glScaled ( MAP_CELL_SIZE, 1, MAP_CELL_SIZE );
	glSolidCube ( 1 );
	glPopMatrix ();
}

void map_draw (map *m)
{
	
}

int map_cell_empty ( map* m, int posx, int posz )
{
	return !m->squares[posx][posz];
}
