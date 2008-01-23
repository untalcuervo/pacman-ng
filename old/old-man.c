/*
 * This file is part of pacman-ng
 * 
 * pacman-ng A 3D version of the pacman game
 * Copyright (C) 2008	Frederic-Gerald Morcos (fredmorcos)
 * 						Marleine Mounir Daoud
 * 						Andrew Botros Boktor
 * 
 * pacman-ng is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * pacman-ng is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with pacman-ng.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/glut.h>
#include "man.h"
#include "material.h"

void man_draw (man *m)
{
	glPushMatrix ();
	if (m->evil)
		material_set_color (1.0, 0.0, 0.0);
	else
		material_set_color (1.0, 1.0, 0.0);
	
	
	glTranslatef (m->pos.x, m->pos.y, m->pos.z);
	glutSolidSphere (m->radius, 100, 100);
	glPopMatrix ();
}

void man_move (man *m)
{
	m->pos.x += m->velocity [0] * 0.05;;
	m->pos.y += m->velocity [1] * 0.05;;
	m->pos.z += m->velocity [2] * 0.05;;
}