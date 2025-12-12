#pragma once

#include <math.h>
#include "Vector.h"
#include <GL/glut.h>
#include <GL/freeglut.h>


#define PI 3.141592653589793238462646

//##From Powerpoint slides
// ========== Object Creation =============
//handles cube instantiation
void SpawnCube(int m, GLint texIDIX, Vector3 pos, Vector3 scale, Vector3 rot, Vector3 color);

//handles sphere instantiation
void SpawnSphere(int m, GLint texIDIX, GLint slices, GLint stack, Vector3 pos, Vector3 scale, Vector3 rot, Vector3 color);