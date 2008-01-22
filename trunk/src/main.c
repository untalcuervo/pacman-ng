#include <stdlib.h>
#include <GL/freeglut.h>
#include "glut-extra.h"
#include "opengl-render.h"
#include "ground.h"
#include "bille.h"
#include "wall.h"

#define WIN_W 800
#define WIN_H 600

void cbKeyboard (unsigned char ucKey, int iX, int iY);
void cbKeyboardSpecial (int iKey, int iX, int iY);

void cbDisplay (void);
void cbExit (void);

ground mainGround = {NULL, 10.0, 0.6, 10.0};

int main (int argc, char *argv [])
{
	atexit (cbExit);
	glutInit (&argc, argv);
	initGlutWindow (WIN_W, WIN_H);
	initGlut ();
	initOpenGL (WIN_W, WIN_H);
	glutMainLoop ();
	return 0;
}

void marlyBille ()
{
	bille *bille1 = newBille(0.05, 0.0, 0.0, 0.0, 1);
	bille_draw(bille1);
}

void andrewsTest ()
{
	wall *w = wall_new (0.0, 0.0, 1.0, HORIZONTAL, 10);
	wall_draw (w);
	
	glutSolidSphere (0.1, 100, 100);
}

void cbDisplay ()
{
	renderOpenGL ();
	renderGround (&mainGround);
	
	/*==================== ANDREW'S TESTING CODE ====================*/
	/* Comment My Testing Function to Disable the testing            */
	
	//andrewsTest();
	
	/*================= END OF ANDREW'S TESTING CODE ================*/
	
	/*==================== MARLY'S TESTING CODE ====================*/
	/* Comment My Testing Function to Disable the testing            */
	
	marlyBille();
	
	/*================= END OF MARLY'S TESTING CODE ================*/
	
	glutSwapBuffers ();
}

void cbKeyboard (unsigned char ucKey, int iX, int iY)
{
	switch (ucKey)
	{
		case 'q':
			glutLeaveMainLoop ();
			break;
	}
}

void cbKeyboardSpecial (int iKey, int iX, int iY)
{
	switch (iKey)
	{
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
	}
}

void initGlut ()
{
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_ACCUM | GLUT_DEPTH);
	glutDisplayFunc (cbDisplay);
//	glutIdleFunc (renderOpenGL);
//	glutReshapeFunc (resize);
	glutKeyboardFunc (cbKeyboard);
	glutSpecialFunc (cbKeyboardSpecial);
}

void cbExit ()
{
	/* free your stuff here, we're exiting */
}