#pragma once

#include "Vector.h"

// ========== Collision Handling =============
bool sphereCollision(const Vector3 aPos, float aR, const Vector3 bPos, float bR) {
	float r = aR + bR;
	//difference in position
	Vector3 tmp = Vector3(aPos.x - bPos.x, aPos.y - bPos.y, aPos.z - bPos.z);
	//dot product less than radius squared
	return (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z) <= r * r;
}