#pragma once
#include "Vector.h"
#include "PrimitiveAssembly.h"
#include <vector>

// ========== Bullet Class =============
class Bullet {
public:
	GLint texIDIX;
	Vector3 pos;
	float angle;
	int id;
	float colliderRadius;
	static int nextid;

	Bullet(Vector3 Position, float Angle);

	void Draw(int mode);

	bool operator==(const Bullet& in) const;

};
