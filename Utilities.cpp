
#include "Utilities.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

//========== Grayscale Adjustment =============
Vector3 toGrayscale(const Vector3& color) { //handles the color transformation, got values from stack overflow
	float gray = 0.299f * color.x +
		0.587f * color.y +
		0.114f * color.z;
	return { gray, gray, gray };
}

// ========== Text Renderer =============
void drawText(float x, float y, const char* text) {
	glRasterPos2f(x, y);
	for (const char* c = text; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

//##From Powerpoint slides - orientation for arcball
void recomputeOrientation(float& arcx, float& arcy, float& arcz, float cameraRadius, float cameraTheta, float cameraPhi)
{
	//std::cout << arcx << " " << arcy << " " << arcz << " " << cameraRadius << " " << cameraTheta << " " << cameraPhi << std::endl;
	arcx = cameraRadius * sinf(cameraTheta) * sinf(cameraPhi);
	arcz = cameraRadius * cosf(cameraTheta) * sinf(cameraPhi);
	arcy = cameraRadius * cosf(cameraPhi);
	glutPostRedisplay();
}



