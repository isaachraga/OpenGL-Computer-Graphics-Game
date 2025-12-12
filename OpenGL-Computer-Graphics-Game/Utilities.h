#pragma once

#include "Vector.h"
Vector3 toGrayscale(const Vector3& color);

// ========== Text Renderer =============
void drawText(float x, float y, const char* text);

//##From Powerpoint slides - orientation for arcball
void recomputeOrientation(float& arcx, float& arcy, float& arcz, float cameraRadius, float cameraTheta, float cameraPhi);



